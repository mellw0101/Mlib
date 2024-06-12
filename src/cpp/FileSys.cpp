#include "../include/FileSys.h"

namespace fs = std::filesystem;

namespace Mlib::FileSys {
    auto fileContentToStr(const string& filename) -> string
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

    auto fileContentToFile(const string& sourcePath, const string& destinationPath) -> void
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

    auto dirContentToStrVec(const string& path) -> vector<string>
    {
        vector<string> files;
        for (const auto& entry : fs::directory_iterator(path))
        {
            files.push_back(entry.path().string());
        }
        return files;
    }

    auto currentWorkingDir() -> string
    {
        return fs::current_path().string();
    }

    auto makeDir(const string& path) -> void
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

    auto removeDir(const string& path) -> void
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

    auto removeFile(const string& path) -> void
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

    auto makeFile(const string& path) -> void
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

    auto fileExists(const string& path) -> bool
    {
        return fs::exists(path);
    }

    auto isDir(const string& path) -> bool
    {
        return fs::is_directory(path);
    }

    auto isFile(const string& path) -> bool
    {
        return fs::is_regular_file(path);
    }

    auto fileLastWriteTime(const string& path) -> time_t
    {
        return fs::last_write_time(path).time_since_epoch().count();
    }

    auto fileLastWriteTimeStr(const string& path) -> string
    {
        time_t time = fileLastWriteTime(path);
        return ctime(&time);
    }

    auto fileLastWriteTimeStr(const string& path, const string& format) -> string
    {
        time_t       time       = fileLastWriteTime(path);
        tm*          timeStruct = localtime(&time);
        stringstream buffer;
        buffer << put_time(timeStruct, format.c_str());
        return buffer.str();
    }

    auto fileSize(const string& path) -> size_t
    {
        return fs::exists(path) ? fs::file_size(path) : 0;
    }
} // namespace Mlib::FileSys
