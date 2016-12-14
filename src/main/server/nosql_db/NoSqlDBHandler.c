#include <nosql_db/NoSqlDBHandler.h>

ErrorCode_t NoSqlDBHandler_openDB(NoSqlDBHandler * dbHandler, const char * filename) {
    ErrorCode_t returnCode = ERROR_FAILED;

    returnCode = NoSqlDBFile_open(&(dbHandler->file), filename);

    return returnCode;
}

ErrorCode_t NoSqlDBHandler_closeDB(NoSqlDBHandler * dbHandler) {
    ErrorCode_t returnCode = ERROR_FAILED;

    returnCode = NoSqlDBFile_close(&(dbHandler->file));

    return returnCode;
}

static ErrorCode_t NoSqlDBHandler_loadRemainingBlocks(NoSqlDBHandler * dbHandler, NoSqlDBBlockSet * blockSet,
        Bool_t * endOfFile) {
    ErrorCode_t returnCode = ERROR_FAILED;
    NoSqlDBBlock block;
    size_t index = 1U;
    size_t remainingBlocks = 0U;

    returnCode = NoSqlDBBlockSet_get(blockSet, 0U, &block);

    if (NO_ERROR == returnCode) {
        remainingBlocks = (block.size / NOSQLDBBLOCK_SIZE) +
                (((block.size % NOSQLDBBLOCK_SIZE) != 0) ? 1 : 0);
        for (index = 1U; (index < remainingBlocks) && (FALSE == *endOfFile) &&
                (NO_ERROR == returnCode);
                ++index) {
            returnCode = NoSqlDBFile_hasNextBlock(&(dbHandler->file), endOfFile);
            if ((FALSE == *endOfFile) && (NO_ERROR == returnCode)) {
                returnCode = NoSqlDBFile_readNextBlock(&(dbHandler->file), &block);
                if (NO_ERROR == returnCode) {
                    if (block.status == NOSQL_DB_BLOCK_STATUS_ASSIGNED) {
                        returnCode = NoSqlDBBlockSet_addBlock(blockSet);
                        if (NO_ERROR == returnCode) {
                            returnCode = NoSqlDBBlockSet_set(blockSet, index,
                                    &block);
                        }
                    } else {
                        returnCode = ERROR_CORRUPTED_FILE;
                    }
                }
            }
        }
    }

    return returnCode;
}

ErrorCode_t NoSqlDBHandler_loadNextRecord(NoSqlDBHandler * dbHandler,
        NoSqlDBBlockSet * blockSet, Bool_t * found, FilePosition_t * foundIndex, Bool_t * endOfFile) {
    ErrorCode_t returnCode = ERROR_FAILED;
    NoSqlDBBlock block;
    FilePosition_t tempFoundIndex;
    *found = FALSE;
    *endOfFile = FALSE;
    returnCode = NoSqlDBBlockSet_empty(blockSet);

    do {
        if (NO_ERROR == returnCode) {
            returnCode = NoSqlDBFile_hasNextBlock(&(dbHandler->file), endOfFile);
            if ((FALSE == *endOfFile) && (NO_ERROR == returnCode)) {
                returnCode = FileHandler_getCurrentPos(&(dbHandler->file.fileHandler), &tempFoundIndex);
                if (NO_ERROR == returnCode) {
                    returnCode = NoSqlDBFile_readNextBlock(&(dbHandler->file), &block);
                }
                if (NO_ERROR == returnCode) {
                    if (NOSQL_DB_BLOCK_STATUS_HEAD == block.status) {
                        returnCode = NoSqlDBBlockSet_addBlock(blockSet);
                        if (NO_ERROR == returnCode) {
                            returnCode = NoSqlDBBlockSet_set(blockSet, 0U, &block);
                        }
                        if (NO_ERROR == returnCode) {
                            returnCode = NoSqlDBHandler_loadRemainingBlocks(
                                    dbHandler, blockSet, endOfFile);
                            if (NO_ERROR == returnCode) {
                                *foundIndex = tempFoundIndex;
                                *found = TRUE;
                            }
                        }
                    }
                }
            }
        }
    } while ((NO_ERROR == returnCode) &&
            (FALSE == *endOfFile) &&
            (FALSE == *found));

    return returnCode;
}

ErrorCode_t NoSqlDBHandler_findFreeBlocks(NoSqlDBHandler * dbHandler,
        size_t size, FilePosition_t startPos, Bool_t * found, FilePosition_t * foundIndex, Bool_t * endOfFile) {
    ErrorCode_t returnCode = ERROR_FAILED;
    NoSqlDBBlock block;
    *found = FALSE;
    *endOfFile = FALSE;

    returnCode = NoSqlDBFile_navigateToPos(&(dbHandler->file), startPos);

    do {
        Bool_t loop = TRUE;
        size_t i = 0;
        FilePosition_t currentPos;

        returnCode = NoSqlDBFile_getPos(&(dbHandler->file), &currentPos);

        for (i = 0; (i < size) && (TRUE == loop) && (NO_ERROR == returnCode); ++i) {
            returnCode = NoSqlDBFile_hasNextBlock(&(dbHandler->file), endOfFile);
            if (NO_ERROR == returnCode) {
                if (FALSE == *endOfFile) {
                    returnCode = NoSqlDBFile_readNextBlock(&(dbHandler->file), &block);
                    if (NO_ERROR == returnCode) {
                        if (block.status != NOSQL_DB_BLOCK_STATUS_UNASSIGNED) {
                            loop = FALSE;
                        }
                    }
                } else {
                    loop = FALSE;
                }
            }
        }
        if ((TRUE == loop) && (NO_ERROR == returnCode)) {
            *found = TRUE;
            *foundIndex = currentPos;
        }
    } while ((NO_ERROR == returnCode) &&
            (FALSE == *endOfFile) &&
            (FALSE == *found));

    return returnCode;
}

static ErrorCode_t NoSqlDBHandler_putBlocks(NoSqlDBHandler * dbHandler,
        NoSqlDBBlockSet * blockSet) {
    ErrorCode_t returnCode = ERROR_FAILED;

    size_t i;
    size_t blocksSize;
    NoSqlDBBlock * block;
    returnCode = NoSqlDBBlockSet_length(blockSet, &blocksSize);
    for (i = 0; (i < blocksSize) &&
            (NO_ERROR == returnCode); i++) {
        returnCode = NoSqlDBBlockSet_getBlockPointer(blockSet, i, &block);
        if (NO_ERROR == returnCode) {
            returnCode = NoSqlDBFile_writeNextBlock(&(dbHandler->file), block);
        }
    }

    return returnCode;
}

ErrorCode_t NoSqlDBHandler_overwriteBlocks(NoSqlDBHandler * dbHandler,
        FilePosition_t position, NoSqlDBBlockSet * blockSet) {
    ErrorCode_t returnCode = ERROR_FAILED;

    returnCode = NoSqlDBFile_navigateToPos(&(dbHandler->file), position);
    if (NO_ERROR == returnCode) {
        returnCode = NoSqlDBHandler_putBlocks(dbHandler, blockSet);
    }

    return returnCode;
}

ErrorCode_t NoSqlDBHandler_appendBlocks(NoSqlDBHandler * dbHandler,
        NoSqlDBBlockSet * blockSet) {
    ErrorCode_t returnCode = ERROR_FAILED;

    returnCode = NoSqlDBFile_navigateToBottom(&(dbHandler->file));
    if (NO_ERROR == returnCode) {
        returnCode = NoSqlDBHandler_putBlocks(dbHandler, blockSet);
    }

    return returnCode;
}

ErrorCode_t NoSqlDBHandler_invalidateBlocks(NoSqlDBHandler * dbHandler,
        FilePosition_t position, size_t numberOfBlocks) {
    ErrorCode_t returnCode = ERROR_FAILED;

    returnCode = NoSqlDBFile_navigateToPos(&(dbHandler->file), position);
    if (NO_ERROR == returnCode) {
        size_t i;
        NoSqlDBBlock block = {0};
        block.status = NOSQL_DB_BLOCK_STATUS_UNASSIGNED;

        for (i = 0; (i < numberOfBlocks) &&
                (NO_ERROR == returnCode); i++) {
            returnCode = NoSqlDBFile_writeNextBlock(&(dbHandler->file), &block);
        }
    }

    return returnCode;
}

ErrorCode_t NoSqlDBHandler_writeBlocks(NoSqlDBHandler * dbHandler,
        NoSqlDBBlockSet * blockSet) {
    ErrorCode_t returnCode = ERROR_FAILED;
    Bool_t found = FALSE;
    FilePosition_t foundIndex;
    Bool_t endOfFile;
    size_t numberOfBlocks;

    returnCode = NoSqlDBBlockSet_length(blockSet, &numberOfBlocks);

    if (NO_ERROR == returnCode) {
        returnCode = NoSqlDBHandler_findFreeBlocks(dbHandler,
                numberOfBlocks, 0U, &found, &foundIndex, &endOfFile);
        if (NO_ERROR == returnCode) {
            if (found == TRUE) {
                returnCode = NoSqlDBHandler_overwriteBlocks(dbHandler,
                        foundIndex, blockSet);
            } else {
                returnCode = NoSqlDBHandler_appendBlocks(dbHandler, blockSet);
            }
        }
    }

    return returnCode;
}

ErrorCode_t NoSqlDBHandler_navigateToTop(NoSqlDBHandler * dbHandler) {
    ErrorCode_t returnCode = ERROR_FAILED;
    FilePosition_t pos;

    returnCode = NoSqlDBHandler_getTopPos(dbHandler, &pos);

    if (NO_ERROR == returnCode) {
        returnCode = NoSqlDBFile_navigateToPos(&(dbHandler->file), pos);
    }

    return returnCode;
}

ErrorCode_t NoSqlDBHandler_getTopPos(NoSqlDBHandler * dbHandler,
        FilePosition_t * pos) {
    ErrorCode_t returnCode = ERROR_FAILED;

    *pos = 0;
    returnCode = NO_ERROR;

    return returnCode;
}