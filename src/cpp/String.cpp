#include "../include/String.h"
using namespace std;

namespace Mlib::String
{
    u64
    findN(const string &str, const string &search, u64 n)
    {
        u64 pos = 0;
        for (u64 i = 0; i < n; ++i)
        {
            pos = str.find(search, pos);
            if (pos == string::npos)
            {
                return string::npos;
            }
            pos += search.length();
        }
        return pos - search.length();
    }

    string
    replaceAll(const string &str, const string &search, const string &replace)
    {
        string result = str;
        size_t pos    = 0;
        while ((pos = result.find(search, pos)) != string::npos)
        {
            result.replace(pos, search.length(), replace);
            pos += replace.length();
        }
        return result;
    }

    string
    replaceN(const string &str, const string &search, const string &replace, size_t n)
    {
        string result = str;
        size_t pos    = 0;
        for (size_t i = 0; i < n; ++i)
        {
            pos = result.find(search, pos);
            if (pos == string::npos)
            {
                return result;
            }
            result.replace(pos, search.length(), replace);
            pos += replace.length();
        }
        return result;
    }
} // namespace Mlib::String
