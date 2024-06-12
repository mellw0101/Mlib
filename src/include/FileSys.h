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
    auto dirContentToStrVec(const string& path) -> vector<string>;
    auto fileContentToStr(const string& filename) -> string;
    auto fileContentToFile(const string& sourcePath, const string& destinationPath) -> void;
    auto currentWorkingDir() -> string;
    auto fileExists(const string& path) -> bool;
    auto isDir(const string& path) -> bool;
    auto isFile(const string& path) -> bool;
    auto fileLastWriteTime(const string& path) -> std::time_t;
    auto fileLastWriteTimeStr(const string& path) -> string;
    auto fileLastWriteTimeStr(const string& path, const string& format) -> string;
    auto fileSize(const string& path) -> size_t;
} // namespace Mlib::FileSys
