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

    enum opt
    {
        RETRIEVE_SIZE = 0
    };

    auto           fileContentToStr(const STRING &filename) -> STRING;
    VECTOR<STRING> fileContentToStrVec(const STRING &filename, const u8 mode = NONE);
    auto           fileContentToFile(const STRING &sourcePath, const STRING &destinationPath) -> void;

    //
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

    //
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

    //
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

    bool file_size(size_t *size, FILE *file);

    size_t retrieve_file_size_from_path(const char *file_path);

    [[nodiscard]]
    FILE *write_to_tmp_file(const void *buf __attribute__((nonnull)), unsigned long *bytes);

    int write_to_file(const void *buf, unsigned long *bytes, const char *file) __attribute_nonnull__((1, 3));

    void write_to_ffile(char *buf, const char *file);

    /**
        Returns a malloc`d 'char **' where all entries are malloc`d as well.
        So you need to free all entries as well as the array itself.
        i.e:
        -   unsigned long i;
        -   char **arry;
        -   arry = dir_content;
        -   for(i = 0; arry[i] != nullptr; i++)
        -   {
        -   -   free(arry[i]);
        -   -   arry[i] = nullptr;
        -   }
        -   free(arry);
        -   arry = nullptr;
        Or use 'dir_content_free';
     */
    char **dir_content(const char *path) __nonnull((1));

    /**
        Use this to free the 'char **' recieved from 'dir_content'.
        Pass by ref 'dir_content_free(&buf)'
     */
    void dir_content_free(char ***buf) __nonnull((1));

    void g_fsize(FILE *f, unsigned long *size) __nonnull((1));

} // namespace Mlib::FileSys
