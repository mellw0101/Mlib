#pragma once
/* clang-format off */
#include "../Mint.h"
#include "../Mbool.h"
#include "../Vector.h"
#include "../Pair.h"
#include "../Error.h"
#include "../Types.h"
#include "../simd.h"
#include "../Flag.h"
#include "../Debug.h"

#include "vec.h"

#include <string>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <unordered_map>

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
    glSsbo(void) : _data{}, _buffer(0) {}
    glSsbo(MVector<T> data) : _data{data} {}

    ~glSsbo(void) {
      if (_buffer) {
        glDeleteBuffers(1, &_buffer);
      }
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

struct cordinathashstruct {
  Ulong operator()(const ivec2 &coord) const {
    Ulong hash_x = std::hash<int>()(coord.x);
    Ulong hash_y = std::hash<int>()(coord.y);
    return hash_x ^ (hash_y * 0x9e3779b9 + (hash_x << 6) + (hash_x >> 2));
  }
};

template<typename T>
class gridmapstruct {
 private:
  int cell_size;
  std::unordered_map<ivec2, MVector<T>, cordinathashstruct> grid;

  ivec2 to_grid_pos(const ivec2 &pos) const {
    return ivec2(pos / cell_size);
  }

 public:
  gridmapstruct(int cell_size) : cell_size(cell_size) {};

  void set(const ivec2 &pos, T element) {
    grid[to_grid_pos(pos)].emplace_back(element);
  }

  void set(const ivec2 &pos, const ivec2 &size, T element) {
    ivec2 start = to_grid_pos(pos);
    ivec2 end   = to_grid_pos(pos + size);
    for (int x = start.x; x <= end.x; ++x) {
      for (int y = start.y; y <= end.y; ++y) {
        grid[ivec2(x, y)].emplace_back(element);
      }
    }
  }

  void remove(const ivec2 &pos, T e) {
    ivec2 at_pos = to_grid_pos(pos);
    auto &vector = grid.at(at_pos);
    for (Uint i = 0; i < vector.size(); ++i) {
      if (grid.at(at_pos)[i] == e) {
        vector.erase(&vector[i--]);
      }
    }
    if (vector.empty()) {
      grid.erase(at_pos);
    }
  }

  /* Remove all entries for a given element, at all grids based on its size. */
  void remove(const ivec2 &pos, const ivec2 &size, T e) {
    ivec2 start = to_grid_pos(pos);
    ivec2 end   = to_grid_pos(pos + size);
    for (int x = start.x; x <= end.x; ++x) {
      for (int y = start.y; y <= end.y; ++y) {
        ivec2 at_pos(x, y);
        auto &vector = grid.at(at_pos);
        for (Uint i = 0; i < vector.size(); ++i) {
          if (vector[i] == e) {
            vector.erase(&vector[i--]);
          }
        }
        if (vector.empty()) {
          grid.erase(at_pos);
        }
      }
    }
  }

  const MVector<T>& get(const ivec2 &pos) const {
    return grid[to_grid_pos(pos)];
  }

  bool contains(const ivec2 &pos) const {
    return (grid.find(to_grid_pos(pos)) != grid.end());
  }
};