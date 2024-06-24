#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include "def.h"
using namespace std;


namespace Mlib::Args
{
    /// @name argvToStrVec
    /// @brief Converts the command line arguments to a vector of strings.
    /// @param argc The number of arguments.
    /// @param argv The arguments.
    /// @returns vector<string>
    vector<string> argvToStrVec(int argc, char **argv);

    /// @name flagValue
    /// @brief
    /// - Gets the value of a flag from the command line arguments.
    /// @param flag
    /// - The flag.
    /// @param argC
    /// - The number of arguments.
    /// @param argV
    /// - The arguments.
    /// @returns string
    string flagValue(const string &flag, int argC, char **argV);

    /// @name flagExists
    /// @brief
    /// - Checks if a flag exists in the command line arguments.
    /// @param flag
    /// - The flag.
    /// @param argC
    /// - The number of arguments.
    /// @param argV
    /// - The arguments.
    /// @returns bool
    bool flagExists(const string &flag, int argC, char **argV);

    /// @name flagValuesToStrVec
    /// @brief
    /// - Converts a vector of flags to a vector of values.
    /// @param flagVec
    /// - The vector of flags.
    /// @param argC
    /// - The number of arguments.
    /// @param argV
    /// - The arguments.
    /// @returns vector<string>
    /// @note
    /// - The flags must be in the form "--flag=value".
    /// - The flag values replace the flags in the vector.
    vector<string> flagValuesToStrVec(vector<string> &flagVec, int argC, char **argV);

    /// @name strVecFromStr
    /// @brief
    /// - Converts a string to a vector of strings.
    /// @param str
    /// - The string.
    /// @param delim
    /// - The delimiter.
    /// @returns vector<string>
    vector<string> strVecFromStr(const string &str, char delim);

    /// @name strFromStrVec
    /// @brief
    /// - Converts a vector of strings to a single string.
    /// @param strVec
    /// - The vector of strings.
    /// @returns string
    string strFromStrVec(const vector<string> &strVec);

    template <typename T>
    void
    eraseFromVector(vector<T> &vec, const T &objToErase)
    {
        vec.erase(std::remove(vec.begin(), vec.end(), objToErase), vec.end());
    }

}; // namespace Mlib::Args
