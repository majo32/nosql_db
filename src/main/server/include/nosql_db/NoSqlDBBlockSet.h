
#ifndef NOSQLDBBLOCKSET_H
#define NOSQLDBBLOCKSET_H

#include <nosql_db/NoSqlDBUtils.h>
#include <allocator/Allocator.h>
#include <dynamic_string/DynamicString.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        NoSqlDBBlock * data;
        size_t length;
    } NoSqlDBBlockBucket;

    typedef struct {
        NoSqlDBBlockBucket * buckets;
        size_t currentNumberOfBuckets;
        size_t numberOfBucketItems;
        size_t currentNumberOfBlocks;
    } NoSqlDBBlockSet;

    ErrorCode_t NoSqlDBBlockSet_init(NoSqlDBBlockSet * blockSet);
    ErrorCode_t NoSqlDBBlockSet_set(NoSqlDBBlockSet * blockSet, size_t index,
            const NoSqlDBBlock * block);
    ErrorCode_t NoSqlDBBlockSet_addBlock(NoSqlDBBlockSet * blockSet);
    ErrorCode_t NoSqlDBBlockSet_get(NoSqlDBBlockSet * blockSet, size_t index,
            NoSqlDBBlock * block);
    ErrorCode_t NoSqlDBBlockSet_getBlockPointer(NoSqlDBBlockSet * blockSet,
        size_t index, NoSqlDBBlock ** block); 
    ErrorCode_t NoSqlDBBlockSet_length(const NoSqlDBBlockSet * blockSet, size_t * length);
    ErrorCode_t NoSqlDBBlockSet_empty(NoSqlDBBlockSet * blockSet);
    ErrorCode_t NoSqlDBBlockSet_destroy(NoSqlDBBlockSet * blockSet);
    
    ErrorCode_t NoSqlDBBlockSet_createBlockSet(const DynamicString * key, const DynamicString * value,
            NoSqlDBBlockSet * blockSet);
    ErrorCode_t NoSqlDBBlockSet_getKey(DynamicString * key, NoSqlDBBlockSet * blockSet);
    ErrorCode_t NoSqlDBBlockSet_getValue(DynamicString * value, NoSqlDBBlockSet * blockSet);


#ifdef __cplusplus
}
#endif

#endif /* NOSQLDBBLOCKSET_H */

