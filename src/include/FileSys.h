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


namespace Mlib::FileSys
{
    enum Mode : u8
    {
        NONE            = (0),
        MKDIR_RECURSIVE = (1 << 0),
        NO_THROW        = (1 << 1),
    };

    auto           fileContentToStr(const STRING &filename) -> STRING;
    VECTOR<STRING> fileContentToStrVec(const STRING &filename, const u8 mode = NONE);
    auto           fileContentToFile(const STRING &sourcePath, const STRING &destinationPath) -> void;

    /// @name dirContentToStrVec
    /// @brief
    /// - Retrieve the content of a directory as a vector of strings.
    /// @param path
    /// - The path to the directory.
    /// @returns vector<string>
    /// - The content of the directory as a vector of strings.
    VECTOR<STRING> dirContentToStrVec(const STRING &path);

    STRING currentWorkingDir();

    void cd(const STRING &path);

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
    void mkdir(const STRING &path, u8 mode = NONE);

    void   rmdir(const STRING &path);
    void   rmFile(const STRING &path);
    void   touch(const STRING &path);
    bool   exists(STRING_VIEW path);
    bool   isDir(STRING_VIEW path);
    bool   isFile(STRING_VIEW path);
    time_t fileLastWriteTime(const STRING &path);
    STRING fileLastWriteTimeStr(const STRING &path);
    STRING fileLastWriteTimeStr(const STRING &path, const STRING &format);
    size_t fileSize(const STRING &path);

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
    void writeStrVecToFile(const STRING &path, const VECTOR<STRING> &lines);

    bool doesFileExistInDirs(const STRING &name, const VECTOR<STRING> &dirPathsVec, const STRING &fileExtention = "");

    [[nodiscard]]
    inline bool
    is_regular_file(STRING_VIEW p, ERROR_CODE &ec) _NO_THROW
    {
        return FS::is_regular_file(p, ec);
    }

} // namespace Mlib::FileSys
