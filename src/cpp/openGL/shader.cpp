#include "../../include/openGL/shader.h"

#include "../../include/File.h"
#include "../../include/float_calc.h"

/* Read file at path into a std::string.  Retruns an empty string on error. */
std::string openGL_read_shader_file(const char *path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    return "";
  }
  std::stringstream ss;
  ss << file.rdbuf();
  return ss.str();
}

Uint openGL_load_shader_file(const char *path, Uint type) {
  int err;
  char *source = read_file_with_lock(path, NULL, &err);
  if (!source) {
    printe("Failed to read file: '%s', errno_str: '%s'.\n", path, errno_str(err));
    return 0;
  }
  const char *source_cstr = source;
  Uint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source_cstr, nullptr);
  glCompileShader(shader);
  free(source);
  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    static char err_log[1024];
    glGetShaderInfoLog(shader, sizeof(err_log), nullptr, err_log);
    printe("Failed to compile shader file: %s\n\nError log: %s\n", path, err_log);
  }
  return shader;
}

Uint openGL_load_shader_file_raw(const char *str, Uint type) {
  Uint shader = glCreateShader(type);
  glShaderSource(shader, 1, &str, nullptr);
  glCompileShader(shader);
  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    static char err_log[1024];
    glGetShaderInfoLog(shader, sizeof(err_log), nullptr, err_log);
    printe("Failed to compile shader.\n\nError log: %s\n", err_log);
  }
  return shader;
}

Uint openGL_create_shader_program(const MVector<Pair<const char *, Uint>> &parts) {
  MVector<Uint> shaders;
  for (const auto &part : parts) {
    shaders.push_back(openGL_load_shader_file(part.first, part.second));
  }
  Uint program = glCreateProgram();
  for (const auto &shader : shaders) {
    glAttachShader(program, shader);
  }
  glLinkProgram(program);
  int success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    static char err_log[1024];
    glGetProgramInfoLog(program, sizeof(err_log), NULL, err_log);
    printe("Failed to link shader program: %s\n", err_log);
  }
  for (auto &shader : shaders) {
    glDeleteShader(shader);
  }
  return program;
}

Uint openGL_create_shader_program_raw(const MVector<Pair<const char *, Uint>> &parts) {
  MVector<Uint> shaders;
  for (const auto &part : parts) {
    shaders.push_back(openGL_load_shader_file_raw(part.first, part.second));
  }
  Uint program = glCreateProgram();
  for (const auto &shader : shaders) {
    glAttachShader(program, shader);
  }
  glLinkProgram(program);
  int success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    static char err_log[1024];
    glGetProgramInfoLog(program, sizeof(err_log), NULL, err_log);
    printe("Failed to link shader program: %s\n", err_log);
  }
  for (auto &shader : shaders) {
    glDeleteShader(shader);
  }
  return program;
}

inline namespace Vec {
  void print_vec4(const vec4 &v) {
    printf("[ ");
    for (Uint i = 0; i < 4; ++i) {
      if (i != 3) {
        printf("%.2f, ", v[i]);
      }
      else {
        printf("%.2f", v[i]);
      }
    }
    printf(" ]\n");
    fflush(stdout);
  }

  vec2 yaw_pitch_from_direction(const vec3 &direction) {
    vec2 ret;
    /* Ensure the direction is normalized */
    vec3 dir = normalize(direction);
    /* Calculate pitch and yaw from the direction vector */
    ret.x = degreesf(atan2(dir.z, dir.x)); 
    ret.y = degreesf(asin(dir.y));
    return ret;
  }

  void yaw_pitch_from_direction(const vec3 &direction, float *yaw, float *pitch) {
    /* Ensure the direction is normalized */
    vec3 dir = normalize(direction);
    /* Calculate pitch and yaw from the direction vector */
    *yaw   = degreesf(atan2(dir.z, dir.x));
    *pitch = degreesf(asin(dir.y));
  }

  vec3 direction_vec(float yaw, float pitch) {
    vec3 dir;
    dir.x = (cos(radiansf(yaw)) * cos(radiansf(pitch)));
    dir.y = sin(radiansf(pitch));
    dir.z = (sin(radiansf(yaw)) * cos(radiansf(pitch)));
    return dir;
  }
  
  vec3 direction_vec(const vec2 &v) {
    vec3 dir;
    dir.x = (cos(radiansf(v.x)) * cos(radiansf(v.y)));
    dir.y = sin(radiansf(v.y));
    dir.z = (sin(radiansf(v.x)) * cos(radiansf(v.y)));
    return dir;
  }

  vec3 right_direction_vec(float yaw) {
    vec3 ret;
    ret.x = cos(radiansf(yaw + 90.0f));
    ret.y = 0.0f;
    ret.z = sin(radiansf(yaw + 90.0f));
    return ret;
  }
}

void print_mat4(const mat4 &m) {
  for (Uint i = 0; i < 4; ++i) {
    printf("[ ");
    for (Uint j = 0; j < 4; ++j) {
      if (j != 3) {
        printf("%.2f, ", m[i][j]);
      }
      else {
        printf("%.2f", m[i][j]);
      }
    }
    printf(" ]\n");
  }
  fflush(stdout);
}

float length(const vec3 &v) {
  __m128 a = _mm_setr_ps(v[0], v[1], v[2], 0.0f);
  __m128 r = _mm_mul_ps(a, a);
  r = _mm_hadd_ps(r, r);
  r = _mm_hadd_ps(r, r);
  return sqrt(_mm_cvtss_f32(r));
}

float dot(const vec3 &v0, const vec3 &v1) {
  __m128 a = _mm_setr_ps(v0[0], v0[1], v0[2], 0.0f);
  __m128 b = _mm_setr_ps(v1[0], v1[1], v1[2], 0.0f);
  __m128 r = _mm_mul_ps(a, b);
  r = _mm_hadd_ps(r, r);
  r = _mm_hadd_ps(r, r);
  return _mm_cvtss_f32(r);
}

vec3 normalize(const vec3 &v) {
  return (v / vec3(length(v)));
}

vec3 cross(const vec3 &v0, const vec3 &v1) {
  return vec3(
    ((v0[1] * v1[2]) - (v0[2] * v1[1])),
    ((v0[2] * v1[0]) - (v0[0] * v1[2])),
    ((v0[0] * v1[1]) - (v0[1] * v1[0]))
  );
}

mat4 ortho_projection(float left, float right, float top, float bottom) {
  mat4 ret;
  ret[0][0] = 2.0f / (right - left);
  ret[1][1] = 2.0f / (top - bottom);
  ret[2][2] = -1.0f;
  ret[3][0] = (-(right + left) / (right - left));
  ret[3][1] = (-(top + bottom) / (top - bottom));
  return ret;
}

mat4 ortho_projection(const vec4 &v) {
  mat4 ret;
  ret[0][0] = (2.0f / (v.right - v.left));
  ret[1][1] = (2.0f / (v.top - v.bottom));
  ret[2][2] = -1.0f;
  ret[3][0] = (-(v.right + v.left) / (v.right - v.left));
  ret[3][1] = (-(v.top + v.bottom) / (v.top - v.bottom));
  return ret;
}

mat4 transpose(const mat4 &m) {
  mat4 ret;

  ret[0][0] = m[0][0];
  ret[0][1] = m[1][0];
  ret[0][2] = m[2][0];
  ret[0][3] = m[3][0];

  ret[1][0] = m[0][1];
  ret[1][1] = m[1][1];
  ret[1][2] = m[2][1];
  ret[1][3] = m[3][1];
  
  ret[2][0] = m[0][2];
  ret[2][1] = m[1][2];
  ret[2][2] = m[2][2];
  ret[2][3] = m[3][2];

  ret[3][0] = m[0][3];
  ret[3][1] = m[1][3];
  ret[3][2] = m[2][3];
  ret[3][3] = m[3][3];

  return ret;
}

mat4 inverse(const mat4 &m) {
  float coef_00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
  float coef_02 = m[1][2] * m[3][3] - m[3][2] * m[1][3]; 
  float coef_03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

  float coef_04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
  float coef_06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
  float coef_07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

  float coef_08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
  float coef_10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
  float coef_11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
  
  float coef_12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
  float coef_14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
  float coef_15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];
  
  float coef_16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
  float coef_18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
  float coef_19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];
  
  float coef_20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
  float coef_22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
  float coef_23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

  vec4 fac_0(coef_00, coef_00, coef_02, coef_03);
  vec4 fac_1(coef_04, coef_04, coef_06, coef_07);
  vec4 fac_2(coef_08, coef_08, coef_10, coef_11);
  vec4 fac_3(coef_12, coef_12, coef_14, coef_15);
  vec4 fac_4(coef_16, coef_16, coef_18, coef_19);
  vec4 fac_5(coef_20, coef_20, coef_22, coef_23);

  vec4 vec_0(m[1][0], m[0][0], m[0][0], m[0][0]);
  vec4 vec_1(m[1][1], m[0][1], m[0][1], m[0][1]);
  vec4 vec_2(m[1][2], m[0][2], m[0][2], m[0][2]);
  vec4 vec_3(m[1][3], m[0][3], m[0][3], m[0][3]);

  vec4 inv_0(vec_1 * fac_0 - vec_2 * fac_1 + vec_3 * fac_2);
  vec4 inv_1(vec_0 * fac_0 - vec_2 * fac_3 + vec_3 * fac_4);
  vec4 inv_2(vec_0 * fac_1 - vec_1 * fac_3 + vec_3 * fac_5);
  vec4 inv_3(vec_0 * fac_2 - vec_1 * fac_4 + vec_2 * fac_5);

  vec4 sign_a( 1.0f, -1.0f,  1.0f, -1.0f);
  vec4 sign_b(-1.0f,  1.0f, -1.0f,  1.0f);
  mat4 inverse(inv_0 * sign_a, inv_1 * sign_b, inv_2 * sign_a, inv_3 * sign_b);

  vec4 row(inverse[0][0], inverse[1][0], inverse[2][0], inverse[3][0]);
  vec4 dot_0(m[0] * row);
  float dot_1 = ((dot_0.x + dot_0.y) + (dot_0.z + dot_0.w));
  float one_over_determinant = (1.0f / dot_1);

  return inverse * one_over_determinant;
}

mat4 translate_matrix(const mat4 &m, const vec3 &v) {
  mat4 ret(m);
  ret[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
  return ret;
}

mat4 look_at_rh(const vec3 &eye, const vec3 &center, const vec3 &up) {
  const vec3 f(normalize(center - eye));
  const vec3 s(normalize(cross(f, up)));
  const vec3 u(cross(s, f));
  mat4 ret(1.0f);
  ret[0][0] = s.x;
  ret[1][0] = s.y;
  ret[2][0] = s.z;
  ret[0][1] = u.x;
  ret[1][1] = u.y;
  ret[2][1] = u.z;
  ret[0][2] = f.x;
  ret[1][2] = f.y;
  ret[2][2] = f.z;
  ret[3][0] = -dot(s, eye);
  ret[3][1] = -dot(u, eye);
  ret[3][2] = -dot(f, eye);
  return ret;
}

mat4 look_at_lh(const vec3 &eye, const vec3 &center, const vec3 &up) {
  const vec3 f(normalize(center - eye));
  const vec3 s(normalize(cross(up, f)));
  const vec3 u(cross(f, s));
  mat4 ret(1.0f);
  ret[0][0] = s.x;
  ret[1][0] = s.y;
  ret[2][0] = s.z;
  ret[0][1] = u.x;
  ret[1][1] = u.y;
  ret[2][1] = u.z;
  ret[0][2] = f.x;
  ret[1][2] = f.y;
  ret[2][2] = f.z;
  ret[3][0] = -dot(s, eye);
  ret[3][1] = -dot(u, eye);
  ret[3][2] = -dot(f, eye);
  return ret;
}

mat4 perspective(float fovy, float aspect, float znear, float zfar) {
  const float tanHalfFovy = tan(fovy / 2.0f);
  mat4 ret(0.0f);
  ret[0][0] = 1.0f / (aspect * tanHalfFovy);
  ret[1][1] = 1.0f / (tanHalfFovy);
  ret[2][2] = zfar / (zfar - znear);
  ret[2][3] = 1.0f;
  ret[3][2] = -(zfar * znear) / (zfar - znear);
  return ret;
}

mat4 scale_matrix(const mat4 &m, const vec3 &v) {
  return mat4(
    (m[0] * v[0]),
    (m[1] * v[1]),
    (m[2] * v[2]),
    m[3]
  );
}

mat4 rotate_matrix(const mat4 &m, float angle, const vec3 &v) {
  const float a = angle;
  const float c = cos(a);
  const float s = sin(a);
  vec3 axis(normalize(v));
  vec3 temp(vec3(1.0f - c) * axis);

  mat4 rotate;
  rotate[0][0] = c + temp[0] * axis[0];
  rotate[0][1] = temp[0] * axis[1] + s * axis[2];
  rotate[0][2] = temp[0] * axis[2] - s * axis[1];

  rotate[1][0] = temp[1] * axis[0] - s * axis[2];
  rotate[1][1] = c + temp[1] * axis[1];
  rotate[1][2] = temp[1] * axis[2] + s * axis[0];

  rotate[2][0] = temp[2] * axis[0] + s * axis[1];
  rotate[2][1] = temp[2] * axis[1] - s * axis[0];
  rotate[2][2] = c + temp[2] * axis[2];

  mat4 ret;
  ret[0] = m[0] * rotate[0][0] + m[1] * rotate[0][1] + m[2] * rotate[0][2];
  ret[1] = m[0] * rotate[1][0] + m[1] * rotate[1][1] + m[2] * rotate[1][2];
  ret[2] = m[0] * rotate[2][0] + m[1] * rotate[2][1] + m[2] * rotate[2][2];
  ret[3] = m[3];
  return ret;
}

mat4 rotate_matrix(const mat4 &m, const vec3 &rotation) {
  mat4 ret = m;
  ret = rotate_matrix(ret, rotation.x, vec3(1.0f, 0.0f, 0.0f));
  ret = rotate_matrix(ret, rotation.y, vec3(0.0f, 1.0f, 0.0f));
  ret = rotate_matrix(ret, rotation.z, vec3(0.0f, 0.0f, 1.0f));
  return ret;
}

mat4 compute_rotation_matrix(float angle) {
  float c = cos(angle);
  float s = sin(angle);
  return mat4(
    c,  -s,  0.0f,  0.0f,
    s,   c,  0.0f,  0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  );
}

mat4 identity(float val) {
  return mat4(
    val,  0.0f, 0.0f, 0.0f,
    0.0f, val,  0.0f, 0.0f,
    0.0f, 0.0f, val,  0.0f,
    0.0f, 0.0f, 0.0f, val
  );
}

inline namespace GlCamera {
  void glCamera_init(glCamera *c, float sensitivity, const vec3 &pos, const vec3 &size) {
    c->size  = size;
    c->pos   = pos;
    c->vel   = vec3(0.0f);
    c->accel = vec3(0.0f);
    c->view  = translate_matrix(mat4(1.0f), c->pos);
    yaw_pitch_from_direction(c->pos, &c->yaw, &c->pitch);
    c->sensitivity = sensitivity;
    glCamera_direction(c);
  }

  /* Calculate the camera's forward direction from yaw and pitch */
  void glCamera_direction(glCamera *c) {
    c->direction = normalize(direction_vec(c->yaw, c->pitch));
    c->up = vec3(0.0f, 1.0f, 0.0f);
    c->flag.unset<GL_CAMERA_ANGLE_CHANGED>();
  }

  void glCamera_change_angle(glCamera *c, const vec2 &change) {
    c->flag.set<GL_CAMERA_ANGLE_CHANGED>();
    c->yaw   += (change.x * c->sensitivity);
    c->pitch += (change.y * c->sensitivity);
    Clamp(c->pitch, -89.9f, 89.9f);
  }

  void glCamera_set_view(glCamera *c, const vec3 &pos) {
    c->pos = pos;
    c->view = translate_matrix({1.0f}, c->pos);
  }

  void glCamera_change_pos(glCamera *c, const vec3 &change) {
    /* Calculate forward, right, and up vectors based on yaw and pitch */
    vec3 forward;
    vec3 right = right_direction_vec(c->yaw);
    vec3 up    = {0.0f, 1.0f, 0.0f};
    if (c->flag.is_set<GL_FREE_CAMERA>()) {
      forward = direction_vec(c->yaw, c->pitch);
    }
    else {
      forward = direction_vec(c->yaw, 0.0f);
      /* Nullify any up/down movement. */
      forward.y = 0.0f;
      right.y   = 0.0f;
    }
    /* Move the camera based on the relative change. */
    c->pos -= (forward * change.z) + (right * change.x) + (up * change.y);
  }

  void glCamera_update(glCamera *c, int loc) {
    if (c->flag.is_set<GL_CAMERA_ANGLE_CHANGED>()) {
      glCamera_direction(c);
    }
    c->view = look_at_rh(c->pos, (c->pos - c->direction), c->up);
    glUniformMatrix4fv(loc, 1, GL_FALSE, &c->view[0][0]);
  }
}