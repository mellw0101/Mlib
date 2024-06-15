/// @file Sys.h
#pragma once

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include "def.h"

namespace Mlib::Sys {
    s8 run_binary(const string& binary_path, const vector<string>& args);
}
