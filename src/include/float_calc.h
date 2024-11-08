#pragma once

/* clang-format off */

#include <cmath>
#include <float.h>

namespace /* Defines. */ {
  #define __float \
    __inline__ constexpr float __attribute((__always_inline__, __nodebug__, __nothrow__, __warn_unused_result__, __const__))

  #define __double \
    __inline__ constexpr double __attribute((__always_inline__, __nodebug__, __nothrow__, __warn_unused_result__, __const__))

  #define Max(x, max) ((x > max) ? (x = max) : 0)
  #define Min(x, min) ((x < min) ? (x = min) : 0)
  #define Set_Max(x, new_x, max) (x = (new_x > max) ? max : new_x)
  #define Set_Min(x, new_x, min) (x = (new_x < min) ? min : new_x)
  #define Clamp(x, min, max) ((x < min) ? (x = min) : (x > max) ? (x = max) : 0)
  #define Clamp_Ret(x, min, max) (x = (x < min) ? min : (x > max) ? max : x)
  
  #define MM_TO_M(x)        ((x) / 1000)
  #define GRAIN_TO_GRAM(x)  ((x) * 0.0648f)

  #define SMOKELESS_GUNPOWDER_ENERGY_PER_GRAM 4000.0f

  #define PIXELS_PER_METER  (100.00f)
  #define PIXEL_TO_M(pixel) (pixel / PIXELS_PER_METER)
  #define M_TO_PIXEL(m)     ((m) * PIXELS_PER_METER)
  #define GRAVITY           (9.806650000000001)
}

__float squaref(float x) {
  return (x * x);
}

__float magnitudef(float x, float y) {
  return sqrt(squaref(x) + squaref(y));
}

__float dotf(float x1, float y1, float x2, float y2) {
  return ((x1 * x2) + (y1 * y2));
}

__float __accelerate(float v, float f, float t) {
  return (v + (f * t));
}

__float __acceleration_force(float f, float t) {
  return (f * t);
}

__float __calc_position(float p, float v, float t) {
  return (p + (v * t));
}

__float radiansf(float angle) {
  return (angle * (M_PI / 180.0f));
}

__float degreesf(float radiant) {
  return (radiant * (180.0f / M_PI));
}

__float anglef(float x1, float y1, float x2, float y2) {
  return degreesf(atan2((y2 - y1), (x2 - x1)));
}

__float lerpf(float a, float b, float t) {
  return ((a + t) * (b - a));
}

__float crossf(float x1, float y1, float x2, float y2) {
  return ((x1 * y2) - (y1 * x2));
}

__float clampf(float x, float min, float max) {
  return ((x < min) ? min : (x > max) ? max : x);
}

__float minf(float x, float min) {
  return ((x < min) ? min : x);
}

__float maxf(float x, float max) {
  return ((x > max) ? max : x);
}

__float radiusf(float diameter_m) {
  return (diameter_m / 2.0f);
}

__float cylinder_volume(float diameter_m, float length_m) {
  return (M_PI * pow(radiusf(diameter_m), 2) * length_m);
}

__float barrel_mass(float length_m, float diameter_m, float round_diameter_m, float density) {
  float volume = cylinder_volume(diameter_m, length_m);
  float negative_volume = cylinder_volume(round_diameter_m, length_m);
  float barrel_volume = (volume - negative_volume);
  return (barrel_volume * density);
}

__float heat_capacity(float mass, float material_heat_capacity) {
  return (mass * material_heat_capacity);
}

__float smokeless_gunpowder_energy(float grains) {
  return (GRAIN_TO_GRAM(grains) * SMOKELESS_GUNPOWDER_ENERGY_PER_GRAM);
}

__float single_shot_temp_increace(float energy, float heat_fraction, float heat_capacity) {
  return ((energy * heat_fraction) / heat_capacity);
}

__float kinematic_position_change(float x0, float v0, float a, float t) {
  return (x0 + x0 * t + 0.5f * a * t * t);
}