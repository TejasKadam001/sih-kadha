#pragma once
#include <Arduino.h>
#include "config.h"

// Brew profile carried by the pod's NTAG213 tag (user memory starting at page 4, 16 bytes):
//   [0]='I' [1]='K'   magic
//   [2]      format version (1)
//   [3]      water ratio class: 16 soft, 8 medium, 4 hard
//   [4..5]   pre-soak seconds (little endian)
//   [6..7]   target evaporated mass in grams (little endian)
//   [8]      temperature setpoint in C
//   [9]      minimum TDS in units of 10 ppm (0 = ignore)
//   [10..11] water volume in mL (little endian)
//   [12]     dose volume in mL
//   [13..14] reserved
//   [15]     XOR of bytes 0..14
struct BrewProfile {
  uint8_t  ratioClass   = 8;
  uint16_t presoakS     = DEF_PRESOAK_S;
  float    targetEvapG  = DEF_TARGET_EVAP_G;
  float    setpointC    = DEF_SETPOINT_C;
  uint16_t tdsMinPpm    = DEF_TDS_MIN_PPM;
  uint16_t waterMl      = DEF_WATER_ML;
  uint16_t doseMl       = DEF_DOSE_ML;
  bool     fromTag      = false;
};

inline bool decodeProfile(const uint8_t *b, BrewProfile &p) {
  if (b[0] != 'I' || b[1] != 'K' || b[2] != 1) return false;
  uint8_t x = 0;
  for (int i = 0; i < 15; i++) x ^= b[i];
  if (x != b[15]) return false;

  uint16_t soak  = b[4] | (b[5] << 8);
  uint16_t evap  = b[6] | (b[7] << 8);
  uint16_t water = b[10] | (b[11] << 8);
  float setpoint = b[8];

  // Reject values outside the design envelope instead of trusting the tag.
  if (setpoint < BAND_LOW_C || setpoint > BAND_HIGH_C - 1.0f) return false;
  if (water < 100 || water > 600 || evap == 0 || evap >= water) return false;
  if (soak > 900) return false;

  p.ratioClass  = b[3];
  p.presoakS    = soak;
  p.targetEvapG = evap;
  p.setpointC   = setpoint;
  p.tdsMinPpm   = (uint16_t)b[9] * 10;
  p.waterMl     = water;
  p.doseMl      = b[12] ? b[12] : DEF_DOSE_ML;
  p.fromTag     = true;
  return true;
}
