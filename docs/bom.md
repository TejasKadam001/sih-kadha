# Bill of materials

Source: `blehpcb/BOM.csv` (46 numbered line items). `blehpcb/` is gitignored, so this file is the copy that reaches the remote.

| # | Category | Component | Ref | Value / description | Qty | Cost (INR) |
|---|---|---|---|---|---|---|
| 1 | Microcontroller | ESP32 DevKit V1 (38-pin) | U1 | Dual-core 240MHz, WiFi + BLE, 38-Pin Module | 1 | 350 |
| 2 | Sensors | DS18B20 Temp Probe | J1 | Waterproof stainless digital temperature sensor (1-Wire) | 1 | 110 |
| 3 | Sensors | HX711 ADC Module | U2 | 24-bit ADC load cell amplifier module | 1 | 90 |
| 4 | Sensors | 1kg Load Cell | J2 | Straight-bar strain gauge load cell (E+, E-, A+, A-) | 1 | 180 |
| 5 | Sensors | MFRC522 RFID/NFC Reader | U3 | 13.56MHz SPI RFID reader module for pod formulation ID | 1 | 140 |
| 6 | Sensors | BMP280 Barometric Sensor | U4 | Digital pressure & ambient temperature sensor (I2C) | 1 | 120 |
| 7 | Sensors | Analog TDS Sensor Module | J3 | Water quality / total dissolved solids concentration sensor | 1 | 190 |
| 8 | Sensors | 10k NTC Thermistor | J4 | 10k ohm NTC temperature sensor probe (enclosure monitoring) | 1 | 15 |
| 9 | Display & UI | SSD1306 OLED Display | U5 | 128x64 monochrome graphic display (0.96 inch, I2C) | 1 | 180 |
| 10 | Display & UI | 5mm LED Red | D1 | Red status LED (Heating active) | 1 | 2 |
| 11 | Display & UI | 5mm LED Amber | D2 | Amber status LED (Soak / Reduction phase) | 1 | 2 |
| 12 | Display & UI | 5mm LED Green | D3 | Green status LED (Decoction Ready / Safe) | 1 | 2 |
| 13 | Display & UI | 5mm LED Blue | D4 | Blue status LED (Water fill / Dispense / BLE) | 1 | 2 |
| 14 | Display & UI | Passive Piezo Buzzer | BZ1 | Audible buzzer for cycle start, completion and error alarms | 1 | 18 |
| 15 | Input Switches | Lid Interlock Microswitch | J5 | Lever micro limit switch for lid position sense | 1 | 20 |
| 16 | Input Switches | Boil-Over Detect Switch | J6 | Chamber rim float / microswitch for anti-boilover safety | 1 | 20 |
| 17 | Input Switches | Water Fill-Level Switch | J7 | Water reservoir / chamber level float limit switch | 1 | 25 |
| 18 | GPIO Expander | PCF8574 I2C Expander #1 | U6 | 8-bit I2C GPIO expander module (Address 0x20, Relays 2-7 + Buzzer) | 1 | 75 |
| 19 | GPIO Expander | PCF8574 I2C Expander #2 | U7 | 8-bit I2C GPIO expander module (Address 0x21, Status LEDs) | 1 | 75 |
| 20 | Actuators | 12V Silicone Heater Pad | TB3 | 12V flexible food-safe silicone heating element (30-50W) | 1 | 350 |
| 21 | Actuators | 12V Mini Vacuum Pump | TB4 | 12V DC mini diaphragm vacuum pump for reduced-pressure boil | 1 | 420 |
| 22 | Actuators | 12V Recirculation Pump | TB5 | 12V DC mini diaphragm/peristaltic liquid pump | 1 | 240 |
| 23 | Actuators | 12V Dispense Pump | TB6 | 12V DC mini food-grade liquid transfer pump | 1 | 240 |
| 24 | Actuators | 12V Inlet Solenoid Valve | TB7 | 12V DC normally-closed water inlet valve | 1 | 180 |
| 25 | Actuators | 12V Solenoid Lid Lock | TB8 | 12V DC electronic solenoid catch lock mechanism | 1 | 190 |
| 26 | Actuators | 12V Brushless Fan 40mm | TB9 | 12V DC 40mm brushless cooling / steam evacuation fan | 1 | 95 |
| 27 | Actuators | SG90 Micro Servo 9g | J8 | 5V 9g micro servo motor for herbal pod ejection mechanism | 1 | 110 |
| 28 | Actuator Drivers | 5V 1-Ch Relay Module | K1..K7 | 5V opto-isolated single-channel relay modules (7 modules total) | 7 | 350 |
| 29 | Power Supply | 220V AC to 12V 5A Supply | PS1 | Enclosed switching power supply module (12V DC 5A, 60W) | 1 | 380 |
| 30 | Power Supply | LM2596 DC-DC Buck Module | U8 | Adjustable step-down regulator module, calibrated to 5.0V output | 1 | 65 |
| 31 | Safety & Protection | 5A Inline / PCB Fuse | F1 | 5x20mm fast-acting glass cartridge fuse with holder | 1 | 20 |
| 32 | Safety & Protection | Bimetallic Thermal Fuse | F2 | 95-100°C normally-closed thermal cutoff (hardware cutoff) | 1 | 40 |
| 33 | Safety & Protection | Lid Interlock Contact Block | TB10 | Hardware series interlock cutout terminal block | 1 | 15 |
| 34 | Passive Components | Resistor 4.7k ohm 1/4W | R1, R6, R7 | 1-Wire & I2C SDA/SCL pull-up resistors | 3 | 3 |
| 35 | Passive Components | Resistor 10k ohm 1/4W | R2, R3, R4, R5 | NTC divider & limit switch pull-up resistors | 4 | 4 |
| 36 | Passive Components | Resistor 330 ohm 1/4W | R8, R9, R10, R11 | Status LED current limiting resistors | 4 | 4 |
| 37 | Passive Components | Ceramic Capacitor 100nF | C1..C10 | Decoupling (ESP32, HX711, RFID, BMP, TDS, Expanders) & Debounce | 10 | 15 |
| 38 | Passive Components | Electrolytic Cap 100uF | C12, C13 | Buck converter input (25V) and output (10V) filtering | 2 | 8 |
| 39 | Passive Components | Electrolytic Cap 470uF 16V | C11 | Bulk decoupling on main 12V high-current rail | 1 | 10 |
| 40 | Connectors & Headers | JST-XH 2-Pin Connector | J4, J5, J6, J7 | 2.50mm vertical JST-XH headers for switches and NTC | 4 | 16 |
| 41 | Connectors & Headers | JST-XH 3-Pin Connector | J1, J3, J8 | 2.50mm vertical JST-XH headers for DS18B20, TDS, Servo | 3 | 15 |
| 42 | Connectors & Headers | JST-XH 4-Pin Connector | J2 | 2.50mm vertical JST-XH header for Load Cell | 1 | 6 |
| 43 | Connectors & Headers | 10-Pin Inter-Board Header | J9, J_PWR_LINK | 2.54mm pin header for inter-board 5V/3V3/GND/PID link | 2 | 20 |
| 44 | Connectors & Headers | 6-Pin Relay Signal Header | J10, J_RELAY_CTRL | 2.54mm pin header for 6x relay control bus | 2 | 16 |
| 45 | Connectors & Headers | Screw Terminal Block 2P | TB1..TB10 | 5.08mm 2-pin screw terminal blocks for 12V power & actuators | 10 | 120 |
| 46 | Hardware | M3 Mounting Hardware | H1..H4 (x2 boards) | M3 PCB standoffs and stainless screws | 8 | 40 |

Sum of the cost column: **INR 4,588** (about US$55).
Sum of quantity x cost: INR 8,335.

## Reading the total

The CSV ends with its own row, "TOTAL ESTIMATED BOM COST (INR): 4588", which equals the plain sum of the cost column. That matches reading each figure as the cost of the whole line (item 46, eight M3 fasteners for INR 40, is consistent with that). If the figures were unit prices the bill would be INR 8,335 instead. Spot-check two or three lines against a supplier listing before quoting a price outside the team.

## Notes

* The README's earlier table used slightly different prices (for example ESP32 at INR 420 versus 350 here); the CSV is the more detailed source.
* Not included: enclosure, SS304 boiling chamber, load-cell mounting, tubing, fittings, pod tooling, and the PCB fabrication cost.
* Sourcing categories in the CSV point to hobby-electronics suppliers; food-contact parts (pumps, tubing, chamber) need food-grade certification before any real use.
