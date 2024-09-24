/* C++ code produced by gperf version 3.1 */
/* Command-line: gperf --language=C++ cpp_hash_map  */
/* Computed positions: -k'2-3' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) && ('%' == 37) && ('&' == 38) &&  \
      ('\'' == 39) && ('(' == 40) && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) && \
      ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) && ('1' == 49) && ('2' == 50) &&  \
      ('3' == 51) && ('4' == 52) && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) &&  \
      ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) && ('=' == 61) && ('>' == 62) &&  \
      ('?' == 63) && ('A' == 65) && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) &&  \
      ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) && ('J' == 74) && ('K' == 75) &&  \
      ('L' == 76) && ('M' == 77) && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) &&  \
      ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) && ('V' == 86) && ('W' == 87) &&  \
      ('X' == 88) && ('Y' == 89) && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) && \
      ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) && ('c' == 99) && ('d' == 100) && \
      ('e' == 101) && ('f' == 102) && ('g' == 103) && ('h' == 104) && ('i' == 105) &&            \
      ('j' == 106) && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) &&            \
      ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) && ('s' == 115) &&            \
      ('t' == 116) && ('u' == 117) && ('v' == 118) && ('w' == 119) && ('x' == 120) &&            \
      ('y' == 121) && ('z' == 122) && ('{' == 123) && ('|' == 124) && ('}' == 125) &&            \
      ('~' == 126))
/* The character set is not based on ISO-646.  */
#    error \
        "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#include <cstring>

#define TOTAL_KEYWORDS  46
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 9
#define MIN_HASH_VALUE  2
#define MAX_HASH_VALUE  75
/* maximum key range = 74, duplicates = 0 */

class HashMap
{
    static inline unsigned int hash(const char *str, size_t len);

public:
    static const char *
    find(const char *str, size_t len)
    {
        static const char *wordlist[] = {
            "",         "",         "do",        "for",      "long",    "const",     "sizeof",
            "",         "continue", "constexpr", "",         "struct",  "",          "volatile",
            "auto",     "",         "public",    "nullptr",  "int",     "void",      "using",
            "inline",   "if",       "unsigned",  "bool",     "",        "switch",    "",
            "noexcept", "TRUE",     "union",     "static",   "typedef", "typename",  "this",
            "while",    "return",   "",          "explicit", "char",    "short",     "extern",
            "",         "template", "case",      "false",    "",        "private",   "operator",
            "enum",     "break",    "",          "",         "",        "namespace", "FALSE",
            "",         "",         "",          "else",     "",        "",          "",
            "",         "true",     "",          "",         "",        "",          "NULL",
            "",         "",         "",          "",         "",        "class"};
        if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
        {
            unsigned int key = hash(str, len);

            if (key <= MAX_HASH_VALUE)
            {
                const char *s = wordlist[key];

                if (*str == *s && !strcmp(str + 1, s + 1))
                {
                    return s;
                }
            }
        }
        return 0;
    };
};

inline unsigned int
HashMap::hash(const char *str, size_t len)
{
    static unsigned char asso_values[] = {
        76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76,
        76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76,
        76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 15,
        76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 35, 76, 76, 76, 76, 15, 76, 76, 30, 10, 76,
        76, 76, 76, 76, 76, 76, 76, 76, 76, 35, 20, 5,  76, 25, 20, 76, 15, 0,  15, 76, 50, 5,
        10, 0,  20, 0,  25, 0,  5,  5,  35, 5,  30, 25, 76, 0,  76, 76, 76, 76, 76, 76, 76, 76,
        76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76,
        76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76,
        76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76,
        76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76,
        76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76,
        76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76};
    unsigned int hval = len;
    switch (hval)
    {
        default :
            hval += asso_values[static_cast<unsigned char>(str[2] + 1)];
        /*FALLTHROUGH*/
        case 2 :
            hval += asso_values[static_cast<unsigned char>(str[1])];
            break;
    }
    return hval;
}
