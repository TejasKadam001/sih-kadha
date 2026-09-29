# Pinout and buses

Specified for an ESP32 DevKit V1 (38 pin). Source: the hardware specification in the README and `blehread/pcb.md`.

## ESP32 pin map

| Pin | Function | Notes |
|---|---|---|
| GPIO4 | DS18B20 liquid temperature | 1-Wire, 4.7 kOhm pull-up to 3.3 V |
| GPIO16 | HX711 DT | data |
| GPIO17 | HX711 SCK | clock |
| GPIO5 | MFRC522 SS | SPI chip select |
| GPIO18 | MFRC522 SCK | SPI clock |
| GPIO19 | MFRC522 MISO | |
| GPIO23 | MFRC522 MOSI | |
| GPIO27 | MFRC522 RST | reset |
| GPIO21 | I2C SDA | 4.7 kOhm pull-up |
| GPIO22 | I2C SCL | 4.7 kOhm pull-up |
| GPIO34 | Analog TDS sensor | ADC input (input-only pin) |
| GPIO32 | 10 kOhm NTC thermistor | enclosure temperature divider |
| GPIO25 | Lid interlock switch | 10 kOhm pull-up plus 100 nF debounce |
| GPIO26 | Optical boil-over sensor | pull-up plus debounce |
| GPIO33 | Reservoir level switch | pull-up plus debounce |
| GPIO13 | SG90 servo | 50 Hz PWM |
| GPIO14 | Relay 1 (heater) | direct PID drive signal |

## I2C addresses

| Address | Device | Role |
|---|---|---|
| 0x3C | SSD1306 128x64 OLED | display |
| 0x76 | BMP280 | chamber pressure |
| 0x20 | PCF8574 #1 | Relay 2 to Relay 7 triggers and buzzer |
| 0x21 | PCF8574 #2 | status LEDs (red, amber, green, blue) |

## Relay assignment (Power Board)

| Relay | Load |
|---|---|
| 1 | 12 V silicone heater pad (50 W), PID |
| 2 | Vacuum (diaphragm) pump |
| 3 | Recirculation pump |
| 4 | Dispense pump |
| 5 | Water inlet solenoid |
| 6 | Lid lock solenoid |
| 7 | Exhaust fan (40 mm) |

## Inter-board link

The specification uses a 10-pin ribbon carrying power, I2C, the heater PID line and signals. The scripted PCB iterations moved both PCF8574 expanders onto the Control Board and widened the link to 16 pins; see [pcb-status.md](pcb-status.md).

## Notes

* GPIO34 is input-only on the ESP32, which suits an analog sensor.
* The relay stage is opto-isolated, which keeps inductive switching noise off the control ground.
