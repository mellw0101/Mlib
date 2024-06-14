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
    auto fileContentToStr(const string& filename) -> string;
    auto fileContentToFile(const string& sourcePath, const string& destinationPath) -> void;

    /// @name dirContentToStrVec
    /// @brief Retrieve the content of a directory as a vector of strings.
    /// @param path The path to the directory.
    /// @returns vector<string>
    /// - The content of the directory as a vector of strings.
    vector<string> dirContentToStrVec(const string& path);

    /// @name strVecToFile
    /// @brief Write a vector of strings to a file.
    /// @param path The path to the file.
    /// @param content The vector of strings to write to the file.
    /// @returns void
    void strVecToFile(const string& path, const vector<string>& content);

    auto currentWorkingDir() -> string;
    void mkdir(const string& path);
    auto rmdir(const string& path) -> void;
    auto rmFile(const string& path) -> void;
    auto touch(const string& path) -> void;
    auto exists(const string& path) -> bool;
    auto isDir(const string& path) -> bool;
    auto isFile(const string& path) -> bool;
    auto fileLastWriteTime(const string& path) -> std::time_t;
    auto fileLastWriteTimeStr(const string& path) -> string;
    auto fileLastWriteTimeStr(const string& path, const string& format) -> string;
    auto fileSize(const string& path) -> size_t;
} // namespace Mlib::FileSys
