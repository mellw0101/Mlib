#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include "def.h"

namespace Mlib::Args {
    auto argvToStrVec(int argc, char** argv) -> vector<string>;
    auto flagValue(const string& flag, int argC, char** argV) -> string;
    auto flagExists(const string& flag, int argC, char** argV) -> bool;
    auto flagValuesToStrVec(vector<string>& flagVec, int argC, char** argV) -> vector<string>;
    auto strVecFromStr(const string& str, char delim) -> vector<string>;
}; // namespace Mlib::Args
