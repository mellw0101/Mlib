/// @file String.h
#pragma once

#include "def.h"

#include <stdarg.h>
#include <string.h>
#include <string>

using std::pair;
using std::string;
using std::vector;

namespace Mlib::String
{
    size_t findN(const string &str, const string &search, size_t n);
    string replaceAll(const string &str, const string &search,
                      const string &replace);
    string replaceN(const string &str, const string &search,
                    const string &replace, size_t n);
    vector<pair<string, string>> parse_variables(const string &input);

} /* namespace Mlib::String */

class MString
{
    char        *data = NULL;
    unsigned int len;
    unsigned int cap;

public:
    MString(const char *str = "");
    MString(const char *str, unsigned int slen);
    MString(const MString &other);
    MString(MString &&other) noexcept;
    ~MString(void);
    MString &operator=(const char *str);
    MString &operator=(const MString &other);
    MString &operator+=(const MString &other);
    MString  operator+(const MString &other) const;

    bool         operator==(const char *str);
    char        &operator[](unsigned int index);
    const char  &operator[](unsigned int index) const;
    const char  *c_str(void) const;
    unsigned int size(void);
};

#define Mstring MString
