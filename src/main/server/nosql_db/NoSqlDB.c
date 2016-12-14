#include <nosql_db/NoSqlDB.h>
#include <stdio.h>
ErrorCode_t NoSqlDB_open(NoSqlDB * db, const char * filename) {
    ErrorCode_t returnCode = ERROR_FAILED;

    returnCode = NoSqlDBHandler_openDB(&(db->handler), filename);

    if (NO_ERROR == returnCode) {
        returnCode = NoSqlDBBlockSet_init(&(db->blockSetReadBuffer));
    }

    if (NO_ERROR == returnCode) {
        returnCode = NoSqlDBBlockSet_init(&(db->blockSetWriteBuffer));
    }

    if (NO_ERROR == returnCode) {
        returnCode = DynamicString_create(&(db->keyBuffer));
    }

    return returnCode;
}

ErrorCode_t NoSqlDB_close(NoSqlDB * db) {
    ErrorCode_t returnCode = ERROR_FAILED;

    returnCode = NoSqlDBHandler_closeDB(&(db->handler));

    if (NO_ERROR == returnCode) {
        returnCode = NoSqlDBBlockSet_destroy(&(db->blockSetReadBuffer));
    }

    if (NO_ERROR == returnCode) {
        returnCode = DynamicString_destroy(&(db->keyBuffer));
    }

    return returnCode;
}

ErrorCode_t NoSqlDB_get(NoSqlDB * db, const DynamicString * key, DynamicString * val, Bool_t * found) {
    ErrorCode_t returnCode = ERROR_FAILED;
    Bool_t endOfFile = FALSE;
    Bool_t foundRecord = TRUE;
    FilePosition_t foundIndex;
    *found = FALSE;
    char * cKey;
    char * c;

    returnCode = NoSqlDBHandler_navigateToTop(&(db->handler));

    if (NO_ERROR == returnCode) {
        returnCode = DynamicString_getString(key, &cKey);
    }

    if (NO_ERROR == returnCode) {
        returnCode = DynamicString_empty(val);
    }

    while ((NO_ERROR == returnCode) && (FALSE == *found) && (FALSE == endOfFile) && (TRUE == foundRecord)) {
        returnCode = NoSqlDBHandler_loadNextRecord(&(db->handler), &(db->blockSetReadBuffer), 
                &foundRecord, &foundIndex,&endOfFile);
        if ((TRUE == foundRecord) && (FALSE == endOfFile)) {
            if (NO_ERROR == returnCode) {
                returnCode = NoSqlDBBlockSet_getKey(&(db->keyBuffer), &(db->blockSetReadBuffer));
            }
            if (NO_ERROR == returnCode) {
                returnCode = DynamicString_getString(&(db->keyBuffer), &c);
            }
            if (NO_ERROR == returnCode) {
                if (strcmp(c, cKey) == 0) {

                    *found = TRUE;
                    returnCode = NoSqlDBBlockSet_getValue(val, &(db->blockSetReadBuffer));
                }
            }
        }
    }

    return returnCode;
}

ErrorCode_t NoSqlDB_set(NoSqlDB * db, const DynamicString * key, const DynamicString * val) {
    ErrorCode_t returnCode = ERROR_FAILED;

    returnCode = NoSqlDBBlockSet_createBlockSet(key, val, &(db->blockSetWriteBuffer));

    if (NO_ERROR == returnCode) {
        returnCode = NoSqlDB_del(db, key);
    }

    if (NO_ERROR == returnCode) {
        returnCode = NoSqlDBHandler_writeBlocks(&(db->handler), &(db->blockSetWriteBuffer));
    }

    return returnCode;
}

ErrorCode_t NoSqlDB_del(NoSqlDB * db, const DynamicString * key) {
    ErrorCode_t returnCode = ERROR_FAILED;
    Bool_t endOfFile = FALSE;
    Bool_t foundRecord = TRUE;
    Bool_t found = FALSE;
    FilePosition_t foundIndex;
    size_t numberOfBlocks;
    char * cKey;
    char * c;

    returnCode = NoSqlDBHandler_navigateToTop(&(db->handler));

    if (NO_ERROR == returnCode) {
        returnCode = DynamicString_getString(&(db->keyBuffer), &cKey);
    }

    while ((NO_ERROR == returnCode) && (FALSE == found) && (FALSE == endOfFile) && (TRUE == foundRecord)) {
        returnCode = NoSqlDBHandler_loadNextRecord(&(db->handler), &(db->blockSetReadBuffer), 
                &foundRecord, &foundIndex, &endOfFile);
        if ((TRUE == foundRecord) && (FALSE == endOfFile)) {
            if (NO_ERROR == returnCode) {
                returnCode = NoSqlDBBlockSet_getKey(&(db->keyBuffer), &(db->blockSetReadBuffer));
            }
            if (NO_ERROR == returnCode) {
                returnCode = DynamicString_getString(&(db->keyBuffer), &c);
            }

            if (NO_ERROR == returnCode) {
                returnCode = NoSqlDBBlockSet_length(&(db->blockSetReadBuffer), &numberOfBlocks);
            }

            if (NO_ERROR == returnCode) {
                if (strcmp(c, cKey) == 0) {
                    found = TRUE;
                    returnCode = NoSqlDBHandler_invalidateBlocks(&(db->handler),
                            foundIndex, numberOfBlocks);
                }
            }
        }
    }

    return returnCode;
}

