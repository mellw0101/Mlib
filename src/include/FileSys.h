#pragma once

#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include "def.h"

namespace Mlib::FileSys {
    FORCE_INLINE UNUSED auto dirContentToStrVec(const string& path) -> vector<string>;
    FORCE_INLINE UNUSED auto fileContentToStr(const string& filename) -> string;
    FORCE_INLINE UNUSED auto fileContentToFile(const string& sourcePath, const string& destinationPath) -> void;
    FORCE_INLINE UNUSED auto currentWorkingDir() -> string;
} // namespace Mlib::FileSys
