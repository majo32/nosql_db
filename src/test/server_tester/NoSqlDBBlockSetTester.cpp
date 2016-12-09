#include <gtest/gtest.h>
#include <nosql_db/NoSqlDBBlockSet.h>
#include <string.h>

TEST(NoSqlDBBlockSetTester, BlockSetUtilsTests) {
    NoSqlDBBlockSet blockSet;
    DynamicString key;
    DynamicString val;
    
    ErrorCode_t errorCode;
    
    ASSERT_EQ(NOSQLDBBLOCK_SIZE, 50);
    
    errorCode = NoSqlDBBlockSet_init(&blockSet);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = DynamicString_create(&key);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = DynamicString_create(&val);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = DynamicString_appendString(&key,"01234567890key45678901234567890123456789012345678");
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = DynamicString_appendString(&val,"01234567890123456val01234567890123456789012345678");
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = NoSqlDBBlockSet_createBlockSet(&key,&val,&blockSet);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    size_t len;
    
    errorCode = NoSqlDBBlockSet_length(&blockSet,&len);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_EQ(2, len);
    
    NoSqlDBBlock block;
    char *c;
    
    errorCode = NoSqlDBBlockSet_get(&blockSet, 0, &block);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_EQ(block.status, NOSQL_DB_BLOCK_STATUS_HEAD);
    ASSERT_EQ(block.size, 100);
    ASSERT_STREQ(block.data, "01234567890key45678901234567890123456789012345678");
    
    errorCode = NoSqlDBBlockSet_get(&blockSet, 1, &block);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_EQ(block.status, NOSQL_DB_BLOCK_STATUS_ASSIGNED);
    ASSERT_EQ(block.size, 0);
    ASSERT_STREQ(block.data, "01234567890123456val01234567890123456789012345678");
    
    errorCode = NoSqlDBBlockSet_getKey(&key,&blockSet);
    ASSERT_EQ(NO_ERROR, errorCode);
    errorCode = DynamicString_getString(&key,&c);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_STREQ(c, "01234567890key45678901234567890123456789012345678");
    
    errorCode = NoSqlDBBlockSet_getValue(&val,&blockSet);
    ASSERT_EQ(NO_ERROR, errorCode);
    errorCode = DynamicString_getString(&val,&c);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_STREQ(c, "01234567890123456val01234567890123456789012345678");
    
    errorCode = DynamicString_empty(&key);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = DynamicString_empty(&val);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = DynamicString_appendString(&key,"keykeykeykeykeykeykeykeykeykeykeykeykeykeykeykeykeykeykeykeykeykeykeykeykey");
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = DynamicString_appendString(&val,"valvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalval");
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = NoSqlDBBlockSet_createBlockSet(&key,&val,&blockSet);
    ASSERT_EQ(NO_ERROR, errorCode);
    
     errorCode = NoSqlDBBlockSet_getKey(&key,&blockSet);
    ASSERT_EQ(NO_ERROR, errorCode);
    errorCode = DynamicString_getString(&key,&c);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_STREQ(c, "keykeykeykeykeykeykeykeykeykeykeykeykeykeykeykeykeykeykeykeykeykeykeykeykey");
    
    errorCode = NoSqlDBBlockSet_getValue(&val,&blockSet);
    ASSERT_EQ(NO_ERROR, errorCode);
    errorCode = DynamicString_getString(&val,&c);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_STREQ(c, "valvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalvalval");
    
    
    
    errorCode = DynamicString_empty(&key);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = DynamicString_empty(&val);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = DynamicString_appendString(&key,"key");
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = DynamicString_appendString(&val,"val");
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = NoSqlDBBlockSet_createBlockSet(&key,&val,&blockSet);
    ASSERT_EQ(NO_ERROR, errorCode);
    
     errorCode = NoSqlDBBlockSet_getKey(&key,&blockSet);
    ASSERT_EQ(NO_ERROR, errorCode);
    errorCode = DynamicString_getString(&key,&c);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_STREQ(c, "key");
    
    errorCode = NoSqlDBBlockSet_getValue(&val,&blockSet);
    ASSERT_EQ(NO_ERROR, errorCode);
    errorCode = DynamicString_getString(&val,&c);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_STREQ(c, "val");
    
    
    
    
}

TEST(NoSqlDBBlockSetTester, SimpleCases) {
    NoSqlDBBlock block, block2;
    NoSqlDBBlockSet blockSet;

    ErrorCode_t errorCode;

    errorCode = NoSqlDBBlockSet_init(&blockSet);
    ASSERT_EQ(NO_ERROR, errorCode);

    errorCode = NoSqlDBBlockSet_get(&blockSet, 0, &block);
    ASSERT_EQ(ERROR_OUT_OF_RANGE, errorCode);

    errorCode = NoSqlDBBlockSet_addBlock(&blockSet);
    ASSERT_EQ(NO_ERROR, errorCode);

    errorCode = NoSqlDBBlockSet_get(&blockSet, 0, &block);
    ASSERT_EQ(NO_ERROR, errorCode);

    const char * str = "ABACA";
    memcpy((void *) block.data, (void *) str, strlen(str));
    errorCode = NoSqlDBBlockSet_set(&blockSet, 0, &block);
    ASSERT_EQ(NO_ERROR, errorCode);

    errorCode = NoSqlDBBlockSet_get(&blockSet, 0, &block2);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_EQ(0, memcmp(&block, &block2, sizeof (block)));

    for (size_t i = 0; i < 10000; i++) {
        errorCode = NoSqlDBBlockSet_addBlock(&blockSet);
        ASSERT_EQ(NO_ERROR, errorCode);

        memcpy((void *) block.data, (void *) &i, sizeof (i));
        errorCode = NoSqlDBBlockSet_set(&blockSet, i, &block);
        ASSERT_EQ(NO_ERROR, errorCode);

        errorCode = NoSqlDBBlockSet_get(&blockSet, i, &block2);
        ASSERT_EQ(NO_ERROR, errorCode);
        ASSERT_EQ(0, memcmp(&block, &block2, sizeof (block)));
    }

    for (size_t i = 0; i < 10000; i++) {
        memcpy((void *) block.data, (void *) &i, sizeof (i));

        errorCode = NoSqlDBBlockSet_get(&blockSet, i, &block2);
        ASSERT_EQ(NO_ERROR, errorCode);
        ASSERT_EQ(0, memcmp(&block, &block2, sizeof (block)));
    }

    errorCode = NoSqlDBBlockSet_destroy(&blockSet);
    ASSERT_EQ(NO_ERROR, errorCode);
}