#pragma once

/* clang-format off */
#include <SDL2/SDL_rect.h>
#include <cmath>

#include "float_calc.h"

namespace /* MVec2 helper defines. */ {
  /* Some calculation helpers. */
  #define RADIANT_F(angle) \
    (angle * (M_PI / 180.0f))
  /* Some attributes macro helpers. */
  #define __warn __attribute__((__warn_unused_result__))
  /* Internal return macro helpers. */
  #define __MVec2_INLINE__(type) __inline__ constexpr type __attribute__((__always_inline__, __nodebug__, __nothrow__)) 
  #define __MVec2_STATIC__(type) static __inline__ constexpr type __attribute__((__always_inline__, __nodebug__, __nothrow__))
  #define __MVec2_copy        __MVec2_INLINE__(MVec2) __warn
  #define __MVec2_STATIC_copy __MVec2_STATIC__(MVec2) __warn __attribute__((const))
  /* Macros to declare operator return and operator params. */
  #define __const_(type) __inline__ constexpr type __attribute((const, __always_inline__, __nodebug__, __nothrow__))
  #define __operator_double(_op) operator _op(const MVec2 &vec1, const MVec2 &vec2)
  #define __operator_single(_op) operator _op(const MVec2 &vec, float value)
  #define __internal_operator_MVec2(_op) operator _op(const MVec2 &other)
  #define __internal_operator_double(_op) operator _op(float value)
  /* Macros to define internal methods returning a MVec2 ref. */
  #define __MVec2_ref __MVec2_INLINE__(MVec2 &)
  #define __MVec2_ref_operator_(_op) \
    __MVec2_ref __internal_operator_MVec2(_op) { this->x _op other.x; this->y _op other.y; return *this; } \
    __MVec2_ref __internal_operator_double(_op) { this->x _op value;   this->y _op value;   return *this; }
  /* Macros to declare and define MVec2 returning operators. */
  #define __const_MVec2 __const_(MVec2) __warn
  #define __const_MVec2_operator_decl(_op) \
    __const_MVec2  __operator_double(_op); \
    __const_MVec2 __operator_single(_op)
  #define __const_MVec2_operator_def(_op) \
    __const_MVec2 __operator_double(_op) { return {vec1.x _op vec2.x, vec1.y _op vec2.y}; } \
    __const_MVec2 __operator_single(_op) { return {vec.x  _op value,  vec.y  _op value};  }
  /* Macros to declare and define bool returning operators. */
  #define __const_bool __const_(bool) __warn
  #define __const_bool_operator_decl(_op) \
    __const_bool __operator_double(_op); \
    __const_bool __operator_single(_op)
  #define __const_bool_operator_def(_op) \
    __const_bool __operator_double(_op) { return (vec1.x _op vec2.x && vec1.y _op vec2.y); } \
    __const_bool __operator_single(_op) { return (vec.x  _op value  && vec.y  _op value);  }
  #define __MVec2_OPERATOR(type) __inline__ constexpr operator type __attribute__((__const__, __always_inline__, __nothrow__, __nodebug__))
  #define __MVec2 __inline__ constexpr MVec2 __attribute__((__const__, __always_inline__, __nodebug__, __nothrow__, __warn_unused_result__))
  #define __MVec2_void __inline__ constexpr void __attribute__((__always_inline__, __nodebug__, __nothrow__))
  #define ref MVec2 &
  #define cref const ref
}

/* Forward decl. */
typedef struct MVec2 MVec2;

/* Operator decl. */
__const_MVec2_operator_decl(+);
__const_MVec2_operator_decl(-);
__const_MVec2_operator_decl(*);
__const_MVec2_operator_decl(/);
__const_bool_operator_decl(<);
__const_bool_operator_decl(>);
__const_bool_operator_decl(<=);
__const_bool_operator_decl(>=);
__const_bool_operator_decl(==);
__const_bool_operator_decl(!=);

typedef struct MVec2 {
  float x;
  float y;

  __MVec2_ref_operator_(*=);
  __MVec2_ref_operator_(+=);
  __MVec2_ref_operator_(-=);
  __MVec2_ref_operator_(=);

  __MVec2_OPERATOR(SDL_FPoint(void) const) {
    return {x, y};
  }

  __MVec2_ref set(float x, float y) {
    *this = {x, y};
    return *this;
  }

  __MVec2_ref accel(float x, float y, float t) {
    this->x += __acceleration_force(x, t);
    this->y += __acceleration_force(y, t);
    return *this;
  }

  __MVec2_ref accel_x(float x, float t) {
    this->x += __acceleration_force(x, t);
    return *this;
  }

  __MVec2_ref accel_y(float y, float t) {
    this->y += __acceleration_force(y, t);
    return *this;
  }

  __MVec2_ref constrain(bool positive, const MVec2 &vec) {
    if (positive) {
      (x > vec.x) ? (x = vec.x) : 0;
      (y > vec.y) ? (y = vec.y) : 0;
    }
    else {
      (x < vec.x) ? (x = vec.x) : 0;
      (y < vec.y) ? (y = vec.y) : 0;
    }
    return *this;
  }

  __MVec2_ref adjust_angle(float angle) {
    y = (x * tan(radiansf(angle)));
    return *this;
  }

  __MVec2_ref set_magnitude_and_angle(float magnitude, float angle) {
    const float radians = radiansf(angle);
    *this = {(magnitude * cos(radians)), (magnitude * sin(radians))};
    return *this;
  }

  __MVec2_ref lerp(const MVec2 &other, float t) {
    *this = {lerpf(this->x, other.x, t), (lerpf(this->y, other.y, t))};
    return *this;
  }

  __const_(float) magnitude(void) const {
    return magnitudef(x, y);
  }

  __const_(float) dot(const MVec2 &other) const {
    return dotf(x, y, other.x, other.y);
  }

  __const_(float) angle_between(const MVec2 &other) const {
    return degreesf(atan2((other.y - y), (other.x - x)));
  }

  __const_(float) distance_to(const MVec2 &other) const {
    return magnitudef((other.x - x), (other.y - y));
  }

  __const_(float) cross(const MVec2 &other) const {
    return crossf(this->x, this->y, other.x, other.y);
  }

  __const_(float) angle_to(const MVec2 &other) const {
    return degreesf(atan2((other.y - y), (other.x - x)));
  }

  __const_(float) angle_to(float x, float y) const {
    return degreesf(atan2((y - this->y), (x - this->x)));
  }

  __const_(MVec2) direction_to(const MVec2 &other, bool perp = false) const {
    return perp ? MVec2{-(other.y - y), (other.x - x)} : MVec2{(other.x - x), (other.y - y)};
  }

  __const_(MVec2) normalize(void) const {
    return {*this / magnitude()};
  }

  __const_(MVec2) offset_at_angle(float angle, const MVec2 &offset) const {
    const float rad = radiansf(angle);
    const float cosv = cos(rad);
    const float sinv = sin(rad);
    return {
      (x + ((offset.x * cosv) - (offset.y * sinv))),
      (y + ((offset.x * sinv) - (offset.y * cosv)))
    };
  }

  __const_(MVec2) lerp_copy(const MVec2 &other, float t) const {
    return {lerpf(this->x, other.x, t), lerpf(this->y, other.y, t)};
  }

  __const_(MVec2) project_onto(const MVec2 &other) const {
    const float scalar = (this->dot(other) / other.magnitude());
    return {(this->x * scalar), (this->y * scalar)};
  }
  
  __MVec2_STATIC_copy from(const SDL_FPoint &point) {
    return {point.x, point.y};
  }

  __MVec2_STATIC_copy center(const MVec2 &vec1, const MVec2 &vec2) {
    return {(vec1 + vec2) / 2};
  }

  __MVec2_STATIC_copy center(const SDL_FPoint &p1, const SDL_FPoint &p2) {
    return {(from(p1) + from(p2)) / 2};
  }

  __MVec2_STATIC_copy center_with_offset(const MVec2 &p1, const MVec2 &p2, const MVec2 &offset) {
    /* Calculate center point. */
    MVec2 ret = center(p1, p2);
    if (offset.x) {
      const MVec2 direction = (p1.direction_to(p2, true).normalize() *= offset.x);
      ret += direction;
    }
    if (offset.y) {
      const MVec2 direction = (p1.direction_to(p2).normalize() *= offset.y);
      ret += direction; 
    }
    return ret;
  }

  __MVec2_STATIC_copy center_with_offset(const SDL_FPoint &p1, const SDL_FPoint &p2, const MVec2 &offset) {
    MVec2 ret = center(p1, p2);
    const MVec2 vec1 = from(p1);
    const MVec2 vec2 = from(p2);
    if (offset.x) {
      const MVec2 direction = (vec1.direction_to(vec2, true).normalize() *= offset.x);
      ret += direction;
    }
    if (offset.y) {
      const MVec2 direction = (vec1.direction_to(vec2).normalize() *= offset.y);
      ret += direction; 
    }
    return ret;
  }
} MVec2;

/* Operator def. */
__const_MVec2_operator_def(+);
__const_MVec2_operator_def(-);
__const_MVec2_operator_def(*);
__const_MVec2_operator_def(/);
__const_bool_operator_def(<);
__const_bool_operator_def(>);
__const_bool_operator_def(<=)
__const_bool_operator_def(>=)
__const_bool_operator_def(==)
__const_bool_operator_def(!=)

__MVec2 catmull_rom_spline(const MVec2 &p0, const MVec2 &p1, const MVec2 &p2, const MVec2 &p3, float t) {
  const float t2 = (t * t);
  const float t3 = (t * t2);
  return {
    0.5f * ((2 * p1.x) + (-p0.x + p2.x) * t + (2 * p0.x - 5 * p1.x + 4 * p2.x - p3.x) * t2 + (-p0.x + 3 * p1.x - 3 * p2.x + p3.x) * t3),
    0.5f * ((2 * p1.y) + (-p0.y + p2.y) * t + (2 * p0.y - 5 * p1.y + 4 * p2.y - p3.y) * t2 + (-p0.y + 3 * p1.y - 3 * p2.y + p3.y) * t3),
  };
}

__MVec2_void implicit_midpoint_step(MVec2 *pos, MVec2 *vel, float dt, const MVec2 &f) {
  /* Midpoint velocity estimate */
  MVec2 mid_vel = { vel->x + 0.5f * f.x * dt, vel->y + 0.5f * f.y * dt };
  /* Update position using midpoint velocity */
  pos->x += M_TO_PIXEL(mid_vel.x * dt);
  pos->y += M_TO_PIXEL(mid_vel.y * dt); 
  /* Update velocity using the full acceleration */
  vel->x += f.x * dt;
  vel->y += f.y * dt;
}

__MVec2_void beeman_step(MVec2 *pos, MVec2 *vel, float dt, const MVec2 &f, const MVec2 &prev_f) {
  /* Step 1: Position update using current and previous acceleration */
  pos->x += M_TO_PIXEL(vel->x * dt + (2.0 / 3.0) * f.x * dt * dt - (1.0 / 6.0) * prev_f.x * dt * dt);
  pos->y += M_TO_PIXEL(vel->y * dt + (2.0 / 3.0) * f.y * dt * dt - (1.0 / 6.0) * prev_f.y * dt * dt);
  /* Step 2: Final velocity correction */
  vel->x += (1.0 / 3.0) * f.x * dt + (5.0 / 6.0) * prev_f.x * dt;
  vel->y += (1.0 / 3.0) * f.y * dt + (5.0 / 6.0) * prev_f.y * dt;
}

__MVec2_void verlet_step(MVec2 *pos, MVec2 *vel, float dt, const MVec2 &f) {
  /* Step 1: Update position using current velocity and half acceleration */
  pos->x += M_TO_PIXEL(vel->x * dt + 0.5f * f.x * dt * dt);
  pos->y += M_TO_PIXEL(vel->y * dt + 0.5f * f.y * dt * dt);
  /* Step 2: Update velocity using the full acceleration */
  vel->x += (f.x * dt);
  vel->y += (f.y * dt);
}

__MVec2_void velocity_verlet_step(MVec2 *pos, MVec2 *vel, float dt, const MVec2 &f) {
  vel->x += (0.5f * (f.x * dt));
  vel->y += (0.5f * (f.y * dt));
  pos->x += M_TO_PIXEL(vel->x * dt);
  pos->y += M_TO_PIXEL(vel->y * dt);
  vel->x += (0.5f * (f.x * dt));
  vel->y += (0.5f * (f.y * dt));
}

__MVec2_void rk4_step(MVec2 *pos, MVec2 *vel, float dt, const MVec2 &f) {
  /* Calculate intermediate k-values for RK4 integration */
  float k1_x = vel->x * dt;
  float k1_y = vel->y * dt;
  float k1_vx = f.x * dt;
  float k1_vy = f.y * dt;
  float k2_x = (vel->x + 0.5f * k1_vx) * dt;
  float k2_y = (vel->y + 0.5f * k1_vy) * dt;
  float k2_vx = f.x * dt;
  float k2_vy = f.y * dt;
  float k3_x = (vel->x + 0.5f * k2_vx) * dt;
  float k3_y = (vel->y + 0.5f * k2_vy) * dt;
  float k3_vx = f.x * dt;
  float k3_vy = f.y * dt;
  float k4_x = (vel->x + k3_vx) * dt;
  float k4_y = (vel->y + k3_vy) * dt;
  float k4_vx = f.x * dt;
  float k4_vy = f.y * dt;
  /* Update position and velocity */
  pos->x += M_TO_PIXEL((k1_x + 2.0f * k2_x + 2.0f * k3_x + k4_x) / 6.0f);
  pos->y += M_TO_PIXEL((k1_y + 2.0f * k2_y + 2.0f * k3_y + k4_y) / 6.0f);
  vel->x += (k1_vx + 2.0f * k2_vx + 2.0f * k3_vx + k4_vx) / 6.0f;
  vel->y += (k1_vy + 2.0f * k2_vy + 2.0f * k3_vy + k4_vy) / 6.0f;
}

__MVec2_void sympletic_euler_step(MVec2 *pos, MVec2 *vel, float dt, const MVec2 &f) {
  vel->x += (f.x * dt);
  vel->y += (f.y * dt);
  pos->x += M_TO_PIXEL(vel->x * dt);
  pos->y += M_TO_PIXEL(vel->y * dt);
}

static constexpr MVec2 gravity_vector = {0.0f, GRAVITY};

namespace /* Undef defines. */ {
  #undef ref
  #undef cref
}