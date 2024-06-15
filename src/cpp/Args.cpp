#include "../include/Args.h"

namespace Mlib::Args {
    vector<string>
    argvToStrVec(int argc, char** argv)
    {
        vector<string> args;
        for (int i = 0; i < argc; ++i)
        {
            args.push_back(argv[i]);
        }
        return args;
    }

    string
    argvToCmd(int argc, char** argv)
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
    flagValue(const string& flag, int argC, char** argV)
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
    flagExists(const string& flag, int argC, char** argV)
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
    flagValuesToStrVec(vector<string>& flagVec, int argC, char** argV)
    {
        for (int i = 0; i < argC; ++i)
        {
            for (auto& flag : flagVec)
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
    strVecFromStr(const string& str, char delim)
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
    strFromStrVec(const vector<string>& strVec)
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
} // namespace Mlib::Args
