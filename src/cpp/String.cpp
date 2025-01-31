#include "../include/Attributes.h"
#include "../include/String.h"
#include "../include/def.h"

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <string>

using std::pair;
using std::string;
using std::vector;

namespace Mlib::String {
  Ulong findN(const string &str, const string &search, Ulong n) {
    Ulong pos = 0;
    for (Ulong i = 0; i < n; ++i) {
      pos = str.find(search, pos);
      if (pos == (Ulong)-1) {
        return (Ulong)-1;
      }
      pos += search.length();
    }
    return pos - search.length();
  }

  string replaceAll(const string &str, const string &search, const string &replace) {
    string result = str;
    size_t pos    = 0;
    while ((pos = result.find(search, pos)) != string::npos) {
      result.replace(pos, search.length(), replace);
      pos += replace.length();
    }
    return result;
  }

  string replaceN(const string &str, const string &search, const string &replace, size_t n) {
    string result = str;
    size_t pos    = 0;
    for (size_t i = 0; i < n; ++i) {
      pos = result.find(search, pos);
      if (pos == string::npos) {
        return result;
      }
      result.replace(pos, search.length(), replace);
      pos += replace.length();
    }
    return result;
  }

  vector<pair<string, string>> parse_variables(const string &input) {
    vector<pair<string, string>> result;
    size_t                       pos = 0;
    while (pos != string::npos) {
      // Find the start of the type (skipping spaces)
      size_t type_start = input.find_first_not_of(" ", pos);
      if (type_start == string::npos) {
        break;
      }

      // Find the end of the type (before the first *, variable name or
      // comma)
      size_t type_end = input.find_first_of(",*;", type_start);
      while (type_end != std::string::npos && input[type_end] != ',' && input[type_end] != ';') {
        if (input[type_end] == '*') {
          type_end = input.find_first_not_of("* ", type_end + 1);
        }
        else {
          type_end = input.find_first_of(",*;", type_end);
        }
      }

      // Extract the type, which could be multi-word (e.g., "const char")
      string type = input.substr(type_start, type_end - type_start);

      pos = type_end;

      while (pos != string::npos && input[pos] != ';') {
        // Find the start of the variable name (skip commas and spaces)
        pos = input.find_first_not_of(",* ", pos);
        if (pos == string::npos || input[pos] == ';') {
          break;
        }

        // Find the end of the variable name
        size_t name_end = input.find_first_of(",;", pos);
        string var_name = input.substr(pos, name_end - pos);

        // Check for pointers before the variable name
        size_t pointer_pos = input.find_first_of("*", type_end);
        string full_type   = type;
        if (pointer_pos != string::npos && pointer_pos < pos) {
          full_type += " *";
        }

        // Store the variable name and its type
        result.push_back({full_type, var_name});
        pos = name_end;
      }

      // Move to the next section after a semicolon
      pos = input.find_first_of(";", pos);
      if (pos != string::npos) {
        pos++;
      }
    }

    return result;
  }

}

/* Copy constructor. */
// MString::MString(const MString &other) {
//   _len  = other._len;
//   _cap  = other._cap;
//   _data = (char *)malloc(_cap);
//   memmove(_data, other._data, _len);
//   _data[_len] = '\0';
// }

// MString &MString::operator=(const MString &other) {
//   if (this != &other) {
//     _data ? free(_data) : void();
//     _cap  = other._cap;
//     _len  = other._len;
//     _data = (char *)malloc(_cap);
//     memmove(_data, other._data, _len);
//     _data[_len] = '\0';
//   }
//   return *this;
// }


MString MString_getenv(const char *str) {
  const char *var = getenv(str);
  if (var == NULL) {
    return "";
  }
  return var;
}

Ulong mstrlen(const char *str) noexcept {
  const char *cp = str;
  for (; *cp; ++cp);
  return (cp - str);
}

Ulong mstrnlen(const char *str, Ulong maxlen) noexcept {
  const char *cp = str;
  for (; maxlen != 0 && *cp; ++cp, --maxlen);
  return (Ulong)(cp - str);
}

char *mstrndup(const char *str, Ulong maxlen) noexcept {
  Ulong _len  = mstrnlen(str, maxlen);
  char *copy = (char *)malloc(_len + 1);
  if (copy) {
    memcpy(copy, str, _len);
    copy[_len] = '\0';
  }
  return copy;
}

/* clang-format off */
void stack_str_ncpy(char *__restrict __dst, char *__restrict __src, Uint __n) noexcept {
  for (Uint i = 0; i < __n; ++i) __dst[i] = __src[i];
}
/* clang-format on */
