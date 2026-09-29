# ino: ESP32 firmware

Arduino sketch for the iKwath control board. It implements the brew cycle from [docs/control-sequence.md](../docs/control-sequence.md) on the pin map in [docs/pinout-and-buses.md](../docs/pinout-and-buses.md).

**Status:** written from the specification. It has not been compiled on this project's toolchain and has not been run on the appliance. Treat it as a first draft for bench bring-up, and measure every `CALIBRATE` value in `config.h` before any powered test.

## Layout

```
ino/
└── ikwath_firmware/
    ├── ikwath_firmware.ino   State machine, sensors, safety checks, serial and optional HTTP interface
    ├── pins.h                GPIO numbers, I2C addresses, expander bit assignments
    ├── config.h              Thresholds, PID gains, calibration, behaviour switches
    ├── pid.h                 PID with output clamp and anti-windup
    ├── hx711_lite.h          Small HX711 reader (no external library)
    └── profile.h             Pod tag payload format and validation
```

## Cycle

```
IDLE -> READY -> FILL -> SOAK -> BOIL -> VENT -> DISPENSE -> EJECT -> DONE
                 any powered state -> FAULT
```

| State | What happens | Exit |
|---|---|---|
| IDLE | Waits for a pod tag | tag read |
| READY | Profile loaded (from tag, else defaults); waits for start | `s` command, or `AUTO_START` |
| FILL | Locks lid, opens inlet valve | mass gain reaches the profile water volume |
| SOAK | Gentle warm-up while the herb hydrates | profile pre-soak time |
| BOIL | Heater PID at the setpoint, vacuum pump with hysteresis, recirculation and exhaust on | evaporated mass reaches the target (within tolerance) |
| VENT | Waits for the chamber to return to ambient (no vent valve is fitted) | pressure back near ambient |
| DISPENSE | Dispense pump | mass drop reaches the dose volume |
| EJECT | Unlocks lid, servo trips the pod catch, beeps | done |
| FAULT | All outputs off | `r` after the cause has cleared |

The endpoint is mass-based: `evaporated = mass at boil start - current mass`.

## Safety checks in software

These add to, and never replace, the hardware layers (lid interlock, bimetallic cutoff, fuse):

* lid opened during a powered state
* liquid probe missing or out of range
* liquid temperature at or above 92 C
* enclosure temperature at or above 60 C
* load cell not responding
* boil-over sensor triggered while heating
* fill, boil, vent, dispense and total-cycle timeouts
* start refused when the lid is open, no pod is read, the tank is empty or sensors are not ready

All outputs are driven to their safe state at the very start of `setup()`, before peripherals initialise.

## Pod tag format

16 bytes at NTAG213 page 4: `'I' 'K'`, version, water-ratio class, pre-soak seconds, target evaporated grams, setpoint, minimum TDS, water mL, dose mL, reserved, XOR checksum. Invalid or out-of-envelope tags fall back to the default profile in `config.h`. Full layout is in `profile.h`.

## Serial interface (115200 baud)

| Command | Action |
|---|---|
| `s` | start the cycle |
| `x` | abort, all outputs off |
| `r` | clear a fault |
| `t` | tare the load cell |
| `cal <grams>` | calibrate with a known mass on the scale; saved to flash |

Telemetry is one JSON line per second (state, temperature, pressure, mass, evaporated grams, TDS, heater percent, flags). With `ENABLE_WIFI 1` the sketch also serves `/status`, `/start` and `/stop`, which is the hook for connecting the web app later.

## Building

Board: ESP32 Dev Module (ESP32 DevKit V1). Libraries (Library Manager): Adafruit SSD1306, Adafruit GFX, Adafruit BMP280, OneWire, DallasTemperature, ESP32Servo, and MFRC522. The HX711 reader is included in the sketch folder, and the PCF8574 expanders are driven directly over I2C.

## Known gaps

* Not yet compiled or run; expect first-build fixes.
* Load-cell readings under vacuum and hose forces need characterising (see docs/limitations.md).
* No vent valve in the current design, so VENT relies on a passive bleed.
* Relay and LED polarity are set by `RELAY_ACTIVE_LOW` and `LED_ACTIVE_LOW`; confirm against the real modules.
* The pod-catch servo angle and the NTC divider orientation are assumptions to verify on the bench.
