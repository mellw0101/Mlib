#include "../../include/openGL/shader.h"

// Read file at path into a std::string.  Retruns an empty string on error.
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
  std::string source = openGL_read_shader_file(path).c_str();
  const char *source_cstr = source.c_str();
  Uint shader = glCreateShader(type);
  __glewShaderSource(shader, 1, &source_cstr, nullptr);
  __glewCompileShader(shader);
  int success;
  __glewGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    static char err_log[1024];
    __glewGetShaderInfoLog(shader, sizeof(err_log), nullptr, err_log);
    fprintf(stderr, "Failed to compile shader file: %s\n\n"
                    "Error log: %s\n", path, err_log);
  }
  return shader;
}

Uint openGL_create_shader_program(const MVector<Pair<const char *, Uint>> &parts) {
  MVector<Uint> shaders;
  for (const auto &part : parts) {
    shaders.push_back(openGL_load_shader_file(part.first, part.second));
  }
  Uint program = __glewCreateProgram();
  for (const auto &shader : shaders) {
    __glewAttachShader(program, shader);
  }
  __glewLinkProgram(program);
  int success;
  __glewGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    static char err_log[1024];
    __glewGetProgramInfoLog(program, sizeof(err_log), NULL, err_log);
    fprintf(stderr, "Failed to link shader program: %s\n", err_log);
  }
  for (auto &shader : shaders) {
    __glewDeleteShader(shader);
  }
  return program;
}
