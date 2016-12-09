#include <nosql_db/NoSqlDBBlockSet.h>
#include <stdio.h>

static ErrorCode_t NoSqlDBBlockSet_allocateNewBucket(NoSqlDBBlockSet * blockSet) {
    ErrorCode_t returnCode = ERROR_FAILED;

    returnCode = Allocator_realloc(
            (void **) &(blockSet->buckets), blockSet->buckets,
            sizeof (NoSqlDBBlockBucket) * (blockSet->currentNumberOfBuckets + 1U));

    if (NO_ERROR == returnCode) {
        NoSqlDBBlockBucket * newBlockBucket =
                &(blockSet->buckets[blockSet->currentNumberOfBuckets]);
        newBlockBucket->length = blockSet->numberOfBucketItems;
        returnCode = Allocator_alloc((void **) &(newBlockBucket->data),
                sizeof (NoSqlDBBlock) * (newBlockBucket->length));
        if (NO_ERROR == returnCode) {
            blockSet->currentNumberOfBuckets = blockSet->currentNumberOfBuckets + 1U;
        }
    }

    return returnCode;
}

static ErrorCode_t NoSqlDBBlockSet_allocateNewBucketIfNeeded(NoSqlDBBlockSet * blockSet) {
    ErrorCode_t returnCode = ERROR_FAILED;

    if ((blockSet->currentNumberOfBuckets * blockSet->numberOfBucketItems) <
            (blockSet->currentNumberOfBlocks + 1U)) {
        returnCode = NoSqlDBBlockSet_allocateNewBucket(blockSet);
    } else {
        returnCode = NO_ERROR;
    }

    return returnCode;
}

ErrorCode_t NoSqlDBBlockSet_getBlockPointer(NoSqlDBBlockSet * blockSet,
        size_t index, NoSqlDBBlock ** block) {
    ErrorCode_t returnCode = ERROR_FAILED;

    const size_t bucket_index = (size_t) index / blockSet->numberOfBucketItems;
    const size_t offset_index = (size_t) index % blockSet->numberOfBucketItems;

    if (bucket_index < blockSet->currentNumberOfBuckets) {
        *block = &(blockSet->buckets[bucket_index].data[offset_index]);
        returnCode = NO_ERROR;
    } else {
        returnCode = ERROR_OUT_OF_RANGE;
    }


    return returnCode;
}

ErrorCode_t NoSqlDBBlockSet_init(NoSqlDBBlockSet * blockSet) {
    ErrorCode_t returnCode = ERROR_FAILED;

    blockSet->buckets = NULL;
    blockSet->currentNumberOfBlocks = 0U;
    blockSet->currentNumberOfBuckets = 0U;
    blockSet->numberOfBucketItems = 10U;
    returnCode = NO_ERROR;

    return returnCode;
}

ErrorCode_t NoSqlDBBlockSet_set(NoSqlDBBlockSet * blockSet, size_t index,
        const NoSqlDBBlock * block) {
    ErrorCode_t returnCode = ERROR_FAILED;
    NoSqlDBBlock * block_ptr;

    returnCode = NoSqlDBBlockSet_getBlockPointer(blockSet, index, &block_ptr);

    if (NO_ERROR == returnCode) {
        memcpy((void *) block_ptr, (void *) block, sizeof (NoSqlDBBlock));
    }

    return returnCode;
}

ErrorCode_t NoSqlDBBlockSet_addBlock(NoSqlDBBlockSet * blockSet) {
    ErrorCode_t returnCode = ERROR_FAILED;

    returnCode = NoSqlDBBlockSet_allocateNewBucketIfNeeded(blockSet);

    if (NO_ERROR == returnCode) {
        blockSet->currentNumberOfBlocks = blockSet->currentNumberOfBlocks + 1U;
    }

    return returnCode;
}

ErrorCode_t NoSqlDBBlockSet_get(NoSqlDBBlockSet * blockSet, size_t index,
        NoSqlDBBlock * block) {
    ErrorCode_t returnCode = ERROR_FAILED;
    NoSqlDBBlock * block_ptr;

    returnCode = NoSqlDBBlockSet_getBlockPointer(blockSet, index, &block_ptr);

    if (NO_ERROR == returnCode) {
        memcpy((void *) block, (void *) block_ptr, sizeof (NoSqlDBBlock));
    }

    return returnCode;
}

ErrorCode_t NoSqlDBBlockSet_length(const NoSqlDBBlockSet * blockSet, size_t * length) {
    ErrorCode_t returnCode = ERROR_FAILED;

    *length = blockSet->currentNumberOfBlocks;
    returnCode = NO_ERROR;

    return returnCode;
}

ErrorCode_t NoSqlDBBlockSet_empty(NoSqlDBBlockSet * blockSet) {
    ErrorCode_t returnCode = ERROR_FAILED;

    blockSet->currentNumberOfBlocks = 0U;
    returnCode = NO_ERROR;

    return returnCode;
}

ErrorCode_t NoSqlDBBlockSet_destroy(NoSqlDBBlockSet * blockSet) {
    ErrorCode_t returnCode = ERROR_FAILED;
    size_t i;

    returnCode = NO_ERROR;
    for (i = 0;
            (i < blockSet->currentNumberOfBuckets) &&
            (NO_ERROR == returnCode);
            i++) {
        returnCode = Allocator_free((void*) blockSet->buckets[i].data);
    }
    blockSet->currentNumberOfBlocks = 0U;
    blockSet->currentNumberOfBuckets = 0U;
    returnCode = Allocator_free(blockSet->buckets);

    return returnCode;
}

ErrorCode_t NoSqlDBBlockSet_createBlockSet(const DynamicString * key, const DynamicString * value,
        NoSqlDBBlockSet * blockSet) {
    ErrorCode_t returnCode = ERROR_FAILED;
    size_t len;
    size_t keyLen;
    size_t valLen;
    size_t i;
    size_t blockPos;
    size_t currentBlock;
    size_t numBlocks;
    char * strKey;
    char * strVal;


    returnCode = DynamicString_getString(key, &strKey);


    if (NO_ERROR == returnCode) {
        returnCode = DynamicString_getString(value, &strVal);
    }

    if (NO_ERROR == returnCode) {
        returnCode = DynamicString_getLength(key, &keyLen);
    }

    if (NO_ERROR == returnCode) {
        returnCode = DynamicString_getLength(value, &valLen);
    }

    if (NO_ERROR == returnCode) {
        len = valLen + keyLen + 2;
        numBlocks = (len / NOSQLDBBLOCK_SIZE) + (len % NOSQLDBBLOCK_SIZE ? 1 : 0);
        returnCode = NoSqlDBBlockSet_empty(blockSet);
    }

    blockPos = 0;
    currentBlock = 0;
    for (i = 0; (i < len) &&(NO_ERROR == returnCode); i++) {
        NoSqlDBBlock * block;

        if (blockPos == 0) {
            returnCode = NoSqlDBBlockSet_addBlock(blockSet);
        }

        if (NO_ERROR == returnCode) {
            returnCode = NoSqlDBBlockSet_getBlockPointer(blockSet, currentBlock, &block);
        }
        if (NO_ERROR == returnCode) {
            if ((blockPos == 0) && (currentBlock == 0)) {
                block->status = NOSQL_DB_BLOCK_STATUS_HEAD;
                block->size = len;
            } else if ((blockPos == 0) && (currentBlock > 0)) {
                block->status = NOSQL_DB_BLOCK_STATUS_ASSIGNED;
                block->size = 0;
            } else {

            }

            if (i < keyLen) {
                block->data[blockPos] = strKey[i];
            } else if (i == keyLen) {
                block->data[blockPos] = 0;
            } else if ((i > keyLen) && (i < (keyLen + 1 + valLen))) {
                block->data[blockPos] = strVal[i - (keyLen + 1)];
            } else {
                block->data[blockPos] = 0;
            }

            blockPos++;
            if (blockPos == NOSQLDBBLOCK_SIZE) {
                blockPos = 0;
                currentBlock++;
            }
        }
    }
    return returnCode;
}

ErrorCode_t NoSqlDBBlockSet_getKey(DynamicString * key, NoSqlDBBlockSet * blockSet) {
    ErrorCode_t returnCode = ERROR_FAILED;
    size_t numBlocks;
    size_t i, j;
    char c;
    Bool_t end = FALSE;


    returnCode = NoSqlDBBlockSet_length(blockSet, &numBlocks);

    if (NO_ERROR == returnCode) {
        returnCode = DynamicString_empty(key);
    }

    for (i = 0; (i < numBlocks) && (FALSE == end) && (NO_ERROR == returnCode); i++) {
        NoSqlDBBlock * block;
        returnCode = NoSqlDBBlockSet_getBlockPointer(blockSet, i, &block);
        for (j = 0; (j < NOSQLDBBLOCK_SIZE) && (FALSE == end) && (NO_ERROR == returnCode); j++) {
            if ('\0' != block->data[j]) {
                returnCode = DynamicString_appendChar(key, block->data[j]);
            } else {
                end = TRUE;
            }
        }
    }

    if (FALSE == end) {
        returnCode = ERROR_FAILED;
    }

    return returnCode;
}

ErrorCode_t NoSqlDBBlockSet_getValue(DynamicString * value, NoSqlDBBlockSet * blockSet) {
    ErrorCode_t returnCode = ERROR_FAILED;
    size_t numBlocks;
    size_t i, j;
    char c;
    Bool_t end = FALSE;
    Bool_t keySeq = TRUE;


    returnCode = NoSqlDBBlockSet_length(blockSet, &numBlocks);

    if (NO_ERROR == returnCode) {
        returnCode = DynamicString_empty(value);
    }

    for (i = 0; (i < numBlocks) && (FALSE == end) && (NO_ERROR == returnCode); i++) {
        NoSqlDBBlock * block;
        returnCode = NoSqlDBBlockSet_getBlockPointer(blockSet, i, &block);
        for (j = 0; (j < NOSQLDBBLOCK_SIZE) && (FALSE == end) && (NO_ERROR == returnCode); j++) {
            if (TRUE == keySeq) {
                if ('\0' == block->data[j]) {
                    keySeq = FALSE;
                }
            } else {
                if ('\0' != block->data[j]) {
                    returnCode = DynamicString_appendChar(value, block->data[j]);
                } else {
                    end = TRUE;
                }
            }
        }
    }

    if (FALSE == end) {
        returnCode = ERROR_FAILED;
    }

    return returnCode;
}