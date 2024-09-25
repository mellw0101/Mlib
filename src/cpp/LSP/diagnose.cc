#include "LSP/language_server_protocol.hpp"

#include "Debug.h"
#include "def.h"

#include <clang-c/CXDiagnostic.h>
#include <clang-c/CXString.h>
#include <clang-c/Index.h>
#include <clang/Index/IndexSymbol.h>

// Callback to process declarations
void
Lsp::_decl_callback(CXClientData client_data, const CXIdxDeclInfo *decl_info)
{
    CXCursor cursor = decl_info->cursor;

    switch (decl_info->entityInfo->kind)
    {
        case CXIdxEntity_Function :
        {
            CXString name = clang_getCursorSpelling(cursor);
            logI("Function: %s", clang_getCString(name));

            // Get return type
            CXType   return_type     = clang_getCursorResultType(cursor);
            CXString return_type_str = clang_getTypeSpelling(return_type);
            logI("  Return type: %s", clang_getCString(return_type_str));
            clang_disposeString(return_type_str);

            // Get function arguments
            int num_args = clang_Cursor_getNumArguments(cursor);
            logI("  Parameters (%d):", num_args);
            for (int i = 0; i < num_args; ++i)
            {
                CXCursor arg_cursor   = clang_Cursor_getArgument(cursor, i);
                CXString arg_name     = clang_getCursorSpelling(arg_cursor);
                CXType   arg_type     = clang_getCursorType(arg_cursor);
                CXString arg_type_str = clang_getTypeSpelling(arg_type);

                logI("    Arg %d: %s (%s)", i, clang_getCString(arg_name),
                     clang_getCString(arg_type_str));

                clang_disposeString(arg_name);
                clang_disposeString(arg_type_str);
            }

            clang_disposeString(name);
            break;
        }
        case CXIdxEntity_CXXClass :
        {
            CXString name = clang_getCursorSpelling(cursor);
            logI("Class: %s", clang_getCString(name));

            // Optionally: Visit children to get class members (fields and methods)
            clang_visitChildren(
                cursor,
                [](CXCursor c, CXCursor parent, CXClientData client_data) {
                    if (clang_getCursorKind(c) == CXCursor_FieldDecl)
                    {
                        CXString field_name     = clang_getCursorSpelling(c);
                        CXType   field_type     = clang_getCursorType(c);
                        CXString field_type_str = clang_getTypeSpelling(field_type);
                        logI("  Field: %s (%s)", clang_getCString(field_name),
                             clang_getCString(field_type_str));
                        clang_disposeString(field_name);
                        clang_disposeString(field_type_str);
                    }
                    return CXChildVisit_Continue;
                },
                nullptr);

            clang_disposeString(name);
            break;
        }
        case CXIdxEntity_Struct :
        {
            CXString name = clang_getCursorSpelling(cursor);
            logI("Struct: %s", clang_getCString(name));
            // Optionally: Visit children to get struct members
            clang_visitChildren(
                cursor,
                [](CXCursor c, CXCursor parent, CXClientData client_data) {
                    if (clang_getCursorKind(c) == CXCursor_FieldDecl)
                    {
                        CXString field_name     = clang_getCursorSpelling(c);
                        CXType   field_type     = clang_getCursorType(c);
                        CXString field_type_str = clang_getTypeSpelling(field_type);
                        logI("  Field: %s (%s)", clang_getCString(field_name),
                             clang_getCString(field_type_str));
                        clang_disposeString(field_name);
                        clang_disposeString(field_type_str);
                    }
                    return CXChildVisit_Continue;
                },
                nullptr);

            clang_disposeString(name);
            break;
        }
        case CXIdxEntity_Variable :
        {
            CXString name         = clang_getCursorSpelling(cursor);
            CXType   var_type     = clang_getCursorType(cursor);
            CXString var_type_str = clang_getTypeSpelling(var_type);
            logI("Variable: %s (%s)", clang_getCString(name), clang_getCString(var_type_str));
            clang_disposeString(var_type_str);
            clang_disposeString(name);
            break;
        }
        default :
            break;
    }
}

void
Lsp::_index_entity_reference(CXClientData client_data, const CXIdxEntityRefInfo *ref_info)
{}

int
Lsp::index_file(const char *file)
{
    // Create a Clang index
    CXIndex index = clang_createIndex(0, 0);
    // Parse the translation unit (the source file)
    CXTranslationUnit translation_unit = clang_parseTranslationUnit(
        index, file, nullptr, 0, nullptr, 0, CXTranslationUnit_DetailedPreprocessingRecord);
    if (translation_unit == nullptr)
    {
        logE("Error parsing translation unit.");
        clang_disposeIndex(index);
        return 1;
    }
    // Create the index action
    CXIndexAction index_action = clang_IndexAction_create(index);
    // Run the indexer with our callbacks
    clang_indexTranslationUnit(
        index_action, nullptr, &index_callbacks, sizeof(index_callbacks), 0, translation_unit);
    // Clean up
    clang_disposeTranslationUnit(translation_unit);
    clang_disposeIndex(index);
    return 0;
}
