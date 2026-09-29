#pragma once
// Tunable settings. Values marked CALIBRATE must be measured on the real build.

// ---- Behaviour switches ----
#define AUTO_START           0     // 1: start as soon as a valid pod is read and the lid is latched
#define ENABLE_WIFI          0     // 1: serve /status /start /stop over HTTP (fill in credentials)
#define RELAY_ACTIVE_LOW     1     // most opto relay modules switch on a LOW input
#define LED_ACTIVE_LOW       0     // LEDs on the expander: 0 = high turns on
#define SERIAL_BAUD          115200

// ---- WiFi (only used when ENABLE_WIFI = 1). Never commit real credentials. ----
#define WIFI_SSID            "your_wifi_name"
#define WIFI_PASSWORD        "your_wifi_password"

// ---- Default brew profile (used when the pod tag has no valid payload) ----
#define DEF_WATER_ML         400
#define DEF_PRESOAK_S        180
#define DEF_TARGET_EVAP_G    300.0f     // 4:1 reduction of 400 g
#define DEF_SETPOINT_C       86.0f
#define DEF_DOSE_ML          100
#define DEF_TDS_MIN_PPM      0          // 0 disables the concentration check

// ---- Process limits ----
#define BAND_LOW_C           85.0f
#define BAND_HIGH_C          90.0f
#define VAC_ON_ABOVE_KPA     58.0f      // pump on above this absolute pressure
#define VAC_OFF_BELOW_KPA    52.0f      // pump off below this
#define VENT_DONE_KPA        95.0f      // treat chamber as vented above this
#define EVAP_TOL_G           2.0f
#define HEATER_WINDOW_MS     2000       // time-proportional window (protects the relay)

// ---- Safety trips (firmware layer; hardware cutoffs are separate) ----
#define LIQUID_TRIP_C        92.0f
#define ENCLOSURE_TRIP_C     60.0f
#define MAX_FILL_MS          90000UL
#define MAX_BOIL_MS          (25UL * 60UL * 1000UL)
#define MAX_VENT_MS          30000UL
#define MAX_DISPENSE_MS      45000UL

// ---- PID for the heater (starting values, tune on the rig) ----
#define PID_KP               0.35f
#define PID_KI               0.02f
#define PID_KD               0.6f

// ---- Load cell ----
// CALIBRATE: counts per gram. The 'cal <grams>' serial command measures and stores it.
#define HX711_COUNTS_PER_G   420.0f

// ---- NTC (10k, beta 3950, 10k to 3V3, NTC to ground) ----
#define NTC_R_FIXED          10000.0f
#define NTC_R25              10000.0f
#define NTC_BETA             3950.0f

// ---- Timing ----
#define TELEMETRY_MS         1000
#define DISPLAY_MS           250
#define SENSOR_MS            250
