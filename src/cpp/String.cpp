#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <string>

#include "Attributes.h"
#include "String.h"
#include "def.h"

using std::pair;
using std::string;
using std::vector;

namespace Mlib::String
{
    unsigned long
    findN(const string &str, const string &search, unsigned long n)
    {
        unsigned long pos = 0;
        for (unsigned long i = 0; i < n; ++i)
        {
            pos = str.find(search, pos);
            if (pos == (unsigned long)-1)
            {
                return (unsigned long)-1;
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

    vector<pair<string, string>>
    parse_variables(const string &input)
    {
        vector<pair<string, string>> result;
        size_t                       pos = 0;
        while (pos != string::npos)
        {
            // Find the start of the type (skipping spaces)
            size_t type_start = input.find_first_not_of(" ", pos);
            if (type_start == string::npos)
            {
                break;
            }

            // Find the end of the type (before the first *, variable name or
            // comma)
            size_t type_end = input.find_first_of(",*;", type_start);
            while (type_end != std::string::npos && input[type_end] != ',' &&
                   input[type_end] != ';')
            {
                if (input[type_end] == '*')
                {
                    type_end = input.find_first_not_of("* ", type_end + 1);
                }
                else
                {
                    type_end = input.find_first_of(",*;", type_end);
                }
            }

            // Extract the type, which could be multi-word (e.g., "const char")
            string type = input.substr(type_start, type_end - type_start);

            pos = type_end;

            while (pos != string::npos && input[pos] != ';')
            {
                // Find the start of the variable name (skip commas and spaces)
                pos = input.find_first_not_of(",* ", pos);
                if (pos == string::npos || input[pos] == ';')
                {
                    break;
                }

                // Find the end of the variable name
                size_t name_end = input.find_first_of(",;", pos);
                string var_name = input.substr(pos, name_end - pos);

                // Check for pointers before the variable name
                size_t pointer_pos = input.find_first_of("*", type_end);
                string full_type   = type;
                if (pointer_pos != string::npos && pointer_pos < pos)
                {
                    full_type += " *";
                }

                // Store the variable name and its type
                result.push_back({full_type, var_name});
                pos = name_end;
            }

            // Move to the next section after a semicolon
            pos = input.find_first_of(";", pos);
            if (pos != string::npos)
            {
                pos++;
            }
        }

        return result;
    }

}

/* Constructor. */
MString::MString(const char *str)
{
    len  = strlen(str);
    cap  = (len) ? len * 2 : 30;
    data = (char *)malloc(cap);
    memmove(data, str, len);
    data[len] = '\0';
}

/* Length based constructor. */
MString::MString(const char *str, unsigned int slen)
{
    len  = slen;
    cap  = len * 2;
    data = (char *)malloc(cap);
    memmove(data, str, len);
    data[len] = '\0';
}

/* Copy constructor. */
MString::MString(const MString &other)
{
    len  = other.len;
    cap  = other.cap;
    data = (char *)malloc(cap);
    memmove(data, other.data, len);
    data[len] = '\0';
}

/* Move constructor. */
MString::MString(MString &&other) noexcept
{
    data       = other.data;
    len        = other.len;
    cap        = other.cap;
    other.data = NULL;
    other.len  = 0;
    other.cap  = 0;
}

/* Destructor. */
MString::~MString(void)
{
    free(data);
    data = NULL;
}

MString &
MString::operator=(const char *str)
{
    int newlen = strlen(str);
    if (newlen >= cap)
    {
        cap  = newlen * 2;
        data = (char *)realloc(data, cap);
    }
    len = newlen;
    memmove(data, str, len);
    data[len] = '\0';
    return *this;
}

MString &
MString::operator=(const MString &other)
{
    if (this != &other)
    {
        data ? free(data) : void();
        cap  = other.cap;
        len  = other.len;
        data = (char *)malloc(cap);
        memmove(data, other.data, len);
        data[len] = '\0';
    }
    return *this;
}

MString &
MString::operator+=(const MString &other)
{
    if (this != &other)
    {
        int newlen = len + other.len;
        if (newlen >= cap)
        {
            cap  = newlen * 2;
            data = (char *)realloc(data, cap);
        }
        memmove(data + len, other.data, other.len);
        len          = newlen;
        data[newlen] = '\0';
    }
    return *this;
}

MString
MString::operator+(const MString &other) const
{
    int     newlen = len + other.len;
    MString result;
    result.cap  = newlen * 2;
    result.len  = newlen;
    result.data = (char *)malloc(result.cap);
    memmove(result.data, data, len);
    memmove(result.data + len, other.data, other.len);
    result.data[newlen] = '\0';
    return result;
}

bool
MString::operator==(const char *str)
{
    if (strcmp(data, str) == 0)
    {
        return TRUE;
    }
    return FALSE;
}

char &
MString::operator[](unsigned int index)
{
    if (index > len)
    {
        return data[len];
    }
    return data[index];
}

const char &
MString::operator[](unsigned int index) const
{
    if (index > len)
    {
        return data[len];
    }
    return data[index];
}

const char *
MString::c_str(void) const
{
    return data;
}

Uint
MString::size(void)
{
    return len;
}

MString
MString_getenv(const char *str)
{
    const char *var = getenv(str);
    if (var == NULL)
    {
        return "";
    }
    return var;
}

Ulong __warn_unused __pure __no_debug __no_throw __no_null(1) mstrlen(const char *str) noexcept
{
    const char *cp = str;
    for (; *cp; ++cp);
    return (cp - str);
}

Ulong __warn_unused __pure __no_debug __no_throw __no_null(1)
    mstrnlen(const char *str, Ulong maxlen) noexcept
{
    const char *cp = str;
    for (; maxlen != 0 && *cp; ++cp, --maxlen);
    return (Ulong)(cp - str);
}

char *__warn_unused __pure __no_debug __no_throw __no_null(1)
    mstrndup(const char *str, Ulong maxlen) noexcept
{
    Ulong len  = mstrnlen(str, maxlen);
    char *copy = (char *)malloc(len + 1);
    if (copy)
    {
        memcpy(copy, str, len);
        copy[len] = '\0';
    }
    return copy;
}

/* clang-format off */
void __no_debug __no_throw __no_null(1, 2)
stack_str_ncpy(char *__restrict __dst,
               char *__restrict __src,
               Uint __n) noexcept
{
    for (Uint i = 0; i < __n; ++i) __dst[i] = __src[i];
}
/* clang-format on */
