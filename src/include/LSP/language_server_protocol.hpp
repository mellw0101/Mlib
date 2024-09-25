#pragma once

#include <clang-c/Index.h>

class Lsp {
private:
    static void _decl_callback(CXClientData client_data, const CXIdxDeclInfo *decl_info);
    static void _index_entity_reference(CXClientData              client_data,
                                        const CXIdxEntityRefInfo *ref_info);

    // Define the indexer callbacks structure
    IndexerCallbacks index_callbacks = {
        nullptr,        // abortQuery
        nullptr,        // diagnostic
        nullptr,        // enteredMainFile
        nullptr,        // ppIncludedFile
        nullptr,        // importedASTFile
        nullptr,        // startedTranslationUnit
        _decl_callback, // indexDeclaration callback
        nullptr         // indexEntityReference
    };

public:
    int index_file(const char *file);
};
