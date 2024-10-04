// #include "Debug.h"
// #include "Flag.h"
// #include "LSP/language_server_protocol.hpp"

// #include "Profile.h"
// #include "Vector.h"
// #include "def.h"

// #include <clang-c/CXFile.h>
// #include <clang-c/CXSourceLocation.h>
// #include <clang-c/CXString.h>
// #include <clang-c/Index.h>
// #include <new>

// /* clang-format off */
// void
// Lsp::utils::get_cursor_start_pos(CXCursor c, decl_pos *dp)
// {
//     PROFILE_FUNCTION;
//     CXSourceRange    r = clang_getCursorExtent(c);
//     CXSourceLocation p = clang_getRangeStart(r);
//     clang_getFileLocation(p, nullptr, &dp->line, &dp->column, &dp->offset);
// }

// void
// Lsp::utils::get_cursor_end_pos(CXCursor c, decl_pos *dp)
// {
//     PROFILE_FUNCTION;
//     CXSourceRange    r = clang_getCursorExtent(c);
//     CXSourceLocation p = clang_getRangeEnd(r);
//     clang_getFileLocation(p, nullptr, &dp->line, &dp->column, &dp->offset);
// }

// void
// Lsp::utils::get_cursor_pos(CXCursor c, decl_pos *dp)
// {
//     CXSourceLocation loc = clang_getCursorLocation(c);
//     clang_getFileLocation(loc, nullptr, &dp->line, &dp->column, &dp->offset);
// }

// void
// Lsp::utils::get_cursor_range(CXCursor c, decl_pos *s, decl_pos *e)
// {
//     PROFILE_FUNCTION;
//     CXSourceRange    r  = clang_getCursorExtent(c);
//     CXSourceLocation ps = clang_getRangeStart(r);
//     clang_getFileLocation(ps, nullptr, &s->line, &s->column, &s->offset);
//     CXSourceLocation pe = clang_getRangeEnd(r);
//     clang_getFileLocation(pe, nullptr, &e->line, &e->column, &e->offset);
// }

// void
// Lsp::utils::get_function_variable_decl(CXCursor c, function_variable_decl *fvd)
// {
//     if (!fvd)
//     {
//         return;
//     }
//     fvd->cursor = c;
//     fvd->name   = get_cursor_name(c);
//     fvd->type   = get_cursor_type(c);
//     get_cursor_start_pos(c, &fvd->s);
//     CXCursor parent = clang_getCursorSemanticParent(c);
//     // Traverse up the cursor's parent hierarchy
//     while (!clang_Cursor_isNull(parent))
//     {
//         if (clang_getCursorKind(parent) == CXCursor_FunctionDecl)
//         {
//             get_cursor_end_pos(parent, &fvd->e);
//             break;
//         }
//     }
// }

// /* Get cursor namespace.  If any. */
// string
// Lsp::utils::get_cursor_namespace_name(CXCursor cursor)
// {
//     string   namespace_name = "";
//     CXCursor parent         = clang_getCursorSemanticParent(cursor);
//     // Traverse up the cursor's parent hierarchy
//     while (!clang_Cursor_isNull(parent))
//     {
//         // Check if the current parent is a namespace
//         if (clang_getCursorKind(parent) == CXCursor_Namespace)
//         {
//             if (clang_Cursor_isInlineNamespace(parent) || clang_Cursor_isAnonymous(parent))
//             {
//                 // Move to the next parent in the hierarchy
//                 parent = clang_getCursorSemanticParent(parent);
//                 continue;
//             }
//             // Get the name of the namespace
//             CXString name              = clang_getCursorSpelling(parent);
//             string   current_namespace = clang_getCString(name);
//             clang_disposeString(name);
//             // If there are nested namespaces, append the new one before the previous ones
//             if (!namespace_name.empty())
//             {
//                 namespace_name = current_namespace + "::" + namespace_name;
//             }
//             else
//             {
//                 namespace_name = current_namespace;
//             }
//         }
//         // Move to the next parent in the hierarchy
//         parent = clang_getCursorSemanticParent(parent);
//     }
//     return namespace_name;
// }

// MVector<function_variable_decl>
// Lsp::utils::get_cursor_function_args(CXCursor cursor, Uint *n)
// {
//     MVector<function_variable_decl> args;
//     // Get function arguments
//     int num_args = clang_Cursor_getNumArguments(cursor);
//     for (int i = 0; i < num_args; ++i)
//     {
//         CXCursor arg_cursor = clang_Cursor_getArgument(cursor, i);
//         args.push_back({get_cursor_type(arg_cursor), get_cursor_name(arg_cursor)});
//     }
//     *n = (num_args >= 0) ? num_args : 0;
//     return args;
// }

// /* Return`s a string containing the name of a cursor. */
// string
// Lsp::utils::get_cursor_name(CXCursor cursor)
// {
//     CXString name = clang_getCursorSpelling(cursor);
//     string   ret  = clang_getCString(name);
//     clang_disposeString(name);
//     return ret;
// }

// string
// Lsp::utils::get_cursor_kind_spelling(CXCursorKind kind)
// {
//     CXString cxstr = clang_getCursorKindSpelling(kind);
//     string ret = clang_getCString(cxstr);
//     clang_disposeString(cxstr);
//     return ret;
// }

// /* Return`s a string containing the type of a cursor. */
// string
// Lsp::utils::get_cursor_type(CXCursor cursor)
// {
//     CXType   type = clang_getCursorType(cursor);
//     CXString str  = clang_getTypeSpelling(type);
//     string   ret  = clang_getCString(str);
//     clang_disposeString(str);
//     return ret;
// }

// /* Return`s a string containing the return type of a cursor. */
// string
// Lsp::utils::get_cursor_return_type(CXCursor cursor)
// {
//     CXType   type = clang_getCursorResultType(cursor);
//     CXString str  = clang_getTypeSpelling(type);
//     string   ret  = clang_getCString(str);
//     clang_disposeString(str);
//     return ret;
// }

// string
// Lsp::utils::get_file_name_and_cursor_range(CXCursor cursor, decl_pos *start, decl_pos *end)
// {
//     // Function decl range.
//     CXSourceRange range = clang_getCursorExtent(cursor);
//     CXFile        file;
//     // Get start line, column and offset.
//     CXSourceLocation start_range = clang_getRangeStart(range);
//     clang_getFileLocation(start_range, &file, &start->line, &start->column, &start->offset);
//     // Get end line, column and offset.
//     CXSourceLocation end_range = clang_getRangeEnd(range);
//     clang_getFileLocation(end_range, &file, &end->line, &end->column, &end->offset);
//     // Extract file name.
//     CXString file_name = clang_getFileName(file);
//     string   name      = clang_getCString(file_name);
//     clang_disposeString(file_name);
//     return name;
// }

// /* Get file name and the cursor position in the file. */
// string
// Lsp::utils::get_file_name_and_cursor_position(CXCursor cursor, decl_pos *pos)
// {
//     CXSourceLocation location = clang_getCursorLocation(cursor);
//     CXFile           file;
//     clang_getFileLocation(location, &file, &pos->line, &pos->column, &pos->offset);
//     // Get the file name as a string
//     CXString cx_file_name = clang_getFileName(file);
//     string   ret          = clang_getCString(cx_file_name);
//     clang_disposeString(cx_file_name);
//     return ret;
// }

// /* Return`s a string containing the access specifier of a cursor. */
// string
// Lsp::utils::get_access_specifier_str(CXCursor cursor)
// {
//     switch (clang_getCXXAccessSpecifier(cursor))
//     {
//         case CX_CXXInvalidAccessSpecifier :
//             return "Invalid access specifier";
//         case CX_CXXPublic :
//             return "public";
//         case CX_CXXProtected :
//             return "protected";
//         case CX_CXXPrivate :
//             return "private";
//     }
// }

// /* Get access specifier opt enum value. */
// opts
// Lsp::utils::get_access_specifier_opt(CXCursor cursor)
// {
//     switch (clang_getCXXAccessSpecifier(cursor))
//     {
//         case CX_CXXInvalidAccessSpecifier :
//             return INVALID_ACCESS_SPECIFIER;
//         case CX_CXXPublic :
//             return PUBLIC_DECL;
//         case CX_CXXPrivate :
//             return PRIVATE_DECL;
//         case CX_CXXProtected :
//             return PROTECTED_DECL;
//     }
// }

// /* Set access specifier flag in flags. */
// void
// Lsp::utils::set_access_specifier_flag(CXCursor cursor, bit_flag_t<OPTS_SIZE> *flags)
// {
//     flags->unset<INVALID_ACCESS_SPECIFIER>();
//     flags->unset<PUBLIC_DECL>();
//     flags->unset<PRIVATE_DECL>();
//     flags->unset<PROTECTED_DECL>();
//     switch (clang_getCXXAccessSpecifier(cursor))
//     {
//         case CX_CXXInvalidAccessSpecifier :
//             flags->set<INVALID_ACCESS_SPECIFIER>();
//             break;
//         case CX_CXXPublic :
//             flags->set<PUBLIC_DECL>();
//             break;
//         case CX_CXXPrivate :
//             flags->set<PRIVATE_DECL>();
//             break;
//         case CX_CXXProtected :
//             flags->set<PROTECTED_DECL>();
//             break;
//     }
// }

// string
// Lsp::utils::get_cursor_breif_comment_string(CXCursor cursor)
// {
//     CXString    cxstr = clang_Cursor_getBriefCommentText(cursor);
//     const char *cstr  = clang_getCString(cxstr);
//     string      ret   = cstr ? cstr : "";
//     clang_disposeString(cxstr);
//     return ret;
// }

// string
// Lsp::utils::get_cursor_raw_comment_string(CXCursor cursor)
// {
//     CXString    cxstr = clang_Cursor_getRawCommentText(cursor);
//     const char *cstr  = clang_getCString(cxstr);
//     string      ret   = cstr ? cstr : "";
//     clang_disposeString(cxstr);
//     return ret;
// }

// CXCursor
// Lsp::utils::get_cursor_from_file(CXTranslationUnit TU, const char *file, Uint line, Uint col)
// {
//     /* Get the file handle for the file */
//     CXFile cxfile = clang_getFile(TU, file);
//     if (!cxfile)
//     {
//         logI("File not found in the translation unit.");
//         return clang_getNullCursor();
//     }
//     /* Get the source location based on the file, line, and column */
//     CXSourceLocation location = clang_getLocation(TU, cxfile, line, col);
//     /* Get the cursor at this location */
//     CXCursor cursor = clang_getCursor(TU, location);
//     return cursor;
// }

// CXCursor
// Lsp::utils::get_cursor_from_line_col(CXFile F, CXTranslationUnit TU, Uint line, Uint col)
// {
//     PROFILE_FUNCTION;
//     CXSourceLocation location = clang_getLocation(TU, F, line, col);
//     return clang_getCursor(TU, location);
// }

// function_decl *
// Lsp::utils::get_function_decl_from_str(const char *file, string name)
// {
//     PROFILE_FUNCTION;
//     index_data *id = Lsp::instance().get_file_index_data(file, false);
//     if (!id)
//     {
//         return nullptr;
//     }
//     for (const auto &it : id->functions)
//     {
//         if (it->name == name)
//         {
//             return it;
//         }
//     }
//     return nullptr;
// }

// function_decl *
// Lsp::utils::get_main_function_decl_from_str(const char *file, string name)
// {
//     PROFILE_FUNCTION;
//     index_data *id = Lsp::instance().get_file_index_data(file, false);
//     if (!id)
//     {
//         return nullptr;
//     }
//     for (const auto &it : id->main.functions)
//     {
//         if (it->name == name)
//         {
//             return it;
//         }
//     }
//     return nullptr;
// }

// function_decl *
// Lsp::utils::get_main_function_decl_from_line_col(const char *file, Uint line, Uint col)
// {
//     PROFILE_FUNCTION;
//     index_data *id = Lsp::instance().get_file_index_data(file, false);
//     if (!id)
//     {
//         return nullptr;
//     }
//     for (const auto &it : id->main.functions)
//     {
//         if (it->pos.line == line && it->pos.column == col)
//         {
//             return it;
//         }
//     }
//     return nullptr;
// }

// CXFile
// Lsp::utils::get_cxfile(CXCursor c)
// {
//     CXFile           F;
//     CXSourceLocation loc = clang_getCursorLocation(c);
//     clang_getFileLocation(loc, &F, nullptr, nullptr, nullptr);
//     return F;
// }

// void
// Lsp::utils::tokenize_function(CXTranslationUnit TU, CXCursor functionCursor)
// {
//     PROFILE_FUNCTION;
//     /* Get the source range of the function */
//     CXSourceRange range = clang_getCursorExtent(functionCursor);
//     /* Tokenize the source code in the range */
//     CXToken *tokens    = nullptr;
//     unsigned numTokens = 0;
//     clang_tokenize(TU, range, &tokens, &numTokens);
//     /* Print tokens and their kinds */
//     for (unsigned i = 0; i < numTokens; ++i)
//     {
//         /* Get the token spelling */
//         CXString tokenText = clang_getTokenSpelling(TU, tokens[i]);
//         /* Get the token kind */
//         CXTokenKind kind = clang_getTokenKind(tokens[i]);
//         /* Print the token text and kind */
//         NETLOGGER << "Token: " << clang_getCString(tokenText) << " - ";
//         switch (kind)
//         {
//             case CXToken_Keyword :
//                 NETLOGGER << "Keyword";
//                 break;
//             case CXToken_Identifier :
//                 NETLOGGER << "Identifier";
//                 break;
//             case CXToken_Literal :
//                 NETLOGGER << "Literal";
//                 break;
//             case CXToken_Punctuation :
//                 NETLOGGER << "Punctuation";
//                 break;
//             case CXToken_Comment :
//                 NETLOGGER << "Comment";
//                 break;
//         }
//         NETLOGGER << "\n" << NETLOG_ENDL;
//         clang_disposeString(tokenText);
//     }
//     clang_disposeTokens(TU, tokens, numTokens);
// }

// void
// Lsp::utils::get_all_function_cursors(CXCursor fc)
// {
//     PROFILE_FUNCTION;
//     clang_visitChildren(
//         fc,
//         [](CXCursor c, CXCursor parent, CXClientData client_data) -> CXChildVisitResult {
//             /* switch (clang_getCursorKind(c))
//             {

//             } */
//             NLOG("name %s\n", get_cursor_kind_spelling(clang_getCursorKind(c)).c_str());
//             return CXChildVisit_Recurse;
//         },
//         nullptr
//     );
// }

// void
// Lsp::utils::reparse_main_file(const char *file)
// {
//     PROFILE_FUNCTION;
//     index_data *id = Lsp::instance().get_file_index_data(file, false);
//     if (!id)
//     {
//         return;
//     }
//     id->main.reparse();
// }
// /* clang-format on */
