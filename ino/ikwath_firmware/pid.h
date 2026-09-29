#pragma once
// Small PID with output clamp and integral anti-windup. Output is a 0..1 duty.

struct Pid {
  float kp, ki, kd;
  float integral = 0.0f, prevError = 0.0f;
  bool  primed = false;

  Pid(float p, float i, float d) : kp(p), ki(i), kd(d) {}

  void reset() { integral = 0.0f; prevError = 0.0f; primed = false; }

  float update(float setpoint, float measured, float dtSeconds) {
    if (dtSeconds <= 0.0f) return 0.0f;
    float error = setpoint - measured;
    float derivative = primed ? (error - prevError) / dtSeconds : 0.0f;
    prevError = error;
    primed = true;

    float candidate = integral + error * dtSeconds;
    float out = kp * error + ki * candidate + kd * derivative;
    if (out > 1.0f)      out = 1.0f;
    else if (out < 0.0f) out = 0.0f;
    else                 integral = candidate;   // integrate only while not saturated
    return out;
  }
};
