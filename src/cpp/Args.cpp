#include "../include/Args.h"

using namespace std;

namespace Mlib::Args
{
    vector<string>
    argvToStrVec(int argc, char **argv)
    {
        vector<string> args;
        for (int i = 0; i < argc; ++i)
        {
            args.push_back(argv[i]);
        }
        return args;
    }

    string
    argvToCmd(int argc, char **argv)
    {
        string cmd;
        for (int i = 0; i < argc; ++i)
        {
            cmd += argv[i];
            if (i < argc - 1)
            {
                cmd += " ";
            }
        }
        return cmd;
    }

    string
    flagValue(const string &flag, int argC, char **argV)
    {
        for (int i = 0; i < argC; ++i)
        {
            if (argV[i] == flag)
            {
                if (i + 1 < argC)
                {
                    return argV[i + 1];
                }
                else
                {
                    throw runtime_error("Flag '" + flag + "' has no value.");
                }
            }
        }
        throw runtime_error("Flag '" + flag + "' not found.");
    }

    bool
    flagExists(const string &flag, int argC, char **argV)
    {
        for (int i = 0; i < argC; ++i)
        {
            if (argV[i] == flag)
            {
                return true;
            }
        }
        return false;
    }

    vector<string>
    flagValuesToStrVec(vector<string> &flagVec, int argC, char **argV)
    {
        for (int i = 0; i < argC; ++i)
        {
            for (auto &flag : flagVec)
            {
                if (argV[i] == flag)
                {
                    if (i + 1 < argC)
                    {
                        flag = argV[i + 1];
                    }
                    else
                    {
                        throw runtime_error("Flag '" + flag + "' has no value.");
                    }
                }
            }
        }
        return flagVec;
    }

    vector<string>
    strVecFromStr(const string &str, char delim)
    {
        vector<string> args;
        for (size_t i = 0, start = 0; i < str.size(); ++i)
        {
            if (str[i] == delim)
            {
                string s1 = str.substr(start, i - start);
                args.push_back(s1);
                start = i + 1;
            }
            if (i == str.size() - 1)
            {
                string s2 = str.substr(start, i - start + 1);
                args.push_back(s2);
            }
        }
        return args;
    }

    string
    strFromStrVec(const vector<string> &strVec)
    {
        string str;
        for (size_t i = 0; i < strVec.size(); ++i)
        {
            str += strVec[i] + " ";
        }
        if (!str.empty())
        {
            str.pop_back();
        }
        return str;
    }

    vector<string_view>
    tokenizeCommand(string_view command)
    {
        vector<string_view> tokens;

        u64 start = 0;
        u64 end   = command.find(' ');

        while (end != std::string_view::npos)
        {
            if (end != start)
            {
                tokens.emplace_back(command.substr(start, end - start));
            }
            start = end + 1;
            end   = command.find(' ', start);
        }

        if (start < command.size())
        {
            tokens.emplace_back(command.substr(start));
        }

        return tokens;
    }

    void
    constructArgumentListWFile(s8 **&arguments, string_view command, string_view file_name)
    {
        auto tokens = tokenizeCommand(command);
        //
        //  +1 for filename, +1 for nullptr terminator
        //
        s32 count = tokens.size() + 2;
        //
        //  Allocate memory for the char* array
        //
        arguments = new char *[count];

        //
        //  Copy tokens to the char* array
        //
        u64 i = 0;
        for (; i < tokens.size(); ++i)
        {
            const auto &token = tokens[i];
            s8         *arg   = new s8[token.size() + 1];
            std::memcpy(arg, token.data(), token.size());
            arg[token.size()] = '\0';
            arguments[i]      = arg;
        }

        //
        //  Add the filename
        //
        s8 *file_arg = new s8[file_name.size() + 1];
        std::memcpy(file_arg, file_name.data(), file_name.size());
        file_arg[file_name.size()] = '\0';
        arguments[i++]             = file_arg;

        //
        //  Add the nullptr terminator
        //
        arguments[i] = nullptr;
    }

    // Function to clean up the allocated memory
    //
    //
    void
    freeArgumentList(s8 **arguments)
    {
        for (size_t i = 0; arguments[i] != nullptr; ++i)
        {
            delete[] arguments[i];
        }
        delete[] arguments;
    }

} // namespace Mlib::Args
