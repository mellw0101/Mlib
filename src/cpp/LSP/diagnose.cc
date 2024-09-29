#include "Flag.h"
#include "LSP/language_server_protocol.hpp"

#include "Debug.h"
#include "Profile.h"
#include "Vector.h"
#include "def.h"

#include <clang-c/CXDiagnostic.h>
#include <clang-c/CXFile.h>
#include <clang-c/CXSourceLocation.h>
#include <clang-c/CXString.h>
#include <clang-c/Index.h>
#include <clang/Index/IndexSymbol.h>
#include <cstdio>
#include <cstdlib>
#include <new>
#include <string>

CXIdxClientFile
ppIncludedFile(CXClientData client_data, const CXIdxIncludedFileInfo *included_file)
{
    if (included_file && included_file->file)
    {
        PROFILE_FUNCTION;
        CXString included_file_name = clang_getFileName(included_file->file);
        logI("Included file: %s", clang_getCString(included_file_name));
        // NLOG("%s\n", clang_getCString(included_file_name));
        clang_disposeString(included_file_name);
    }
    return nullptr;
}

CXIdxClientFile
entered_main_file(CXClientData client_data, CXFile main_file, void *reserved)
{
    return nullptr;
}

void
diagnostic_callback(CXClientData client_data, CXDiagnosticSet diagnostics, void *reserved)
{
    Uint num_diagnostics = clang_getNumDiagnosticsInSet(diagnostics);
    for (Uint i = 0; i < num_diagnostics; ++i)
    {
        CXDiagnostic diag = clang_getDiagnosticInSet(diagnostics, i);
        /* Retrieve the diagnostic severity (error, warning, note, etc.) */
        CXDiagnosticSeverity severity     = clang_getDiagnosticSeverity(diag);
        const char          *severity_str = nullptr;
        switch (severity)
        {
            case CXDiagnostic_Ignored :
                severity_str = "Ignored";
                break;
            case CXDiagnostic_Note :
                severity_str = "Note";
                break;
            case CXDiagnostic_Warning :
                severity_str = "Warning";
                break;
            case CXDiagnostic_Error :
                severity_str = "Error";
                break;
            case CXDiagnostic_Fatal :
                severity_str = "Fatal Error";
                break;
            default :
                severity_str = "Unknown Severity";
                break;
        }
        /* Get diagnostic message */
        CXString diagnostic_string =
            clang_formatDiagnostic(diag, clang_defaultDiagnosticDisplayOptions());
        const char *diagnostic_msg = clang_getCString(diagnostic_string);
        /* Print or log the diagnostic message with its severity */
        logI("[%s] %s.", severity_str, diagnostic_msg);
        /* Dispose of the CXString to avoid memory leaks */
        clang_disposeString(diagnostic_string);
        /* Optionally, get the source location for the diagnostic */
        CXSourceLocation location = clang_getDiagnosticLocation(diag);
        CXFile           file;
        Uint             line, column;
        clang_getSpellingLocation(location, &file, &line, &column, nullptr);
        if (file)
        {
            CXString    filename     = clang_getFileName(file);
            const char *filename_str = clang_getCString(filename);
            logI("  In file: %s at line %u, column %u.", filename_str, line, column);
            clang_disposeString(filename);
        }
        /* Dispose of the diagnostic after use */
        clang_disposeDiagnostic(diag);
    }
}

/* Callback to process declarations */
void
decl_callback(CXClientData client_data, const CXIdxDeclInfo *decl_info)
{
    index_data *data   = (index_data *)client_data;
    CXCursor    cursor = decl_info->cursor;
    switch (decl_info->entityInfo->kind)
    {
        case CXIdxEntity_Function :
        {
            function_decl *decl = new function_decl();
            decl->name          = Lsp::utils::get_cursor_name(cursor);
            decl->return_type   = Lsp::utils::get_cursor_return_type(cursor);
            decl->file =
                Lsp::utils::get_file_name_and_cursor_range(cursor, &decl->start, &decl->end);
            Lsp::utils::get_cursor_pos(cursor, &decl->pos);
            /* Get function params. */
            int num_args   = clang_Cursor_getNumArguments(cursor);
            decl->num_args = num_args;
            for (int i = 0; i < num_args; ++i)
            {
                CXCursor arg_cursor = clang_Cursor_getArgument(cursor, i);
                decl->params.push_back({
                    Lsp::utils::get_cursor_type(arg_cursor),
                    Lsp::utils::get_cursor_name(arg_cursor),
                    arg_cursor,
                });
            }
            for (auto &it : decl->params)
            {
                Lsp::utils::get_cursor_pos(it.cursor, &it.pos);
                Lsp::utils::get_cursor_start_pos(it.cursor, &it.s);
                it.e = decl->end;
            }
            decl->cursor = cursor;
            decl->TU     = clang_Cursor_getTranslationUnit(cursor);
            decl->F      = Lsp::utils::get_cxfile(cursor);
            decl->doc    = Lsp::utils::get_cursor_breif_comment_string(cursor);
            if (clang_Location_isFromMainFile(clang_getCursorLocation(cursor)))
            {
                decl->flags.set<MAIN_FILE>();
                clang_visitChildren(
                    cursor,
                    [](CXCursor c, CXCursor parent,
                       CXClientData client_data) -> CXChildVisitResult {
                        function_decl *decl = (function_decl *)client_data;
                        switch (clang_getCursorKind(c))
                        {
                            case CXCursor_VarDecl :
                            {
                                function_variable_decl var;
                                Lsp::utils::get_cursor_pos(c, &var.pos);
                                Lsp::utils::get_cursor_start_pos(c, &var.s);
                                Lsp::utils::get_cursor_end_pos(
                                    clang_getCursorSemanticParent(c), &var.e);
                                decl->body.vars.push_back({
                                    Lsp::utils::get_cursor_type(c),
                                    Lsp::utils::get_cursor_name(c),
                                    c,
                                    var.pos,
                                    var.s,
                                    var.e,
                                });
                            }
                            default :
                                break;
                        }
                        return CXChildVisit_Recurse;
                    },
                    decl);
                data->main.functions.push_back(decl);
            }
            else
            {
                data->functions.push_back(decl);
            }
            break;
        }
        case CXIdxEntity_CXXClass :
        {
            class_decl *decl = new class_decl();
            decl->name       = Lsp::utils::get_cursor_name(cursor);
            decl->file =
                Lsp::utils::get_file_name_and_cursor_range(cursor, &decl->start, &decl->end);
            decl->inside_namespace = Lsp::utils::get_cursor_namespace_name(cursor);
            if (clang_Location_isFromMainFile(clang_getCursorLocation(cursor)))
            {
                decl->flags.set<MAIN_FILE>();
            }
            /* Visit children to get class members (fields and methods) */
            clang_visitChildren(
                cursor,
                [](CXCursor c, CXCursor parent, CXClientData client_data) -> CXChildVisitResult {
                    class_decl *decl = (class_decl *)client_data;
                    switch (clang_getCursorKind(c))
                    {
                        // Handle fields (already implemented)
                        case CXCursor_FieldDecl :
                        {
                            variable_decl var;
                            var.type = Lsp::utils::get_cursor_type(c);
                            var.name = Lsp::utils::get_cursor_name(c);
                            var.file = Lsp::utils::get_file_name_and_cursor_position(c, &var.pos);
                            decl->variabels.push_back({var.type, var.name, var.file, var.pos});
                            decl->variabels.back().flags.set(
                                Lsp::utils::get_access_specifier_opt(c));
                            break;
                        }
                        case CXCursor_CXXMethod :
                        case CXCursor_FunctionDecl :
                        {
                            function_decl method;
                            method.name = Lsp::utils::get_cursor_name(c);
                            Lsp::utils::get_cursor_pos(c, &method.pos);
                            method.return_type = Lsp::utils::get_cursor_return_type(c);
                            method.params =
                                Lsp::utils::get_cursor_function_args(c, &method.num_args);
                            // Add method to class decl.
                            decl->methods.push_back({decl->file,
                                                     method.pos,
                                                     decl->start,
                                                     decl->end,
                                                     method.name,
                                                     method.return_type,
                                                     method.num_args,
                                                     {},
                                                     method.params});
                            decl->methods.back().flags.set(Lsp::utils::get_access_specifier_opt(c));
                            break;
                        }
                        default :
                        {
                            break;
                        }
                    }
                    return CXChildVisit_Continue;
                },
                decl);
            data->classes.push_back(decl);
            break;
        }
        case CXIdxEntity_Struct :
        {
            if (!clang_equalCursors(cursor, clang_getCanonicalCursor(cursor)))
            {
                break;
            }
            struct_decl *decl = new struct_decl();
            decl->name        = Lsp::utils::get_cursor_name(cursor);
            decl->file =
                Lsp::utils::get_file_name_and_cursor_range(cursor, &decl->start, &decl->end);
            decl->inside_namespace = Lsp::utils::get_cursor_namespace_name(cursor);
            if (clang_Location_isFromMainFile(clang_getCursorLocation(cursor)))
            {
                decl->flags.set<MAIN_FILE>();
            }
            /* Visit children to get class members (fields and methods) */
            clang_visitChildren(
                cursor,
                [](CXCursor c, CXCursor parent, CXClientData client_data) -> CXChildVisitResult {
                    struct_decl *decl = (struct_decl *)client_data;
                    switch (clang_getCursorKind(c))
                    {
                        // Handle fields (already implemented)
                        case CXCursor_FieldDecl :
                        {
                            variable_decl var;
                            var.type = Lsp::utils::get_cursor_type(c);
                            var.name = Lsp::utils::get_cursor_name(c);
                            var.file = Lsp::utils::get_file_name_and_cursor_position(c, &var.pos);
                            decl->variabels.push_back({var.type, var.name, var.file, var.pos});
                            decl->variabels.back().flags.set(
                                Lsp::utils::get_access_specifier_opt(c));
                            break;
                        }
                        case CXCursor_CXXMethod :
                        case CXCursor_FunctionDecl :
                        {
                            function_decl method;
                            // Get method name
                            method.name = Lsp::utils::get_cursor_name(c);
                            // Get return type
                            method.return_type = Lsp::utils::get_cursor_return_type(c);
                            // Get function arguments
                            method.params =
                                Lsp::utils::get_cursor_function_args(c, &method.num_args);
                            Lsp::utils::get_cursor_pos(c, &method.pos);
                            // Add method to class decl.
                            decl->methods.push_back({
                                decl->file,
                                method.pos,
                                decl->start,
                                decl->end,
                                method.name,
                                method.return_type,
                                method.num_args,
                                {},
                                method.params,
                            });
                            decl->methods.back().flags.set(Lsp::utils::get_access_specifier_opt(c));
                            break;
                        }
                        default :
                        {
                            break;
                        }
                    }
                    return CXChildVisit_Continue;
                },
                decl);
            data->structs.push_back(decl);
            break;
        }
        case CXIdxEntity_Variable :
        {
            if (!clang_Location_isFromMainFile(clang_getCursorLocation(cursor)))
            {
                variable_decl *decl = new variable_decl();
                decl->name          = Lsp::utils::get_cursor_name(cursor);
                decl->type          = Lsp::utils::get_cursor_type(cursor);
                data->variabels.push_back(decl);
            }
            break;
        }
        default :
        {
            break;
        }
    }
}

/* Init Lsp::_instance to nullptr, this needs to be done as it`s a static decl. */
Lsp *Lsp::_instance = nullptr;

void
Lsp::_destroy(void) noexcept
{
    for (const auto &id : Lsp::instance().idata)
    {
        /* Delete all function_decl * accumelated. */
        for (const auto &f : id->functions)
        {
            delete f;
        }
        id->main.destroy();
        /* Delete all class_decl * accumelated. */
        for (const auto &c : id->classes)
        {
            delete c;
        }
        /* Delete all struct_decl * accumelated. */
        for (const auto &s : id->structs)
        {
            delete s;
        }
        for (const auto &v : id->variabels)
        {
            delete v;
        }
        clang_disposeTranslationUnit(id->tu);
        clang_IndexAction_dispose(id->ia);
        clang_disposeIndex(id->index);
        delete id;
    }
    delete Lsp::_instance;
}

index_data *
Lsp::get_file_index_data(const char *file, bool make_new_on_fail) noexcept
{
    index_data *data = nullptr;
    for (const auto &it : idata)
    {
        if (it->file_str == file)
        {
            data = it;
            break;
        }
    }
    if (data == nullptr)
    {
        if (!make_new_on_fail)
        {
            return nullptr;
        }
        data = new (std::nothrow) index_data();
        if (!data)
        {
            logE("Failed to alloc index data.");
            exit(1);
        }
        data->flags.set<index_data::IS_NEW>();
    }
    return data;
}

Lsp &
Lsp::instance(void) noexcept
{
    if (_instance == nullptr)
    {
        _instance = new (std::nothrow) Lsp();
        if (_instance == nullptr)
        {
            logE("new (std::nothrow) Failed to alloc Lsp.");
            exit(1);
        }
        atexit(Lsp::_destroy);
    }
    return *_instance;
}

/* clang-format off */
int
Lsp::index_file(const char *file)
{
    PROFILE_FUNCTION;
    index_data *id = get_file_index_data(file, true);
    if (!id->flags.is_set<index_data::IS_NEW>())
    {
        return 0;
    }
    id->file_str             = file;
    id->index                = clang_createIndex(0, 0);
    const char *clang_args[] = {"-I/usr/lib/clang/18/include"};
    /* Parse the translation unit (the source file) */
    id->tu = clang_parseTranslationUnit(
        id->index,
        file,
        clang_args,
        1,
        nullptr,
        0,
        (
            CXTranslationUnit_KeepGoing | CXTranslationUnit_DetailedPreprocessingRecord
        )
    );
    if (!id->tu)
    {
        logE("Error parsing translation unit. file: '%s'.", file);
        return 1;
    }
    id->file = clang_getFile(id->tu, file);
    /* Create the index action */
    id->ia = clang_IndexAction_create(id->index);
    /* Run the indexer with our callbacks */
    clang_indexTranslationUnit(id->ia, id, &_index_callbacks, sizeof(_index_callbacks), 0, id->tu);
    id->main.id = id;
    id->main.do_index();
    id->flags.unset<index_data::IS_NEW>();
    idata.push_back(id);
    return 0;
}
/* clang-format on */
