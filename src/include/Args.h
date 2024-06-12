#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include "def.h"

namespace Mlib::Args {
    FORCE_INLINE UNUSED auto argvToVec(int argc, char** argv) -> vector<string>;
    FORCE_INLINE UNUSED auto flagValue(const string& flag, int argC, char** argV) -> string;
    FORCE_INLINE UNUSED auto flagExists(const string& flag, int argC, char** argV) -> bool;
    FORCE_INLINE UNUSED auto flagValuesToStrVec(vector<string>& flagVec, int argC, char** argV) -> vector<string>;
    FORCE_INLINE UNUSED auto makeStrVecFromStr(const string& str, char delim) -> vector<string>;
    FORCE_INLINE UNUSED auto makeArgVec(const string& str, char delim) -> vector<string>;
}; // namespace Mlib::Args
