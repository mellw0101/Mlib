#include "../include/FileSys.h"
#include "../include/Args.h"
#include "../include/Error.h"

#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

namespace fs = std::filesystem;

namespace Mlib ::FileSys {
  string fileContentToStr(const string &filename) {
    if (!fs::exists(filename)) {
      throw runtime_error("File does not exist: " + filename);
    }
    ifstream file(filename);
    if (!file.is_open()) {
      throw runtime_error("Could not open file: " + filename);
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
  }

  vector<string> fileContentToStrVec(const string &filename, const Uchar mode) {
    /* Check if the file exists. */
    if (!fs::exists(filename)) {
      if ((mode & NO_THROW) == false) {
        /* If NO_THROW is set, dont throw an error. */
        throw runtime_error("File does not exist: " + filename);
      }
      return {};
    }
    vector<string> lines;
    ifstream       file(filename);
    if (!file.is_open()) {
      if ((mode & NO_THROW) == false) {
        throw runtime_error("Could not open file: " + filename);
      }
      // If NO_THROW is set, return an empty vector.
      return {};
    }
    string line;
    while (getline(file, line)) {
      lines.push_back(line);
    }
    return lines;
  }

  void fileContentToFile(const string &sourcePath, const string &destinationPath) {
    /** If the source file does not exist, @throw an error. */
    if (!fs::exists(sourcePath)) {
      throw runtime_error("Source file does not exist: " + sourcePath);
    }
    /** If the source file is a directory, @throw an error. */
    if (fs::is_directory(sourcePath)) {
      throw runtime_error("Source file is a directory: " + sourcePath);
    }
    ifstream sourceFile(sourcePath, ios::binary);
    /** If the source file cannot be opened, @throw an error. */
    if (!sourceFile.is_open()) {
      throw runtime_error("Failed to open source file: " + sourcePath);
    }
    ofstream destinationFile(destinationPath, ios::binary);
    // If the destination file cannot be opened, throw an error.
    if (!destinationFile.is_open()) {
      throw runtime_error("Failed to open destination file: " + destinationPath);
    }
    // Transfer the contents of the source file to the destination file.
    destinationFile << sourceFile.rdbuf();
    // Close the files.
    sourceFile.close();
    destinationFile.close();
    // If the file sizes do not match, throw an error.
    if (fileSize(sourcePath) != fileSize(destinationPath)) {
      throw runtime_error("Failed to copy file. File sizes do not match.");
    }
  }

  vector<string> dirContentToStrVec(const string &path) {
    vector<string> files;
    for (const auto &entry : fs::directory_iterator(path)) {
      files.push_back(entry.path().string());
    }
    return files;
  }

  string currentWorkingDir(void) {
    return fs::current_path().string();
  }

  void cd(const string &path) {
    if (!fs::exists(path)) {
      throw runtime_error("Directory does not exist: " + path);
    }
    if (!fs::is_directory(path)) {
      throw runtime_error("Path is not a directory: " + path);
    }
    fs::current_path(path);
  }

  void mkdir(const string &path, Uchar mode) {
    if (fs::exists(path)) {
      if (!fs::is_directory(path)) {
        if ((mode & NO_THROW) == false) {
          throw runtime_error("Path exists but is not a directory: " + path);
        }
      }
      if ((mode & NO_THROW) == false) {
        throw runtime_error("Directory already exists: " + path);
      }
    }
    if (!fs::create_directories(path)) {
      if ((mode & MKDIR_RECURSIVE) == true) {
        string         recursivePath = "";
        vector<string> dirs          = Args::strVecFromStr(path, '/');
        for (const string &dir : dirs) {
          recursivePath += dir;
          if (!fs::exists(recursivePath)) {
            if (!fs::create_directory(recursivePath)) {
              if ((mode & NO_THROW) == false) {
                throw runtime_error("Failed to recursively create directory: " + path);
              }
            }
          }
          recursivePath += "/";
        }
      }
      if ((mode & NO_THROW) == false) {
        throw runtime_error("Failed to create directory: " + path);
      }
    }
  }

  void rmdir(const string &path) {
    if (!fs::exists(path)) {
      throw runtime_error("Directory does not exist: " + path);
    }
    if (!fs::remove_all(path)) {
      throw runtime_error("Failed to remove directory: " + path);
    }
  }

  void rmFile(const string &path) {
    if (!fs::exists(path)) {
      throw runtime_error("File does not exist: " + path);
    }
    if (!fs::remove(path)) {
      throw runtime_error("Failed to remove file: " + path);
    }
  }

  void touch(const string &path) {
    if (fs::exists(path)) {
      throw runtime_error("File already exists: " + path);
    }
    ofstream file(path);
    if (!file.is_open()) {
      throw runtime_error("Failed to create file: " + path);
    }
  }

  bool exists(STRING_VIEW path) {
    return fs::exists(path);
  }

  bool isDir(STRING_VIEW path) {
    return fs::is_directory(path);
  }

  bool isFile(STRING_VIEW path) {
    return fs::is_regular_file(path);
  }

  bool isSymLink(STRING_VIEW path) {
    return fs::is_symlink(path);
  }

  time_t fileLastWriteTime(const string &path) {
    return fs::last_write_time(path).time_since_epoch().count();
  }

  string fileLastWriteTimeStr(const string &path) {
    time_t time = fileLastWriteTime(path);
    return ctime(&time);
  }

  string fileLastWriteTimeStr(const string &path, const string &format) {
    time_t       time       = fileLastWriteTime(path);
    tm          *timeStruct = localtime(&time);
    stringstream buffer;
    buffer << put_time(timeStruct, format.c_str());
    return buffer.str();
  }

  size_t fileSize(const string &path) {
    return fs::exists(path) ? fs::file_size(path) : 0;
  }

  void writeStrVecToFile(const string &path, const vector<string> &lines) {
    ofstream file(path);
    if (!file.is_open()) {
      throw runtime_error("Failed to open file: " + path);
    }
    for (const string &line : lines) {
      if (line.back() != '\n') {
        file << line << '\n';
      }
      else {
        file << line;
      }
    }
    file.close();
  }

  bool isSymlink(const string &path) {
    return fs::is_symlink(path);
  }

  string canonicalPath(const string &path) {
    return fs::canonical(path).string();
  }

  void copyFile(const string &sourcePath, const string &destinationPath) {
    fs::copy_file(sourcePath, destinationPath);
  }

  void renameFile(const string &oldPath, const string &newPath) {
    fs::rename(oldPath, newPath);
  }

  void createHardLink(const string &sourcePath, const string &destinationPath) {
    fs::create_hard_link(sourcePath, destinationPath);
  }

  void createSymlink(const string &sourcePath, const string &destinationPath) {
    fs::create_symlink(sourcePath, destinationPath);
  }

  void createDirectorySymlink(const string &sourcePath, const string &destinationPath) {
    fs::create_directory_symlink(sourcePath, destinationPath);
  }

  void setPermissions(const string &path, fs::perms permissions) {
    fs::permissions(path, permissions);
  }

  fs::perms getPermissions(const string &path) {
    return fs::status(path).permissions();
  }

  bool doesFileExistInDirs(const string &name, const vector<string> &dirPathsVec, const string &fileExtention) {
    for (const auto &path : dirPathsVec) {
      if (fs::exists(path) && fs::is_directory(path)) {
        for (const auto &entry : fs::directory_iterator(path)) {
          if (entry.is_regular_file() && (entry.path().extension() == fileExtention || fileExtention.empty())) {
            if (entry.path().filename().string().find(name) != string::npos) {
              return true;
            }
          }
        }
      }
    }
    return false;
  }

  bool file_size(unsigned long *size, FILE *file) {
    int prev = ftell(file);
    if (prev == -1) {
      return false;
    }
    fseek(file, 0L, SEEK_END);
    *size = ftell(file);
    if (*size == -1) {
      return false;
    }
    fseek(file, prev, SEEK_SET);
    return true;
  }

  size_t retrieve_file_size_from_path(const char *file_path) {
    FILE  *file = fopen(file_path, "rb");
    size_t size;
    if (!file_size(&size, file)) {
      fprintf(stderr, "%s: Failed to retrieve file size for file: [%s].\n", __func__, file_path);
      return (size_t)-1;
    }
    return size;
  }

  FILE *write_to_tmp_file(const void *buf, Ulong *bytes) {
    Ulong data_len, written_len;
    FILE *file;
    if ((file = tmpfile64()) == nullptr) {
      fatal_err("tmpfile64");
    }
    data_len = bytes ? *bytes == RETRIEVE_SIZE ? strlen((const char *)buf) : *bytes : strlen((const char *)buf);
    if ((written_len = fwrite(buf, 1, data_len, file)) != data_len) {
      fatal_err("fwrite");
    }
    bytes ? *bytes = written_len : 0;
    return file;
  }

  int write_to_file(const void *buf, Ulong *bytes, const char *file) {
    int   file_fd;
    long  written_bytes;
    Ulong data_len;
    if ((file_fd = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) == -1) {
      ferr("open");
    }
    data_len = bytes ? *bytes == RETRIEVE_SIZE ? strlen((const char *)buf) : *bytes : strlen((const char *)buf);
    if ((written_bytes = write(file_fd, buf, data_len)) != data_len) {
      close(file_fd);
      ferr("write");
    }
    bytes ? *bytes = written_bytes : 0;
    close(file_fd);
    return 0;
  }

  void write_to_ffile(char *buf, const char *file) {
    FILE *f;
    if (!(f = fopen(file, "wb"))) {
      ferr("fopen");
    }
    fwrite(buf, sizeof(char), strlen(buf), f);
    fclose(f);
  }

  char **dir_content(const char *path) {
    int     i, j, c_size, e_len;
    DIR    *d;
    dirent *entry;
    char  **buf, **t_buf;
    if ((d = opendir(path)) == nullptr) {
      nerre("opendir", "Failed to retrieve dir entrys in: ['%s'].\n", path);
      return nullptr;
    }
    c_size = 10;
    if ((buf = (char **)malloc(sizeof(char *) * c_size)) == nullptr) {
      closedir(d);
      ferr("malloc");
    }
    for (i = 0; (entry = readdir(d)) != nullptr; i++) {
      if (i == c_size) {
        c_size *= 2;
        if ((t_buf = (char **)realloc(buf, sizeof(char *) * c_size)) == nullptr) {
          for (j = 0; j < i - 1; j++) {
            free(buf[i]);
            buf[i] = nullptr;
          }
          free(buf);
          buf = nullptr;
          closedir(d);
          ferr("realloc");
        }
        buf = t_buf;
      }
      e_len = strlen(entry->d_name);
      if ((buf[i] = (char *)malloc(e_len + 1)) == nullptr) {
        for (j = 0; j < i - 1; j++) {
          free(buf[j]);
          buf[j] = nullptr;
        }
        free(buf);
        buf = nullptr;
        closedir(d);
        ferr("malloc");
      }
      memcpy(buf[i], entry->d_name, e_len);
      buf[i][e_len] = '\0';
    }
    buf[i] = nullptr;
    closedir(d);
    return buf;
  }

  void dir_content_free(char **buf) {
    unsigned long i;
    for (i = 0; buf[i] != nullptr; i++) {
      free(buf[i]);
    }
    free(buf);
  }

  void g_fsize(FILE *f, Ulong *size) {
    fseek(f, 0, SEEK_END);
    *size = ftell(f);
    fseek(f, 0, SEEK_SET);
  }
}
