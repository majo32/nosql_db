#ifndef NOSQLDBHANDLER_H
#define NOSQLDBHANDLER_H

#include <dynamic_string/DynamicString.h>
#include <nosql_db/NoSqlDBFile.h>
#include <nosql_db/NoSqlDBBlockSet.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        NoSqlDBFile file;
    } NoSqlDBHandler;

    ErrorCode_t NoSqlDBHandler_openDB(NoSqlDBHandler * dbHandler, const char * filename);

    ErrorCode_t NoSqlDBHandler_closeDB(NoSqlDBHandler * dbHandler);

    ErrorCode_t NoSqlDBHandler_loadNextRecord(NoSqlDBHandler * dbHandler,
            NoSqlDBBlockSet * blockSet, Bool_t * found, Bool_t * endOfFile);

    ErrorCode_t NoSqlDBHandler_findFreeBlocks(NoSqlDBHandler * dbHandler,
            size_t size, FilePosition_t startPos, Bool_t * found, size_t * foundIndex, Bool_t * endOfFile);

    ErrorCode_t NoSqlDBHandler_overwriteBlocks(NoSqlDBHandler * dbHandler,
            FilePosition_t position, NoSqlDBBlockSet * blockSet);

    ErrorCode_t NoSqlDBHandler_appendBlocks(NoSqlDBHandler * dbHandler,
            NoSqlDBBlockSet * blockSet);

    ErrorCode_t NoSqlDBHandler_invalidateBlocks(NoSqlDBHandler * dbHandler,
            FilePosition_t position, size_t numberOfBlocks);

    ErrorCode_t NoSqlDBHandler_writeBlocks(NoSqlDBHandler * dbHandler,
            NoSqlDBBlockSet * blockSet);

    ErrorCode_t NoSqlDBHandler_navigateToTop(NoSqlDBHandler * dbHandler);

    ErrorCode_t NoSqlDBHandler_getTopPos(NoSqlDBHandler * dbHandler,
            size_t * pos);

#ifdef __cplusplus
}
#endif

#endif /* NOSQLDBHANDLER_H */

