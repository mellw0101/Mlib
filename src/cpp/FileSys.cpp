#include "../include/FileSys.h"

namespace fs = std::filesystem;

namespace Mlib::FileSys {
    string fileContentToStr(const string& filename)
    {
        if (!fs::exists(filename))
        {
            throw runtime_error("File does not exist: " + filename);
        }
        ifstream file(filename);
        if (!file.is_open())
        {
            throw runtime_error("Could not open file: " + filename);
        }
        stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    FORCE_INLINE UNUSED auto fileContentToFile(const string& sourcePath, const string& destinationPath) -> void
    {
        if (!fs::exists(sourcePath))
        {
            throw runtime_error("Source file does not exist." + sourcePath);
        }
        ifstream sourceFile(sourcePath, ios::binary);

        if (!sourceFile.is_open())
        {
            throw runtime_error("Failed to open source file." + sourcePath);
        }
        ofstream destinationFile(destinationPath, ios::binary);

        if (!destinationFile.is_open())
        {
            throw runtime_error("Failed to open destination file. " + destinationPath);
        }
        destinationFile << sourceFile.rdbuf();

        sourceFile.close();
        destinationFile.close();
    }

    FORCE_INLINE UNUSED auto dirContentToStrVec(const string& path) -> vector<string>
    {
        vector<string> files;
        for (const auto& entry : fs::directory_iterator(path))
        {
            files.push_back(entry.path().string());
        }
        return files;
    }

    FORCE_INLINE UNUSED auto currentWorkingDir() -> string
    {
        return fs::current_path().string();
    }

    FORCE_INLINE UNUSED auto makeDir(const string& path) -> void
    {
        if (fs::exists(path))
        {
            throw runtime_error("Directory already exists: " + path);
        }
        if (!fs::create_directories(path))
        {
            throw runtime_error("Failed to create directory: " + path);
        }
    }

    FORCE_INLINE UNUSED auto removeDir(const string& path) -> void
    {
        if (!fs::exists(path))
        {
            throw runtime_error("Directory does not exist: " + path);
        }
        if (!fs::remove_all(path))
        {
            throw runtime_error("Failed to remove directory: " + path);
        }
    }

    FORCE_INLINE UNUSED auto removeFile(const string& path) -> void
    {
        if (!fs::exists(path))
        {
            throw runtime_error("File does not exist: " + path);
        }
        if (!fs::remove(path))
        {
            throw runtime_error("Failed to remove file: " + path);
        }
    }

    FORCE_INLINE UNUSED auto makeFile(const string& path) -> void
    {
        if (fs::exists(path))
        {
            throw runtime_error("File already exists: " + path);
        }
        ofstream file(path);
        if (!file.is_open())
        {
            throw runtime_error("Failed to create file: " + path);
        }
    }

    FORCE_INLINE UNUSED auto fileExists(const string& path) -> bool
    {
        return fs::exists(path);
    }

    FORCE_INLINE UNUSED auto isDir(const string& path) -> bool
    {
        return fs::is_directory(path);
    }

    FORCE_INLINE UNUSED auto isFile(const string& path) -> bool
    {
        return fs::is_regular_file(path);
    }

    FORCE_INLINE UNUSED auto fileLastWriteTime(const string& path) -> time_t
    {
        return fs::last_write_time(path).time_since_epoch().count();
    }

    FORCE_INLINE UNUSED auto fileLastWriteTimeStr(const string& path) -> string
    {
        auto const time  = fs::last_write_time(path);
        auto const ctime = decltype(time)::clock::to_time_t(time);
        return std::ctime(&ctime);
    }

    FORCE_INLINE UNUSED auto fileLastWriteTimeStr(const string& path, const string& format) -> string
    {
        auto              time  = fs::last_write_time(path);
        auto              ctime = decltype(time)::clock::to_time_t(time);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&ctime), format.c_str());
        return ss.str();
    }

    FORCE_INLINE UNUSED auto fileSize(const string& path) -> size_t
    {
        return fs::file_size(path);
    }
} // namespace Mlib::FileSys
