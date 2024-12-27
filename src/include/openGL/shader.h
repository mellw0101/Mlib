#pragma once
/* clang-format off */
#include "../def.h"
#include "../Vector.h"
#include "../Pair.h"
#include "../Error.h"
#include "../Types.h"
#include "../simd.h"
#include "../Flag.h"
#include "../Debug.h"

#include <string>
#include <fstream>
#include <sstream>
#include <GL/glew.h>

std::string openGL_read_shader_file(const char *path);
Uint openGL_load_shader_file(const char *path, Uint type);
Uint openGL_load_shader_file_raw(const char *str, Uint type);
Uint openGL_create_shader_program(const MVector<Pair<const char *, Uint>> &parts);
Uint openGL_create_shader_program_raw(const MVector<Pair<const char *, Uint>> &parts);

struct ShaderProgram {
 private:
  Uint _id;

 public:
  ShaderProgram(const char *vertex_shader_path, const char *fragment_shader_path) {
    _id = openGL_create_shader_program({
      {vertex_shader_path,   GL_VERTEX_SHADER},
      {fragment_shader_path, GL_FRAGMENT_SHADER}
    });
  }

  ~ShaderProgram(void) {
    glDeleteProgram(_id);
  }

  int get_uniform_location(const char *const &name) {
    int ret = glGetUniformLocation(_id, name);
    if (ret == -1) {
      logE("Failed to get uniform location: '%s'\n");
    }
    return ret;
  }

  GLuint id(void) {
    return _id;
  }
};

inline namespace Vec {
  namespace /* Defines */ {
    #define __constructor(...) __inline__ constexpr __attribute__((__always_inline__, __nodebug__, __nothrow__)) vec(__VA_ARGS__)
    #define __ref              __inline__ constexpr vec & __attribute((__always_inline__, __nothrow__, __nodebug__))
    #define __copy             __inline__ constexpr vec __attribute((__always_inline__, __nothrow__, __nodebug__, __const__))
    #define __bool             __inline__ constexpr bool __attribute((__always_inline__, __nothrow__, __nodebug__, __const__))
    #define __copy_(s, t)   __inline__ constexpr vec<s, t> __attribute((__always_inline__, __nothrow__, __nodebug__, __const__))
    #define __type_ref         __inline__ constexpr T &__warn_unused __attribute((__always_inline__, __nothrow__, __nodebug__))
    #define __type_T           __inline__ constexpr T __warn_unused __attribute((__always_inline__, __nothrow__, __nodebug__))
    
    namespace /* Internal data */ {
      #define VEC_STRUCT_2  \
        struct { T x, y; }; \
        struct { T w, h; }; \
        T data[2];

      #define VEC_STRUCT_3     \
        struct { T x, y, z; }; \
        T data[3];

      #define VEC_STRUCT_4                                \
        struct { T x, y, z, w; };                         \
        struct { T left, right, top, bottom; };           \
        struct { T s, t, p, q; }; /* Texture cordinats */ \
        T data[4];

      #define VEC_DATA(size) \
        union {              \
          VEC_STRUCT_##size  \
        }
    }
    namespace /* Operator */ {
      #define VEC_IDX_OPERATOR \
        __type_ref       operator[](Uint idx)       { return data[idx]; } \
        const __type_ref operator[](Uint idx) const { return data[idx]; }

      #define VEC_OPERATOR(size, op)             \
        __copy operator op(const vec &v) const { \
          vec ret;                               \
          for (Uint i = 0; i < size; ++i) {      \
            ret[i] = (data[i] op v[i]);          \
          }                                      \
          return ret;                            \
        }                                        \
        __copy operator op(T value) const {      \
          vec ret;                               \
          for (Uint i = 0; i < size; ++i) {      \
            ret[i] = (data[i] op value);         \
          }                                      \
          return ret;                            \
        }                                        \
        __ref operator op##=(const vec &v) {     \
          for (Uint i = 0; i < size; ++i) {      \
            data[i] op##= v[i];                  \
          }                                      \
          return *this;                          \
        }                                        \
        __ref operator op##=(T value) {          \
          for (Uint i = 0; i < size; ++i) {      \
            data[i] op##= value;                 \
          }                                      \
          return *this;                          \
        }

        #define OP__add +
        #define OP__sub -
        #define OP__mul *
        #define OP__div /

        #define OP_add operator +
        #define OP_sub operator -
        #define OP_mul operator *
        #define OP_div operator /
        #define OP_eq_add operator +=
        #define OP_eq_sub operator -=
        #define OP_eq_mul operator *=
        #define OP_eq_div operator /=

        #define OP(op) OP_##op
        #define OP_(op) OP__##op
        #define OP_eq(op) OP_eq_##op

        #define VEC4_H_OPERATOR(op)                           \
          __copy OP(op)(const vec &other) const {          \
            __avx combined(data[0], other.data[0], data[1], other.data[1], data[2], other.data[2], data[3], other.data[3]); \
            __avx result = combined.h##op(combined); \
            vec ret(result[0], result[1], result[4], result[5]); \
            return ret;                                   \
          }                                                 \
          __copy OP(op)(T value) const {          \
            __avx combined(data[0], value, data[1], value, data[2], value, data[3], value); \
            __avx result = combined.h##op(combined); \
            vec ret(result[0], result[1], result[4], result[5]); \
            return ret;                                   \
          }                         \
          __ref OP_eq(op)(const vec &other) {              \
            *this = (*this OP_(op) other); \
            return *this;                                 \
          }                                                 \
          __ref OP_eq(op)(T value) {                   \
           *this = (*this OP_(op) vec(value)); \
            return *this;                                 \
          }

        #define VEC4_OPERATOR(op)                           \
          __copy OP(op)(const vec &v) const {          \
            vec ret;                                      \
            __m128 a = _mm_load_ps(data);                \
            __m128 b = _mm_load_ps(v.data);              \
            __m128 res = _mm_##op##_ps(a, b);         \
            _mm_store_ps(ret.data, res);                 \
            return ret;                                   \
          }                                                 \
          __copy OP(op)(T value) const {               \
            vec ret;                                      \
            __m128 a = _mm_load_ps(data);                \
            __m128 b = _mm_set1_ps(value);                \
            __m128 res = _mm_##op##_ps(a, b);             \
            _mm_store_ps(ret.data, res);                 \
            return ret;                                   \
          }                            \
          __ref OP_eq(op)(const vec &v) {              \
            __m128 a = _mm_load_ps(data);                \
            __m128 b = _mm_load_ps(v.data);              \
            __m128 res = _mm_##op##_ps(a, b);             \
            _mm_store_ps(data, res);                     \
            return *this;                                 \
          }                                                 \
          __ref OP_eq(op)(T value) {                   \
            __m128 a = _mm_load_ps(data);                \
            __m128 b = _mm_set1_ps(value);                \
            __m128 res = _mm_##op##_ps(a, b);             \
            _mm_store_ps(data, res);                     \
            return *this;                                 \
          }
    }
    namespace /* Swizzling */ {
      #define VEC_SWIZZLING_2                       \
        __copy xx(void) const { return vec(x, x); } \
        __copy yy(void) const { return vec(y, y); } \
        __copy xy(void) const { return vec(x, y); } \
        __copy yx(void) const { return vec(y, x); }
      
      #define VEC_SWIZZLING_3                                    \
        __copy_(2, T) xx(void)  const { return vec<2, T>(x, x); } \
        __copy_(2, T) yy(void)  const { return vec<2, T>(y, y); } \
        __copy_(2, T) xy(void)  const { return vec<2, T>(x, y); } \
        __copy_(2, T) yx(void)  const { return vec<2, T>(y, x); } \
        __copy_(3, T) xxx(void) const { return vec<3, T>(x, x, x); } \
        __copy_(3, T) yyy(void) const { return vec<3, T>(y, y, y); } \
        __copy_(3, T) zzz(void) const { return vec<3, T>(z, z, z); } \
        __copy_(3, T) yxx(void) const { return vec<3, T>(y, x, x); } \
        __copy_(3, T) xyx(void) const { return vec<3, T>(x, y, x); } \
        __copy_(3, T) xxy(void) const { return vec<3, T>(x, x, y); } \
        __copy_(3, T) xyy(void) const { return vec<3, T>(x, y, y); } \
        __copy_(3, T) yxy(void) const { return vec<3, T>(y, x, y); } \
        __copy_(3, T) yyx(void) const { return vec<3, T>(y, y, x); }

      #define VEC_SWIZZLING(size) VEC_SWIZZLING_##size
    }
    namespace /* Types */ {
      #define bvec2 vec<2, bool, 2>
      #define bvec3 vec<3, bool, 4>
      #define bvec4 vec<4, bool, 4>
      #define ivec2 vec<2, int, 8>
      #define ivec3 vec<3, int, 16>
      #define ivec4 vec<4, int, 16>
      #define uvec2 vec<2, Uint, 8>
      #define uvec3 vec<3, Uint, 16>
      #define uvec4 vec<4, Uint, 16>
      #define vec2 vec<2, float, 8>
      #define vec3 vec<3, float, 16>
      #define vec4 vec<4, float, 16>
      #define dvec2 vec<2, double, 16>
      #define dvec3 vec<3, double, 32>
      #define dvec4 vec<4, double, 32>
    }
  }

  template <Uint Size, typename T, Uint Alignment = alignof(T)> struct vec;
  
  template <typename T, Uint Alignment>
  struct vec<2, T, Alignment> {
    VEC_DATA(2);
    __constructor(void) : data{ T{}, T{} } {}
    __constructor(T v) : data{ v, v } {}
    __constructor(T x, T y) : data{ x, y } {}
    __constructor(const vec &other) : data{ other.x, other.y } {}
    template <Uint Size, typename U, Uint A>
    __constructor(const vec<Size, U, A> &other) {
      for (Uint i = 0; i < 2; ++i) {
        data[i] = (T)other[i];
      }
    }
    __bool operator==(const vec &other) const {
      return ((x == other.x) && (y == other.y));
    }
    VEC_SWIZZLING(2)
    VEC_IDX_OPERATOR
    VEC_OPERATOR(2, *);
    VEC_OPERATOR(2, +);
    VEC_OPERATOR(2, -);
    VEC_OPERATOR(2, /);
  } __align_size(Alignment);

  template <typename T, Uint Alignment>
  struct vec<3, T, Alignment> {
    VEC_DATA(3);
    __constructor(void) : data{ T{}, T{}, T{} } {}
    __constructor(T v) : data{ v, v, v } {}
    __constructor(T x, T y, T z) : data{ x, y, z } {}
    __constructor(const vec &other) : data{ other.x, other.y, other.z } {}
    template <Uint Size, typename U, Uint A, typename... Args>
    __constructor(const vec<Size, U, A> &other, Args &&...args) {
      static_assert(sizeof...(Args) + Size <= 3, "To meny elements.");
      for (Uint i = 0; i < Size && i < 3; ++i) {
        data[i] = (T)other[i];
      }
      if constexpr (Size < 3) {
        if constexpr (sizeof...(Args) > 0) {
          float tmp[sizeof...(Args)] = {(T)args...};
          for (Uint i = Size; i < 3; ++i) {
            data = tmp[i - Size];
          }
        }
        else {
          for (Uint i = Size; i < 3; ++i) {
            data[i] = (T)(0);
          }
        }
      }
    }
    __bool operator==(const vec &other) const {
      return ((x == other.x) && (y == other.y) && (z == other.z));
    }
    VEC_SWIZZLING(3)
    VEC_IDX_OPERATOR
    VEC_OPERATOR(3, +)
    VEC_OPERATOR(3, -)
    VEC_OPERATOR(3, *)
    VEC_OPERATOR(3, /)
  } __align_size(Alignment);

  template <typename T, Uint Alignment>
  struct vec<4, T, Alignment> {
    VEC_DATA(4);
    __constructor(void) : data{ T{}, T{}, T{}, T{} } {}
    __constructor(T v) : data{ v, v, v, v } {}
    __constructor(T x, T y, T z, T w) : data{ x, y, z, w } {}
    __constructor(const vec &other) : data{ other.x, other.y, other.z, other.w } {}
    template <Uint Size, typename U, Uint A, typename... Args>
    __constructor(const vec<Size, U, A> &other, Args &&...args) {
      static_assert(sizeof...(Args) + Size <= 4, "To meny elements.");
      for (Uint i = 0; i < Size; ++i) {
        data[i] = (T)other[i];
      }
      if constexpr (Size < 4) {
        if constexpr (sizeof...(Args) > 0) {
          T tmp[sizeof...(Args)] = {(T)(args)...};
          for (Uint i = Size; i < 4; ++i) {
            data[i] = tmp[i - Size];
          }
        }
        else {
          for (Uint i = Size; i < 4; ++i) {
            data[i] = (T)0;
          }
        }
      }
    }
    __bool operator==(const vec &other) const {
      return ((x == other.x) && (y == other.y) && (z == other.z) && (w == other.w));
    }
    VEC_IDX_OPERATOR
    VEC4_H_OPERATOR(add);
    VEC4_H_OPERATOR(sub);
    VEC4_H_OPERATOR(mul);
    VEC4_H_OPERATOR(div);
  } __align_size(Alignment);

  namespace /* Undef */ {
    #undef __constructor
    #undef __ref
    #undef __copy
    #undef __bool
    #undef __copy_
    #undef __type_ref

    #undef VEC_STRUCT_2
    #undef VEC_STRUCT_3
    #undef VEC_STRUCT_4
    #undef VEC_DATA
    #undef VEC_IDX_OPERATOR
    #undef VEC_OPERATOR

    #undef OP__add
    #undef OP__sub
    #undef OP__mul
    #undef OP__div

    #undef OP_add
    #undef OP_sub
    #undef OP_mul
    #undef OP_div
    #undef OP_eq_add
    #undef OP_eq_sub
    #undef OP_eq_mul
    #undef OP_eq_div

    #undef OP_
    #undef OP
    #undef OP_eq

    #undef VEC4_H_OPERATOR
    #undef VEC4_OPERATOR

    #undef VEC_SWIZZLING_2
    #undef VEC_SWIZZLING_3
    #undef VEC_SWIZZLING
  }

  void print_vec4(const vec4 &v);
  vec2 yaw_pitch_from_direction(const vec3 &direction);
  void yaw_pitch_from_direction(const vec3 &direction, float *yaw, float *pitch);
  vec3 direction_vec(float pitch, float yaw);
  vec3 direction_vec(const vec2 &v);
  vec3 right_direction_vec(float yaw);
}

#define mat4 mat4_t
struct mat4_t;

mat4 identity(float val);

struct mat4_t {
 private:
  vec4 data[4];
  
 public:
  mat4_t(void) {
    *this = identity(1.0f);
  }

  mat4_t(float value) {
    *this = identity(value);
  }

  mat4(float x0, float x1, float x2, float x3,
       float y0, float y1, float y2, float y3,
       float z0, float z1, float z2, float z3,
       float w0, float w1, float w2, float w3)
  : data({x0, x1, x2, x3},
         {y0, y1, y2, y3},
         {z0, z1, z2, z3},
         {w0, w1, w2, w3}) {}

  mat4(const vec4 &x,
       const vec4 &y,
       const vec4 &z, 
       const vec4 &w)
  : data(x, y, z, w) {}


  // template <typename... Args>
  // mat4_t(Args &&...args) {
  //   static_assert((sizeof...(Args) == 16 || sizeof...(Args) == 4), "Must be 16 elements.");
  //   if constexpr (all_same_v<float, Args...> && sizeof...(args) == 16) {
  //     float tmp[16] = {(float)(args)...};
  //     for (Uint i = 0; i < 4; ++i) {
  //       data[i] = {tmp[i * 4], tmp[(i * 4) + 1], tmp[(i * 4) + 2], tmp[(i * 4) + 3]};
  //     }
  //   }
  //   else if constexpr (all_same_v<double, Args...> && sizeof...(args) == 16) {
  //     float tmp[16] = {(float)(args)...};
  //     for (Uint i = 0; i < 4; ++i) {
  //       data[i] = {tmp[i * 4], tmp[(i * 4) + 1], tmp[(i * 4) + 2], tmp[(i * 4) + 3]};
  //     }
  //   }
  //   else if constexpr (all_same_v<vec4, Args...> && sizeof...(args) == 4) {
  //     vec4 tmp[4] = {(vec4)(args)...};
  //     for (Uint i = 0; i < 4; ++i) {
  //       data[i] = tmp[i];
  //     }
  //   }
  //   // else if constexpr (all_same_v<mat4, Args...> && sizeof...(args) == 1) {
  //   //   *this = identity(1.0f);
  //   // }
  // }

  vec4 &      operator[](Uint idx)       { return data[idx]; }
  const vec4 &operator[](Uint idx) const { return data[idx]; }

  mat4_t operator*(float value) {
    __avx scalar(value);
    __avx a0(data[0][0], data[0][1], data[0][2], data[0][3], data[1][0], data[1][1], data[1][2], data[1][3]);
    __avx a1(data[2][0], data[2][1], data[2][2], data[2][3], data[3][0], data[3][1], data[3][2], data[3][3]);
    __avx r0 = (a0 * scalar);
    __avx r1 = (a1 * scalar);
    mat4_t ret(
      r0[0], r0[1], r0[2], r0[3],
      r0[4], r0[5], r0[6], r0[7],
      r1[0], r1[1], r1[2], r1[3],
      r1[4], r1[5], r1[6], r1[7]
    );
    return ret;
  }

  mat4_t operator*(const mat4_t &m) const {
    mat4_t result;
    /* Process two rows at a time */
    for (Uint i = 0; i < 4; i += 2) {
      __m256 row1 = _mm256_set_ps(
        (*this)[i + 1][3], (*this)[i + 1][2], (*this)[i + 1][1], (*this)[i + 1][0],
        (*this)[i][3], (*this)[i][2], (*this)[i][1], (*this)[i][0]
      );
      for (Uint j = 0; j < 4; ++j) {
        /* Column vector of `m` */
        __m256 col = _mm256_set_ps(
          m[3][j], m[2][j], m[1][j], m[0][j],
          m[3][j], m[2][j], m[1][j], m[0][j]
        );
        /* Multiply and sum for dot products */
        __m256 dot = _mm256_dp_ps(row1, col, 0xFF);
        /* Store results */
        result[i][j] = ((float*)&dot)[0];
        result[i + 1][j] = ((float*)&dot)[4];
      }
    }
    return result;
  }
};

void print_mat4(const mat4 &m);

float length(const vec3 &v);
vec3 normalize(const vec3 &v);
mat4 ortho_projection(float left, float right, float top, float bottom);
mat4 ortho_projection(const vec4 &v);
mat4 transpose(const mat4 &m);
mat4 inverse(const mat4 &m);
mat4 translate_matrix(const mat4 &m, const vec3 &v);
mat4 look_at_rh(const vec3 &eye, const vec3 &center, const vec3 &up);
mat4 look_at_lh(const vec3 &eye, const vec3 &center, const vec3 &up);
mat4 perspective(float fovy, float aspect, float znear, float zfar);
mat4 scale_matrix(const mat4 &m, const vec3 &v);
mat4 rotate_matrix(const mat4 &m, float angle, const vec3 &v);
mat4 rotate_matrix(const mat4 &m, const vec3 &rotation);
mat4 compute_rotation_matrix(float angle);
mat4 rotation_matrix(const vec3 &rotation);

inline namespace GlSsbo {
  template<typename T, Uint Binding>
  class glSsbo {
   private:
    MVector<T> _data;
    Uint _buffer;

    T *_get_buffer_range(Uint start_idx, Uint n_idx, Uint flags) {
      return (T *)glMapBufferRange(
        GL_SHADER_STORAGE_BUFFER,
        start_idx * sizeof(T),
        n_idx * sizeof(T),
        flags
      );
    }

   public:
    glSsbo(void) : _data{} {}
    glSsbo(MVector<T> data) : _data{data} {}

    ~glSsbo(void) {
      glDeleteBuffers(1, &_buffer);
    }

    void init(const MVector<T> &data = {}) {
      if (!data.empty()) {
        _data = data;
      }
      glGenBuffers(1, &_buffer);
      glBindBuffer(GL_SHADER_STORAGE_BUFFER, _buffer);
      glBufferData(GL_SHADER_STORAGE_BUFFER, (_data.size() * sizeof(T)), _data.data(), GL_STATIC_DRAW);
      glBindBufferBase(GL_SHADER_STORAGE_BUFFER, Binding, _buffer);
    }

    Uint ssbo(void) const {
      return _buffer;
    }

    template <typename Action>
    void perform_write_action_on_buffer_range(Uint start_idx, Uint n_idx, Action &&action) {
      /* Return if start index or (start index + num of index) is bigger then the size of the buffer. */
      if (start_idx >= _data.size() || start_idx + n_idx > _data.size()) {
        return;
      }
      glBindBuffer(GL_SHADER_STORAGE_BUFFER, _buffer);
      /* Access the selected range of the buffer. */
      T *data = _get_buffer_range(start_idx, n_idx, (GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT));
      if (data) {
        action(data, n_idx);
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
      }
      else {
        printe("Failed to map buffer range.");
      }
      glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    void resize(Uint new_size) {
      _data = MVector<T>(new_size);
      glBindBuffer(GL_SHADER_STORAGE_BUFFER, _buffer);
      glBufferData(GL_SHADER_STORAGE_BUFFER, (_data.size() * sizeof(T)), _data.data(), GL_STATIC_DRAW);
      glBindBufferBase(GL_SHADER_STORAGE_BUFFER, Binding, _buffer);
    }

    Uint size(void) const {
      return _data.size();
    }
  };
}

inline namespace GlCamera {
  enum {
    GL_CAMERA_ANGLE_CHANGED,
    GL_FREE_CAMERA
  };

  typedef struct {
    float sensitivity;
    float pitch;
    float yaw;
    mat4 view;
    vec3 size;
    vec3 direction;
    vec3 up;
    vec3 pos;
    vec3 vel;
    vec3 accel;
    bit_flag_t<8> flag;
  } glCamera;

  void glCamera_init(glCamera *c, float sensitivity, const vec3 &pos, const vec3 &size = vec3(0.0f));
  void glCamera_direction(glCamera *c);
  void glCamera_change_angle(glCamera *c, const vec2 &change);
  void glCamera_set_view(glCamera *c, const vec3 &pos);
  void glCamera_change_pos(glCamera *c, const vec3 &change);
  void glCamera_update(glCamera *c, int loc);
}
