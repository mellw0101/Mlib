/// @file String.h
#pragma once

#include "def.h"

#include <string>
using namespace std;


namespace Mlib::String {
    size_t findN(const string &str, const string &search, size_t n);
    string replaceAll(const string &str, const string &search, const string &replace);
    string replaceN(const string &str, const string &search, const string &replace, size_t n);
} // namespace Mlib::String
