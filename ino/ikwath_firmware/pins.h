#pragma once
// iKwath control board: ESP32 DevKit V1 (38 pin). Matches docs/pinout-and-buses.md.

// 1-Wire liquid temperature probe (DS18B20, 4.7k pull-up to 3.3V)
#define PIN_DS18B20      4

// HX711 load-cell amplifier
#define PIN_HX711_DT     16
#define PIN_HX711_SCK    17

// MFRC522 NFC reader (SPI)
#define PIN_NFC_SS       5
#define PIN_NFC_SCK      18
#define PIN_NFC_MISO     19
#define PIN_NFC_MOSI     23
#define PIN_NFC_RST      27

// Shared I2C bus (4.7k pull-ups)
#define PIN_I2C_SDA      21
#define PIN_I2C_SCL      22

// Analog inputs
#define PIN_TDS          34   // TDS probe (input-only pin)
#define PIN_NTC          32   // enclosure NTC divider

// Digital safety inputs (10k pull-up + 100nF debounce on the board)
#define PIN_LID          25
#define PIN_BOILOVER     26
#define PIN_LEVEL        33

// Outputs
#define PIN_SERVO        13   // SG90 pod ejector
#define PIN_HEATER       14   // Relay 1: heater, time-proportional PID

// I2C addresses
#define ADDR_OLED        0x3C
#define ADDR_BMP280      0x76
#define ADDR_PCF_RELAYS  0x20 // Relay 2..7 and buzzer
#define ADDR_PCF_LEDS    0x21 // status LEDs

// PCF8574 #1 bit assignment
#define PCF_R2_VACUUM    0
#define PCF_R3_RECIRC    1
#define PCF_R4_DISPENSE  2
#define PCF_R5_INLET     3
#define PCF_R6_LIDLOCK   4
#define PCF_R7_EXHAUST   5
#define PCF_BUZZER       6

// PCF8574 #2 bit assignment
#define LED_RED          0
#define LED_AMBER        1
#define LED_GREEN        2
#define LED_BLUE         3
