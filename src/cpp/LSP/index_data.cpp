#include "Debug.h"
#include "LSP/language_server_protocol.hpp"
#include "Profile.h"

#include <clang-c/CXSourceLocation.h>
#include <clang-c/Index.h>

void
main_file_tu::delete_data(void) noexcept
{
    for (const auto &it : functions)
    {
        delete it;
    }
    functions.resize(0);
}

/* clang-format off */
void
main_file_tu::destroy(void)
{
    delete_data();
    clang_disposeTranslationUnit(tu);
}

void
main_file_tu::do_index(void)
{
    PROFILE_FUNCTION;
    const char *clang_args[] {"-I/usr/lib/clang/18/include"};
    tu = clang_parseTranslationUnit(
        id->index,
        id->file_str.c_str(),
        clang_args,
        1,
        nullptr,
        0,
        CXTranslationUnit_PrecompiledPreamble |
        CXTranslationUnit_KeepGoing |
        CXTranslationUnit_SingleFileParse |
        CXTranslationUnit_IgnoreNonErrorsFromIncludedFiles
    );
    if (!tu)
    {
        logE("Failed to parse main file only.");
        return;
    }
    clang_indexTranslationUnit(
        id->ia,
        id,
        (IndexerCallbacks[1]) {{
            nullptr,
            diagnostic_callback,
            nullptr,
            ppIncludedFile,
            nullptr,
            nullptr,
            decl_callback,
            nullptr
        }},  
        64, 
        CXIndexOpt_SuppressRedundantRefs | CXIndexOpt_SkipParsedBodiesInSession,
        tu
    );
}

void
main_file_tu::reparse(void)
{
    delete_data();
    if (clang_reparseTranslationUnit(tu, 0, nullptr, 0) != 0)
    {
        logE("Failed to reparse file: '%s'.", id->file_str.c_str());
        clang_disposeTranslationUnit(tu);
        return;
    }
    clang_indexTranslationUnit(
        id->ia,
        id,
        (IndexerCallbacks[]) {{
            nullptr,
            diagnostic_callback,
            nullptr,
            ppIncludedFile,
            nullptr,
            nullptr,
            decl_callback,
            nullptr
        }},
        64,
        CXIndexOpt_SuppressRedundantRefs | CXIndexOpt_SkipParsedBodiesInSession, /* Skip bodies to speed up reparse */
        tu
    );
}
/* clang-format on */

void
index_data::delete_data(void) noexcept
{
    for (const auto &it : functions)
    {
        delete it;
    }
    functions.resize(0);
    for (const auto &it : classes)
    {
        delete it;
    }
    classes.resize(0);
    for (const auto &it : structs)
    {
        delete it;
    }
    structs.resize(0);
    for (const auto &it : variabels)
    {
        delete it;
    }
    variabels.resize(0);
}
