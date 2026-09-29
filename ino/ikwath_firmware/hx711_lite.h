#pragma once
#include <Arduino.h>

// Minimal 24-bit HX711 reader (channel A, gain 128). No external library.
class Hx711Lite {
 public:
  Hx711Lite(uint8_t dt, uint8_t sck) : _dt(dt), _sck(sck) {}

  void begin() {
    pinMode(_dt, INPUT);
    pinMode(_sck, OUTPUT);
    digitalWrite(_sck, LOW);
  }

  bool ready() const { return digitalRead(_dt) == LOW; }

  // Blocking read with timeout. Returns false if the chip never became ready.
  bool readRaw(long &out, uint32_t timeoutMs = 200) {
    uint32_t start = millis();
    while (!ready()) {
      if (millis() - start > timeoutMs) return false;
      delay(1);
    }
    uint32_t value = 0;
    noInterrupts();
    for (uint8_t i = 0; i < 24; i++) {
      digitalWrite(_sck, HIGH);
      delayMicroseconds(1);
      value = (value << 1) | (uint32_t)digitalRead(_dt);
      digitalWrite(_sck, LOW);
      delayMicroseconds(1);
    }
    digitalWrite(_sck, HIGH);   // 25th pulse selects gain 128 for the next reading
    delayMicroseconds(1);
    digitalWrite(_sck, LOW);
    interrupts();
    if (value & 0x800000UL) value |= 0xFF000000UL;   // sign-extend 24 -> 32 bit
    out = (long)value;
    return true;
  }

  // Average of n readings; false if any read failed.
  bool readAverage(uint8_t n, double &out) {
    double sum = 0.0;
    for (uint8_t i = 0; i < n; i++) {
      long v;
      if (!readRaw(v)) return false;
      sum += v;
    }
    out = sum / n;
    return true;
  }

 private:
  uint8_t _dt, _sck;
};
