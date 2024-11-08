#pragma once

#include "../def.h"
#include "../Vector.h"
#include "../Pair.h"

#include <string>
#include <fstream>
#include <sstream>
#include <GL/glew.h>

std::string openGL_read_shader_file(const char *path);
Uint openGL_load_shader_file(const char *path, Uint type);
Uint openGL_create_shader_program(const MVector<Pair<const char *, Uint>> &parts);