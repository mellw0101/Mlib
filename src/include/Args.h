#pragma once

#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

#include "def.h"


namespace Mlib ::Args
{
    /// @name argvToStrVec
    /// @brief Converts the command line arguments to a vector of strings.
    /// @param argc The number of arguments.
    /// @param argv The arguments.
    /// @returns vector<string>
    std::vector<std::string> argvToStrVec(int argc, char **argv);

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
    std::string flagValue(const std::string &flag, int argC, char **argV);

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
    bool flagExists(const std::string &flag, int argC, char **argV);

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
    std::vector<std::string> flagValuesToStrVec(std::vector<std::string> &flagVec, int argC, char **argV);

    /// @name strVecFromStr
    /// @brief
    /// - Converts a string to a vector of strings.
    /// @param str
    /// - The string.
    /// @param delim
    /// - The delimiter.
    /// @returns vector<string>
    std::vector<std::string> strVecFromStr(const std::string &str, char delim);

    /// @name strFromStrVec
    /// @brief
    /// - Converts a vector of strings to a single string.
    /// @param strVec
    /// - The vector of strings.
    /// @returns string
    std::string strFromStrVec(const std::vector<std::string> &strVec);

    template <typename T>
    void
    eraseFromVector(std::vector<T> &vec, const T &objToErase)
    {
        vec.erase(std::remove(vec.begin(), vec.end(), objToErase), vec.end());
    }

    //
    //  Helper function to split a command str into a tokens and create a vector<string_views>
    //
    std::vector<std::string_view> tokenizeCommand(std::string_view command);

    //
    //  Function to construct the argument list for the execv function.
    //  This function also adds a filename to the argument list.
    //
    void constructArgumentListWFile(char **&arguments, std::string_view command, std::string_view filename);

}; // namespace Mlib::Args
