#include "../include/FileSys.h"

namespace fs = std::filesystem;

namespace Mlib::FileSys {
    string
    fileContentToStr(const string& filename)
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

    void
    fileContentToFile(const string& sourcePath, const string& destinationPath)
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

    vector<string>
    dirContentToStrVec(const string& path)
    {
        vector<string> files;
        for (const auto& entry : fs::directory_iterator(path))
        {
            files.push_back(entry.path().string());
        }
        return files;
    }

    void
    strVecToFile(string const& path, vector<string> const& lines)
    {
        ofstream file(path);
        if (!file.is_open())
        {
            throw runtime_error("Failed to open file: " + path);
        }
        for (string const& line : lines)
        {
            file << line << '\n';
        }
    }

    string
    currentWorkingDir()
    {
        return fs::current_path().string();
    }

    void
    mkdir(const string& path)
    {
        if (fs::exists(path))
        {
            if (!fs::is_directory(path))
            {
                throw runtime_error("Path exists but is not a directory: " + path);
            }
            throw runtime_error("Directory already exists: " + path);
        }
        if (!fs::create_directories(path))
        {
            throw runtime_error("Failed to create directory: " + path);
        }
    }

    void
    rmdir(const string& path)
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

    void
    rmFile(const string& path)
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

    void
    touch(const string& path)
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

    bool
    exists(const string& path)
    {
        return fs::exists(path);
    }

    bool
    isDir(const string& path)
    {
        return fs::is_directory(path);
    }

    bool
    isFile(const string& path)
    {
        return fs::is_regular_file(path);
    }

    time_t
    fileLastWriteTime(const string& path)
    {
        return fs::last_write_time(path).time_since_epoch().count();
    }

    string
    fileLastWriteTimeStr(const string& path)
    {
        time_t time = fileLastWriteTime(path);
        return ctime(&time);
    }

    string
    fileLastWriteTimeStr(const string& path, const string& format)
    {
        time_t       time       = fileLastWriteTime(path);
        tm*          timeStruct = localtime(&time);
        stringstream buffer;
        buffer << put_time(timeStruct, format.c_str());
        return buffer.str();
    }

    size_t
    fileSize(const string& path)
    {
        return fs::exists(path) ? fs::file_size(path) : 0;
    }
} // namespace Mlib::FileSys
