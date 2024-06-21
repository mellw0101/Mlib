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

using namespace std;

namespace Mlib::FileSys {
    enum Mode : u8
    {
        NONE            = (0),
        MKDIR_RECURSIVE = (1 << 0),
        NO_THROW        = (1 << 1),
    };

    auto           fileContentToStr(const string &filename) -> string;
    vector<string> fileContentToStrVec(const string &filename, const u8 mode = NONE);
    auto           fileContentToFile(const string &sourcePath, const string &destinationPath) -> void;

    /// @name dirContentToStrVec
    /// @brief
    /// - Retrieve the content of a directory as a vector of strings.
    /// @param path
    /// - The path to the directory.
    /// @returns vector<string>
    /// - The content of the directory as a vector of strings.
    vector<string> dirContentToStrVec(string const &path);

    string currentWorkingDir();

    void cd(const string &path);

    /// @name mkdir
    /// @brief
    ///  - Create a directory.
    /// @param path
    ///  - The path to the directory.
    /// @param mode
    ///  - The mode to use when creating the directory.
    ///  - Modes:
    ///  - - ( NONE )            - No mode.
    ///  - - ( MKDIR_RECURSIVE ) - Create parent directories if they do not exist.
    ///  - - ( NO_THROW )        - Do not throw exceptions.
    /// @returns void
    /// @throws ( if mode & NO_THROW == false )
    ///  - ( runtime_error ) - If path is directory.
    ///  - ( runtime_error ) - If the directory cannot be created.
    ///  - ( runtime_error ) - If the directory already exists.
    void mkdir(const string &path, u8 mode = NONE);

    void   rmdir(const string &path);
    void   rmFile(const string &path);
    void   touch(const string &path);
    bool   exists(const string &path);
    bool   isDir(const string &path);
    bool   isFile(const string &path);
    time_t fileLastWriteTime(const string &path);
    string fileLastWriteTimeStr(const string &path);
    string fileLastWriteTimeStr(const string &path, const string &format);
    size_t fileSize(const string &path);

    /// @name strVecToFile
    /// @brief
    ///  - Write a vector of strings to a file.
    /// @param path
    ///  - The path to the file.
    /// @param content
    ///  - The vector of strings to write to the file.
    /// @returns void
    /// @throws
    ///  - ( runtime_error ) - If the file cannot be opened.
    ///  - ( runtime_error ) - If the file cannot be written to.
    ///  - ( runtime_error ) - If the file cannot be closed.
    ///  - ( runtime_error ) - If the file is a dir.
    void writeStrVecToFile(const string &path, const vector<string> &lines);

    bool doesFileExistInDirs(const string &name, const vector<string> &dirPathsVec, const string &fileExtention = "");
} // namespace Mlib::FileSys
