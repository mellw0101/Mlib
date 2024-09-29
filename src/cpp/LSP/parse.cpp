#include "LSP/parse.hpp"
#include "Debug.h"
#include "Profile.h"
#include "def.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdio.h>

void
MParse::parse_source_file(const char *path)
{
    PROFILE_FUNCTION;
    /* Open file path with flags read bytes */
    FILE *f = fopen(path, "rb");
    if (!f)
    {
        return;
    }
    _flags.clear();
    static char *buf;
    Ulong size;
    long len;
    line_t *head = nullptr;
    line_t *tail = nullptr;
    while ((len = getline(&buf, &size, f)) > 0)
    {
        if (buf[len - 1] == '\n')
        {
            buf[--len] = '\0';
        }
        line_t *line = (line_t *)malloc(sizeof(*tail));
        line->data = buf;
        line->next = nullptr;
        line->prev = tail;
        if (tail == nullptr)
        {
            head = line;
            tail = line;
        }
        tail->next = line;
        tail = tail->next;
    }
    fclose(f);
    for (line_t *l = head; l; l = l->next)
    {
        NLOG("%s\n", l->data.c_str());
    }
}
