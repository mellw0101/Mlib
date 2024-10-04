// #pragma once

// #include "../Flag.h"
// #include "../Vector.h"
// #include "../def.h"

// #include "parse.hpp"

// #include <clang-c/CXDiagnostic.h>
// #include <clang-c/CXFile.h>
// #include <clang-c/CXSourceLocation.h>
// #include <clang-c/Index.h>

// #include <string>

// using std::string;

// void            decl_callback(CXClientData client_data, const CXIdxDeclInfo *decl_info);
// CXIdxClientFile ppIncludedFile(CXClientData                 client_data,
//                                const CXIdxIncludedFileInfo *included_file);
// void diagnostic_callback(CXClientData client_data, CXDiagnosticSet diagnostics, void *reserved);

// enum opts
// {
//     MAIN_FILE = 1,
//     INVALID_ACCESS_SPECIFIER,
//     PUBLIC_DECL,
//     PRIVATE_DECL,
//     PROTECTED_DECL,
//     IS_CONST,
//     IS_STATIC,
//     OPTS_SIZE = 8
// };

// inline namespace Lsp_dataStructures
// {
//     struct decl_pos
//     {
//         Uint line;
//         Uint column;
//         Uint offset;
//     };

//     struct variable_decl
//     {
//         string                type;
//         string                name;
//         string                file;
//         decl_pos              pos;
//         bit_flag_t<OPTS_SIZE> flags;
//         CXCursor              cursor;
//     };

//     struct function_variable_decl
//     {
//         string   type;
//         string   name;
//         CXCursor cursor;
//         decl_pos pos;
//         decl_pos s;
//         decl_pos e;
//     };

//     struct function_body
//     {
//         MVector<function_variable_decl> vars;
//     };

//     struct function_decl
//     {
//         string                          file;
//         decl_pos                        pos;
//         decl_pos                        start;
//         decl_pos                        end;
//         string                          name;
//         string                          return_type;
//         Uint                            num_args;
//         bit_flag_t<OPTS_SIZE>           flags;
//         MVector<function_variable_decl> params;
//         CXCursor                        cursor;
//         CXFile                          F;
//         CXTranslationUnit               TU;
//         string                          doc;
//         function_body                   body;
//     };

//     struct class_decl
//     {
//         string                 file;
//         decl_pos               start;
//         decl_pos               end;
//         string                 name;
//         bit_flag_t<OPTS_SIZE>  flags;
//         MVector<variable_decl> variabels;
//         MVector<function_decl> methods;
//         string                 inside_namespace;
//     };

//     struct struct_decl
//     {
//         string                 file;
//         decl_pos               start;
//         decl_pos               end;
//         string                 name;
//         bit_flag_t<OPTS_SIZE>  flags;
//         MVector<variable_decl> variabels;
//         MVector<function_decl> methods;
//         string                 inside_namespace;
//     };

//     struct index_data;

//     struct main_file_tu
//     {
//         CXTranslationUnit        tu;
//         MVector<function_decl *> functions {};

//         index_data *id;

//         void delete_data(void) noexcept;
//         void destroy(void);
//         void do_index(void);
//         void reparse(void);
//     };

//     struct index_data
//     {
//         CXFile            file;
//         string            file_str;
//         CXIndex           index;
//         CXTranslationUnit tu;
//         CXIndexAction     ia;
//         main_file_tu      main;

//         enum flags
//         {
//             IS_NEW    = 1,
//             FLAG_SIZE = 8
//         };
//         bit_flag_t<FLAG_SIZE> flags;

//         MVector<function_decl *> functions {};
//         MVector<class_decl *>    classes {};
//         MVector<struct_decl *>   structs {};
//         MVector<variable_decl *> variabels {};

//         void delete_data(void) noexcept;
//     };
// }

// class Lsp {
// private:
//     DEL_CM_CONSTRUCTORS(Lsp);
//     static Lsp *_instance;

//     /* Indexer callback structure */
//     IndexerCallbacks _index_callbacks = {
//         nullptr,             /* abortQuery */
//         diagnostic_callback, /* diagnostic */
//         nullptr,             /* enteredMainFile */
//         ppIncludedFile,      /* ppIncludedFile */
//         nullptr,             /* importedASTFile */
//         nullptr,             /* startedTranslationUnit */
//         decl_callback,       /* indexDeclaration callback */
//         nullptr              /* indexEntityReference */
//     };

//     Lsp(void) noexcept {};

//     static void _destroy(void) noexcept;

// public:
//     struct utils
//     {
//         static string get_cursor_type(CXCursor cursor);
//         static string get_cursor_name(CXCursor cursor);
//         static string get_cursor_kind_spelling(CXCursorKind kind);
//         static void   get_cursor_start_pos(CXCursor c, decl_pos *dp);
//         static void   get_cursor_end_pos(CXCursor c, decl_pos *dp);
//         static void   get_cursor_pos(CXCursor c, decl_pos *dp);
//         static void   get_cursor_range(CXCursor c, decl_pos *s, decl_pos *e);
//         static void   get_function_variable_decl(CXCursor c, function_variable_decl *fvd);
//         static string get_cursor_namespace_name(CXCursor cursor);
//         static MVector<function_variable_decl> get_cursor_function_args(CXCursor cursor, Uint *n);
//         static string                          get_cursor_return_type(CXCursor cursor);
//         static string   get_file_name_and_cursor_range(CXCursor cursor, decl_pos *start,
//                                                        decl_pos *end);
//         static string   get_file_name_and_cursor_position(CXCursor cursor, decl_pos *pos);
//         static string   get_access_specifier_str(CXCursor cursor);
//         static opts     get_access_specifier_opt(CXCursor cursor);
//         static void     set_access_specifier_flag(CXCursor cursor, bit_flag_t<OPTS_SIZE> *flags);
//         static string   get_cursor_breif_comment_string(CXCursor cursor);
//         static string   get_cursor_raw_comment_string(CXCursor cursor);
//         static CXCursor get_cursor_from_file(CXTranslationUnit TU, const char *file, Uint line,
//                                              Uint col);
//         static CXCursor get_cursor_from_line_col(CXFile F, CXTranslationUnit TU, Uint line,
//                                                  Uint col);
//         static CXFile   get_cxfile(CXCursor c);
//         static function_decl *get_function_decl_from_str(const char *file, string name);
//         static function_decl *get_main_function_decl_from_str(const char *file, string name);
//         static function_decl *get_main_function_decl_from_line_col(const char *file, Uint line,
//                                                                    Uint col);
//         static void           tokenize_function(CXTranslationUnit TU, CXCursor functionCursor);
//         static void           get_all_function_cursors(CXCursor fc);
//         static void           reparse_main_file(const char *file);
//     };

//     MVector<index_data *> idata {};

//     static Lsp &instance(void) noexcept;

//     index_data *get_file_index_data(const char *file, bool make_new_on_fail) noexcept;
//     int         index_file(const char *file);
// };
// #define LangServer Lsp::instance()
