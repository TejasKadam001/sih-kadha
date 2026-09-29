/*
 * iKwath control firmware (ESP32 DevKit V1)
 *
 * Runs the brew cycle described in docs/control-sequence.md:
 *   IDLE -> READY -> FILL -> SOAK -> BOIL -> VENT -> DISPENSE -> EJECT -> DONE
 * with FAULT reachable from every powered state.
 *
 * The heater is also protected by hardware in series with it (lid interlock,
 * bimetallic cutoff, fuse); the checks here are an additional software layer.
 *
 * Serial commands (115200 baud), one per line:
 *   s        start the cycle (pod must be read and lid closed)
 *   x        abort and return to idle
 *   r        clear a fault (only when the cause is gone)
 *   t        tare the load cell
 *   cal <g>  calibrate with a known mass (in grams) on the scale; stored in flash
 *   h        help
 *
 * Status: first draft written from the project specification. It has not been
 * compiled or run on the physical appliance; expect first-build fixes, and measure
 * every CALIBRATE value in config.h before a powered test.
 */

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <math.h>
#include <Preferences.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>
#include <MFRC522.h>
#include <ESP32Servo.h>

#include "pins.h"
#include "config.h"
#include "pid.h"
#include "hx711_lite.h"
#include "profile.h"

#if ENABLE_WIFI
#include <WiFi.h>
#include <WebServer.h>
#endif

// ------------------------------------------------------------------ devices
OneWire            oneWire(PIN_DS18B20);
DallasTemperature  liquidProbe(&oneWire);
Adafruit_SSD1306   oled(128, 64, &Wire, -1);
Adafruit_BMP280    bmp(&Wire);
MFRC522            nfc(PIN_NFC_SS, PIN_NFC_RST);
Servo              ejector;
Hx711Lite          scale(PIN_HX711_DT, PIN_HX711_SCK);
Preferences        prefs;
Pid                heaterPid(PID_KP, PID_KI, PID_KD);

// ------------------------------------------------------------------ state
enum State : uint8_t { S_IDLE, S_READY, S_FILL, S_SOAK, S_BOIL, S_VENT, S_DISPENSE, S_EJECT, S_DONE, S_FAULT };
const char *STATE_NAMES[] = { "IDLE", "READY", "FILL", "SOAK", "BOIL", "VENT", "DISPENSE", "EJECT", "DONE", "FAULT" };

State       state = S_IDLE;
uint32_t    stateSince = 0;
String      faultReason = "";
bool        oledOk = false, bmpOk = false;

BrewProfile profile;
bool        podReady = false;

// live readings
float   liquidC = NAN;
float   chamberKpa = NAN;
float   enclosureC = NAN;
float   massG = NAN;          // grams above tare
float   tdsPpm = 0.0f;
bool    lidClosed = false, boilOver = false, waterPresent = false;

// scale
double  tareRaw = 0.0;
float   countsPerG = HX711_COUNTS_PER_G;
bool    scaleOk = false;

// cycle bookkeeping
float   massBase = 0.0f;       // mass before filling
float   massAtBoilStart = 0.0f;
float   massBeforeDispense = 0.0f;
float   evaporatedG = 0.0f;
float   heaterDuty = 0.0f;
bool    vacuumPumpOn = false;
bool    startRequested = false;

// output shadows
uint8_t pcfRelays = 0xFF;      // relay bits: 1 = off when active-low
uint8_t pcfLeds   = 0xFF;
uint32_t heaterWindowStart = 0;
bool     heaterPinOn = false;

uint32_t lastSensor = 0, lastTelemetry = 0, lastDisplay = 0, lastPid = 0;
uint32_t cycleStart = 0;

#if ENABLE_WIFI
WebServer server(80);
#endif

// ------------------------------------------------------------------ helpers
void setState(State s) {
  state = s;
  stateSince = millis();
  Serial.printf("{\"event\":\"state\",\"state\":\"%s\"}\n", STATE_NAMES[s]);
}

void pcfWrite(uint8_t addr, uint8_t value) {
  Wire.beginTransmission(addr);
  Wire.write(value);
  Wire.endTransmission();
}

void relaySet(uint8_t bit, bool on) {
  bool level = RELAY_ACTIVE_LOW ? !on : on;
  if (level) pcfRelays |= (1 << bit); else pcfRelays &= ~(1 << bit);
  pcfWrite(ADDR_PCF_RELAYS, pcfRelays);
}

void buzzer(bool on) {   // buzzer bit is active high
  if (on) pcfRelays |= (1 << PCF_BUZZER); else pcfRelays &= ~(1 << PCF_BUZZER);
  pcfWrite(ADDR_PCF_RELAYS, pcfRelays);
}

void ledSet(uint8_t bit, bool on) {
  bool level = LED_ACTIVE_LOW ? !on : on;
  if (level) pcfLeds |= (1 << bit); else pcfLeds &= ~(1 << bit);
}

void ledsShow() { pcfWrite(ADDR_PCF_LEDS, pcfLeds); }

void heaterPin(bool on) {
  heaterPinOn = on;
  bool level = RELAY_ACTIVE_LOW ? !on : on;
  digitalWrite(PIN_HEATER, level ? HIGH : LOW);
}

void heaterOff() { heaterDuty = 0.0f; heaterPid.reset(); heaterPin(false); }

// Everything that can hurt anyone goes off. Lid lock is released so the pod can be removed.
void allOutputsOff() {
  heaterOff();
  pcfRelays = RELAY_ACTIVE_LOW ? 0xFF : 0x00;   // all relays off
  pcfRelays &= ~(1 << PCF_BUZZER);
  pcfWrite(ADDR_PCF_RELAYS, pcfRelays);
  vacuumPumpOn = false;
}

void fault(const String &why) {
  if (state == S_FAULT) return;
  allOutputsOff();
  faultReason = why;
  Serial.printf("{\"event\":\"fault\",\"reason\":\"%s\"}\n", why.c_str());
  setState(S_FAULT);
}

// ------------------------------------------------------------------ sensors
float readEnclosureC() {
  int raw = analogRead(PIN_NTC);
  if (raw <= 10 || raw >= 4085) return NAN;                  // open or shorted
  float r = NTC_R_FIXED * raw / (4095.0f - raw);             // NTC to ground
  float t = 1.0f / (1.0f / 298.15f + logf(r / NTC_R25) / NTC_BETA) - 273.15f;
  return t;
}

float readTdsPpm(float tempC) {
  // Standard analog-TDS conversion with linear temperature compensation.
  float v = analogRead(PIN_TDS) * 3.3f / 4095.0f;
  float comp = 1.0f + 0.02f * ((isnan(tempC) ? 25.0f : tempC) - 25.0f);
  float vc = v / comp;
  float ppm = (133.42f * vc * vc * vc - 255.86f * vc * vc + 857.39f * vc) * 0.5f;
  return ppm < 0 ? 0 : ppm;
}

bool tareScale() {
  double avg;
  if (!scale.readAverage(10, avg)) return false;
  tareRaw = avg;
  return true;
}

void readScale() {
  double avg;
  if (scale.readAverage(3, avg)) {
    massG = (float)((avg - tareRaw) / countsPerG);
    scaleOk = true;
  } else {
    scaleOk = false;
  }
}

void readSensors() {
  liquidProbe.requestTemperatures();
  float t = liquidProbe.getTempCByIndex(0);
  liquidC = (t == DEVICE_DISCONNECTED_C || t < -20.0f || t > 125.0f) ? NAN : t;
  if (bmpOk) chamberKpa = bmp.readPressure() / 1000.0f;
  enclosureC = readEnclosureC();
  tdsPpm = readTdsPpm(liquidC);
  lidClosed    = (digitalRead(PIN_LID) == LOW);        // switch closes to ground when lid is latched
  boilOver     = (digitalRead(PIN_BOILOVER) == LOW);   // sensor pulls low when foam is detected
  waterPresent = (digitalRead(PIN_LEVEL) == LOW);      // float switch closes when tank has water
  readScale();
}

// ------------------------------------------------------------------ NFC
void pollPod() {
  if (state != S_IDLE && state != S_READY) return;
  if (!nfc.PICC_IsNewCardPresent() || !nfc.PICC_ReadCardSerial()) return;

  uint8_t buf[18];
  uint8_t size = sizeof(buf);
  BrewProfile p;
  bool ok = false;
  if (nfc.MIFARE_Read(4, buf, &size) == MFRC522::STATUS_OK) ok = decodeProfile(buf, p);
  nfc.PICC_HaltA();

  profile = ok ? p : BrewProfile();       // default profile if the tag is blank or invalid
  podReady = true;
  Serial.printf("{\"event\":\"pod\",\"from_tag\":%s,\"water_ml\":%u,\"setpoint_c\":%.1f,\"evap_g\":%.0f,\"soak_s\":%u}\n",
                profile.fromTag ? "true" : "false", profile.waterMl, profile.setpointC,
                profile.targetEvapG, profile.presoakS);
  if (state == S_IDLE) setState(S_READY);
}

// ------------------------------------------------------------------ safety
// Stops the cycle (via fault) when a limit is crossed; runs on every sensor refresh.
void safetyCheck() {
  if (state == S_IDLE || state == S_READY || state == S_DONE || state == S_FAULT) return;

  bool heating = (state == S_SOAK || state == S_BOIL);
  if (!lidClosed && state != S_EJECT)                     { fault("lid opened during cycle"); return; }
  if (isnan(liquidC))                                     { fault("liquid temperature sensor invalid"); return; }
  if (liquidC >= LIQUID_TRIP_C)                           { fault("liquid over-temperature"); return; }
  if (!isnan(enclosureC) && enclosureC >= ENCLOSURE_TRIP_C) { fault("enclosure over-temperature"); return; }
  if (!scaleOk)                                           { fault("load cell not responding"); return; }
  if (heating && boilOver)                                { fault("boil-over detected"); return; }
  if (millis() - cycleStart > MAX_BOIL_MS + 10UL * 60UL * 1000UL) { fault("maximum cycle time exceeded"); return; }
}

// ------------------------------------------------------------------ control
void serviceHeater(float setpointC) {
  uint32_t now = millis();
  float dt = (now - lastPid) / 1000.0f;
  if (dt >= 0.5f) {                              // PID update at 2 Hz
    heaterDuty = heaterPid.update(setpointC, liquidC, dt);
    lastPid = now;
  }
  // Time-proportional output across a fixed window (relay-friendly).
  if (now - heaterWindowStart >= HEATER_WINDOW_MS) heaterWindowStart = now;
  bool on = (now - heaterWindowStart) < (uint32_t)(heaterDuty * HEATER_WINDOW_MS);
  if (liquidC >= setpointC + 2.0f) on = false;   // hard software ceiling just above setpoint
  if (on != heaterPinOn) heaterPin(on);
}

void serviceVacuum() {
  if (isnan(chamberKpa)) { relaySet(PCF_R2_VACUUM, false); vacuumPumpOn = false; return; }
  if (!vacuumPumpOn && chamberKpa > VAC_ON_ABOVE_KPA)  { vacuumPumpOn = true;  relaySet(PCF_R2_VACUUM, true); }
  if (vacuumPumpOn  && chamberKpa < VAC_OFF_BELOW_KPA) { vacuumPumpOn = false; relaySet(PCF_R2_VACUUM, false); }
}

void beep(uint8_t count) {
  for (uint8_t i = 0; i < count; i++) { buzzer(true); delay(120); buzzer(false); delay(120); }
}

void beginCycle() {
  if (!lidClosed) { Serial.println("{\"event\":\"refused\",\"reason\":\"lid open\"}"); return; }
  if (!podReady)  { Serial.println("{\"event\":\"refused\",\"reason\":\"no pod read\"}"); return; }
  if (!waterPresent) { Serial.println("{\"event\":\"refused\",\"reason\":\"water tank empty\"}"); return; }
  if (isnan(liquidC) || !scaleOk) { Serial.println("{\"event\":\"refused\",\"reason\":\"sensors not ready\"}"); return; }
  cycleStart = millis();
  evaporatedG = 0.0f;
  relaySet(PCF_R6_LIDLOCK, true);         // lock the lid
  massBase = massG;
  relaySet(PCF_R5_INLET, true);           // open the inlet valve
  setState(S_FILL);
}

void runStateMachine() {
  uint32_t inState = millis() - stateSince;

  switch (state) {
    case S_IDLE:
      break;

    case S_READY:
      if (!podReady) { setState(S_IDLE); break; }
      if (startRequested || (AUTO_START && lidClosed)) { startRequested = false; beginCycle(); }
      break;

    case S_FILL: {
      float added = massG - massBase;
      if (added >= profile.waterMl) {
        relaySet(PCF_R5_INLET, false);
        setState(S_SOAK);
      } else if (inState > MAX_FILL_MS) {
        fault("water fill timeout");
      }
      break;
    }

    case S_SOAK:
      // Gentle warm-up while the herb hydrates: hold well below the boil setpoint.
      serviceHeater(min(profile.setpointC - 20.0f, 60.0f));
      if (inState >= (uint32_t)profile.presoakS * 1000UL) {
        heaterPid.reset();
        massAtBoilStart = massG;
        relaySet(PCF_R3_RECIRC, true);
        relaySet(PCF_R7_EXHAUST, true);
        setState(S_BOIL);
      }
      break;

    case S_BOIL:
      serviceHeater(profile.setpointC);
      serviceVacuum();
      evaporatedG = massAtBoilStart - massG;
      if (evaporatedG >= profile.targetEvapG - EVAP_TOL_G) {
        heaterOff();
        relaySet(PCF_R2_VACUUM, false); vacuumPumpOn = false;
        relaySet(PCF_R3_RECIRC, false);
        relaySet(PCF_R7_EXHAUST, false);
        setState(S_VENT);
      } else if (inState > MAX_BOIL_MS) {
        fault("boil did not reach the target reduction in time");
      }
      break;

    case S_VENT:
      // No vent valve is fitted: the chamber is expected to bleed back to ambient.
      if ((!isnan(chamberKpa) && chamberKpa >= VENT_DONE_KPA) || isnan(chamberKpa)) {
        massBeforeDispense = massG;
        relaySet(PCF_R4_DISPENSE, true);
        setState(S_DISPENSE);
      } else if (inState > MAX_VENT_MS) {
        fault("chamber failed to vent");
      }
      break;

    case S_DISPENSE: {
      float delivered = massBeforeDispense - massG;
      if (delivered >= profile.doseMl) {
        relaySet(PCF_R4_DISPENSE, false);
        if (profile.tdsMinPpm > 0 && tdsPpm < profile.tdsMinPpm)
          Serial.printf("{\"event\":\"warning\",\"reason\":\"TDS below profile minimum\",\"tds_ppm\":%.0f}\n", tdsPpm);
        setState(S_EJECT);
      } else if (inState > MAX_DISPENSE_MS) {
        relaySet(PCF_R4_DISPENSE, false);
        fault("dispense timeout");
      }
      break;
    }

    case S_EJECT:
      relaySet(PCF_R6_LIDLOCK, false);       // unlock
      ejector.write(90);                      // trip the pod catch
      delay(600);
      ejector.write(0);
      podReady = false;
      beep(3);
      setState(S_DONE);
      break;

    case S_DONE:
      if (!lidClosed || inState > 15000UL) setState(S_IDLE);   // ready for the next pod
      break;

    case S_FAULT:
      break;   // outputs already off; wait for 'r'
  }
}

// ------------------------------------------------------------------ UI
void drawDisplay() {
  if (!oledOk) return;
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0, 0);
  oled.printf("iKwath  %s", STATE_NAMES[state]);
  oled.setCursor(0, 12);
  if (isnan(liquidC)) oled.print("Temp  --"); else oled.printf("Temp  %.1f C", liquidC);
  oled.setCursor(0, 22);
  if (isnan(chamberKpa)) oled.print("Press --"); else oled.printf("Press %.1f kPa", chamberKpa);
  oled.setCursor(0, 32);
  oled.printf("Mass  %.0f g", isnan(massG) ? 0.0f : massG);
  oled.setCursor(0, 42);
  oled.printf("Evap  %.0f/%.0f g", evaporatedG, profile.targetEvapG);
  oled.setCursor(0, 54);
  if (state == S_FAULT) oled.print(faultReason.substring(0, 21));
  else oled.printf("%s heater %d%%", podReady ? (profile.fromTag ? "Pod" : "Dflt") : "No pod", (int)(heaterDuty * 100));
  oled.display();
}

void updateLeds() {
  ledSet(LED_RED,   state == S_FAULT);
  ledSet(LED_AMBER, state == S_FILL || state == S_SOAK || state == S_BOIL || state == S_VENT);
  ledSet(LED_GREEN, state == S_READY || state == S_DONE);
  ledSet(LED_BLUE,  state == S_DISPENSE || state == S_EJECT);
  ledsShow();
}

void printTelemetry() {
  Serial.printf("{\"state\":\"%s\",\"liquid_c\":%.2f,\"chamber_kpa\":%.1f,\"mass_g\":%.1f,\"evap_g\":%.1f,"
                "\"target_evap_g\":%.0f,\"tds_ppm\":%.0f,\"heater_pct\":%d,\"vacuum\":%s,\"lid\":%s,\"fault\":\"%s\"}\n",
                STATE_NAMES[state], isnan(liquidC) ? -1.0f : liquidC, isnan(chamberKpa) ? -1.0f : chamberKpa,
                isnan(massG) ? 0.0f : massG, evaporatedG, profile.targetEvapG, tdsPpm,
                (int)(heaterDuty * 100), vacuumPumpOn ? "true" : "false", lidClosed ? "true" : "false",
                faultReason.c_str());
}

void abortCycle() {
  allOutputsOff();
  faultReason = "";
  podReady = false;
  setState(S_IDLE);
}

void handleCommand(String line) {
  line.trim();
  if (line.length() == 0) return;
  if (line == "s") { startRequested = true; if (state == S_READY) { startRequested = false; beginCycle(); } }
  else if (line == "x") { abortCycle(); }
  else if (line == "r") {
    if (state == S_FAULT && !boilOver && (isnan(liquidC) || liquidC < LIQUID_TRIP_C - 3.0f)) {
      faultReason = ""; podReady = false; setState(S_IDLE);
    } else Serial.println("{\"event\":\"refused\",\"reason\":\"fault cause still present\"}");
  }
  else if (line == "t") { Serial.println(tareScale() ? "{\"event\":\"tared\"}" : "{\"event\":\"tare_failed\"}"); }
  else if (line.startsWith("cal ")) {
    float grams = line.substring(4).toFloat();
    double avg;
    if (grams > 0 && scale.readAverage(15, avg) && fabs(avg - tareRaw) > 100) {
      countsPerG = (float)((avg - tareRaw) / grams);
      prefs.putFloat("cpg", countsPerG);
      Serial.printf("{\"event\":\"calibrated\",\"counts_per_g\":%.3f}\n", countsPerG);
    } else Serial.println("{\"event\":\"calibration_failed\"}");
  }
  else Serial.println("commands: s start | x abort | r reset fault | t tare | cal <grams>");
}

void pollSerial() {
  static String buf;
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\n' || c == '\r') { handleCommand(buf); buf = ""; }
    else if (buf.length() < 40) buf += c;
  }
}

#if ENABLE_WIFI
void sendStatus() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  String json = String("{\"state\":\"") + STATE_NAMES[state] + "\",\"liquid_c\":" + String(isnan(liquidC) ? -1 : liquidC, 2) +
                ",\"chamber_kpa\":" + String(isnan(chamberKpa) ? -1 : chamberKpa, 1) +
                ",\"evap_g\":" + String(evaporatedG, 1) + ",\"target_evap_g\":" + String(profile.targetEvapG, 0) +
                ",\"tds_ppm\":" + String(tdsPpm, 0) + ",\"fault\":\"" + faultReason + "\"}";
  server.send(200, "application/json", json);
}
void startWeb() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  server.on("/status", sendStatus);
  server.on("/start", []() { handleCommand("s"); sendStatus(); });
  server.on("/stop",  []() { handleCommand("x"); sendStatus(); });
  server.begin();
}
#endif

// ------------------------------------------------------------------ setup / loop
void setup() {
  Serial.begin(SERIAL_BAUD);

  // Make every output safe before anything else initialises.
  pinMode(PIN_HEATER, OUTPUT);
  heaterPin(false);
  pinMode(PIN_LID, INPUT);         // external 10k pull-ups on the board
  pinMode(PIN_BOILOVER, INPUT);
  pinMode(PIN_LEVEL, INPUT);
  analogReadResolution(12);

  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
  pcfRelays = RELAY_ACTIVE_LOW ? 0xFF : 0x00;
  pcfRelays &= ~(1 << PCF_BUZZER);
  pcfWrite(ADDR_PCF_RELAYS, pcfRelays);
  pcfLeds = LED_ACTIVE_LOW ? 0xFF : 0x00;
  pcfWrite(ADDR_PCF_LEDS, pcfLeds);

  oledOk = oled.begin(SSD1306_SWITCHCAPVCC, ADDR_OLED);
  bmpOk  = bmp.begin(ADDR_BMP280);
  if (bmpOk) bmp.setSampling(Adafruit_BMP280::MODE_NORMAL, Adafruit_BMP280::SAMPLING_X1,
                             Adafruit_BMP280::SAMPLING_X4, Adafruit_BMP280::FILTER_X4, Adafruit_BMP280::STANDBY_MS_63);

  liquidProbe.begin();
  liquidProbe.setWaitForConversion(true);

  SPI.begin(PIN_NFC_SCK, PIN_NFC_MISO, PIN_NFC_MOSI, PIN_NFC_SS);
  nfc.PCD_Init();

  ejector.setPeriodHertz(50);
  ejector.attach(PIN_SERVO, 500, 2400);
  ejector.write(0);

  scale.begin();
  prefs.begin("ikwath", false);
  countsPerG = prefs.getFloat("cpg", HX711_COUNTS_PER_G);
  tareScale();

#if ENABLE_WIFI
  startWeb();
#endif

  Serial.printf("{\"event\":\"boot\",\"oled\":%s,\"bmp280\":%s,\"counts_per_g\":%.2f}\n",
                oledOk ? "true" : "false", bmpOk ? "true" : "false", countsPerG);
  setState(S_IDLE);
}

void loop() {
  uint32_t now = millis();

  pollSerial();
  pollPod();

  if (now - lastSensor >= SENSOR_MS) {
    lastSensor = now;
    readSensors();
    safetyCheck();
  }

  runStateMachine();

  if (now - lastDisplay >= DISPLAY_MS) { lastDisplay = now; drawDisplay(); updateLeds(); }
  if (now - lastTelemetry >= TELEMETRY_MS) { lastTelemetry = now; printTelemetry(); }

#if ENABLE_WIFI
  server.handleClient();
#endif
}
