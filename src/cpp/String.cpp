#include "../include/String.h"

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
    replaceN(const string &str, const string &search, const string &replace,
             size_t n)
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

} // namespace Mlib::String

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

unsigned int
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

#define BOLLCHECKER_MAX_SIZE 100
namespace boolchecker
{
    int
    precedence(char *op)
    {
        if (strcmp(op, "!") == 0)
        {
            return 3;
        }
        else if (strcmp(op, "&&"))
        {
            return 2;
        }
        else if (strcmp(op, "||"))
        {
            return 1;
        }
        return 0;
    }

    /* Check if a token is an operator. */
    int
    is_operator(char *token)
    {
        return (strcmp(token, "!") == 0 || strcmp(token, "&&") == 0 ||
                strcmp(token, "||") == 0);
    }

    char **
    tokenize(char *expression, int *token_count)
    {
        char **tokens = AMALLOC_ARRAY(tokens, BOLLCHECKER_MAX_SIZE);
        char  *token  = strtok(expression, " ");
        int    count  = 0;
        while (token != NULL)
        {
            tokens[count] = (char *)malloc(strlen(token) + 1);
            strcpy(tokens[count++], token);
            token = strtok(NULL, " ");
        }
        *token_count = count;
        return tokens;
    }

    void
    push(Stack *s, char *item)
    {
        s->items[++(s->top)] = item;
    }

    char *
    pop(Stack *s)
    {
        return s->items[(s->top)--];
    }

    char *
    peek(Stack *s)
    {
        return s->items[s->top];
    }

    int
    is_empty(Stack *s)
    {
        return s->top == -1;
    }

    void
    infix_to_postfix(char **tokens, int token_count)
    {
        Stack operator_stack = {.top = -1};
        char *postfix[BOLLCHECKER_MAX_SIZE];
        int   postfix_count = 0;
        for (int i = 0; i < token_count; i++)
        {
            char *token = tokens[i];
            if (isalpha(token[0]))
            {
                postfix[postfix_count++] = token;
            }
            else if (strcmp(token, "(") == 0)
            {
                push(&operator_stack, token);
            }
            else if (strcmp(token, ")") == 0)
            {
                while (!is_empty(&operator_stack) &&
                       strcmp(peek(&operator_stack), "(") != 0)
                {
                    postfix[postfix_count++] = pop(&operator_stack);
                }
                pop(&operator_stack);
            }
            else if (is_operator(token))
            {
                while (!is_empty(&operator_stack) &&
                       precedence(peek(&operator_stack)))
                {
                    postfix[postfix_count++] = pop(&operator_stack);
                }
                push(&operator_stack, token);
            }
        }
        while (!is_empty(&operator_stack))
        {
            postfix[postfix_count++] = pop(&operator_stack);
        }
        printf("Postfix Expression (Order of Operations): ");
        for (int i = 0; i < postfix_count; i++)
        {
            printf("%s ", postfix[i]);
        }
        printf("\n\n");
        printf("Order of Evaluation:\n");
        Stack evel_stack = {.top = -1};
        for (int i = 0; i < postfix_count; i++)
        {
            char *token = postfix[i];
            if (isalpha(token[0]))
            {
                push(&evel_stack, token);
            }
            else
            {
                if (strcmp(token, "!") == 0)
                {
                    char *operand = pop(&evel_stack);
                    printf("Evaluating: !%s\n", operand);
                    push(&evel_stack, operand);
                }
                else
                {
                    char *right = pop(&evel_stack);
                    char *left  = pop(&evel_stack);
                    printf("Evaluating: %s %s %s", left, token, right);
                    push(&evel_stack, left);
                }
            }
        }
    }
} // namespace boolchecker
