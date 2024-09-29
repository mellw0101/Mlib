#pragma once

#include "../Flag.h"
#include "../Vector.h"

#include <string>

using std::string;

struct line_t
{
    string data;
    line_t *prev;
    line_t *next;
};

class MParse {
    enum flags_t
    {
        in_comment = 1,
        in_body    = 2
    };
    bit_flag_t<8> _flags;

public:
    void parse_source_file(const char *path);
};
