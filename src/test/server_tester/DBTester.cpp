#include <gtest/gtest.h>
#include <nosql_db/NoSqlDBHandler.h>
#include <nosql_db/NoSqlDB.h>
TEST(NoSqlDBHandlerTester, SimpleCases) {
    NoSqlDBHandler dbHandler;
    ErrorCode_t errorCode;
    const char * filename = "tester.db.xxx";
    const char * payload = "01234567890123456789";
    char data[500];
    FileHandler filehandler;
    size_t size;
    FilePosition_t startPos;
    Bool_t found;
    size_t foundIndex;
    Bool_t endOfFile;
    NoSqlDBBlock block, block2;
    NoSqlDBBlockSet blockSet, blockSet2;

    errorCode = NoSqlDBBlockSet_init(&blockSet);
    ASSERT_EQ(NO_ERROR, errorCode);

    errorCode = NoSqlDBBlockSet_init(&blockSet2);
    ASSERT_EQ(NO_ERROR, errorCode);

    errorCode = FileHandler_create(&filehandler, filename);
    ASSERT_EQ(NO_ERROR, errorCode);

    errorCode = FileHandler_close(&filehandler);
    ASSERT_EQ(NO_ERROR, errorCode);


    errorCode = NoSqlDBHandler_openDB(&dbHandler, filename);
    ASSERT_EQ(NO_ERROR, errorCode);

    errorCode = NoSqlDBHandler_findFreeBlocks(&dbHandler,
            1U, 0U, &found, &foundIndex, &endOfFile);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_EQ(FALSE, found);
    ASSERT_EQ(TRUE, endOfFile);

    block.status = NOSQL_DB_BLOCK_STATUS_HEAD;
    block.size = 10;
    block.data[0] = 10;

    ASSERT_TRUE(10 < NOSQLDBBLOCK_SIZE);

    errorCode = NoSqlDBBlockSet_empty(&blockSet);
    ASSERT_EQ(NO_ERROR, errorCode);

    errorCode = NoSqlDBBlockSet_addBlock(&blockSet);
    ASSERT_EQ(NO_ERROR, errorCode);

    errorCode = NoSqlDBBlockSet_set(&blockSet, 0, &block);
    ASSERT_EQ(NO_ERROR, errorCode);

    errorCode = NoSqlDBHandler_writeBlocks(&dbHandler, &blockSet);
    ASSERT_EQ(NO_ERROR, errorCode);

    errorCode = FileHandler_getFileSize(&(dbHandler.file.fileHandler), &size);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_EQ(sizeof (NoSqlDBBlock), size);

    errorCode = NoSqlDBHandler_invalidateBlocks(&dbHandler, 0U, 1U);
    ASSERT_EQ(NO_ERROR, errorCode);

    errorCode = NoSqlDBHandler_findFreeBlocks(&dbHandler,
            1U, 0U, &found, &foundIndex, &endOfFile);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_EQ(0U, foundIndex);
    ASSERT_EQ(TRUE, found);
    ASSERT_EQ(FALSE, endOfFile);

    errorCode = NoSqlDBHandler_writeBlocks(&dbHandler, &blockSet);
    ASSERT_EQ(NO_ERROR, errorCode);

    errorCode = FileHandler_getFileSize(&(dbHandler.file.fileHandler), &size);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_EQ(sizeof (NoSqlDBBlock), size);

    block.data[0] = 11;

    errorCode = NoSqlDBBlockSet_set(&blockSet, 0, &block);
    ASSERT_EQ(NO_ERROR, errorCode);

    errorCode = NoSqlDBHandler_writeBlocks(&dbHandler, &blockSet);
    ASSERT_EQ(NO_ERROR, errorCode);

    errorCode = FileHandler_getFileSize(&(dbHandler.file.fileHandler), &size);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_EQ(sizeof (NoSqlDBBlock) * 2, size);


    errorCode = NoSqlDBFile_navigateToTop(&(dbHandler.file));
    ASSERT_EQ(NO_ERROR, errorCode);

    errorCode = NoSqlDBHandler_loadNextRecord(&dbHandler,
            &blockSet2, &found, &endOfFile);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_EQ(TRUE, found);
    ASSERT_EQ(FALSE, endOfFile);
    ASSERT_EQ(1, blockSet2.currentNumberOfBlocks);
    errorCode = NoSqlDBBlockSet_get(&blockSet2, 0, &block2);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_EQ(10, block2.data[0]);

    errorCode = NoSqlDBHandler_loadNextRecord(&dbHandler,
            &blockSet2, &found, &endOfFile);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_EQ(TRUE, found);
    ASSERT_EQ(FALSE, endOfFile);
    ASSERT_EQ(1, blockSet2.currentNumberOfBlocks);
    errorCode = NoSqlDBBlockSet_get(&blockSet2, 0, &block2);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_EQ(11, block2.data[0]);
    
    errorCode = NoSqlDBHandler_loadNextRecord(&dbHandler,
            &blockSet2, &found, &endOfFile);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_EQ(FALSE, found);
    ASSERT_EQ(TRUE, endOfFile);
    ASSERT_EQ(0, blockSet2.currentNumberOfBlocks);
    
}
TEST(NoSqlDBHandlerTester, DBTest1) {
    NoSqlDB db;
    
    ErrorCode_t errorCode;
    const char * filename = "tester.db1.xxx";
    
    FileHandler filehandler;
    Bool_t found;

    
    DynamicString key;
    DynamicString val;
    
    errorCode = DynamicString_create(&key);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = DynamicString_create(&val);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = FileHandler_create(&filehandler, filename);
    ASSERT_EQ(NO_ERROR, errorCode);

    errorCode = FileHandler_close(&filehandler);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    
    errorCode = NoSqlDB_open(&db,filename);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    
    errorCode = DynamicString_appendString(&key,"Key1");
    ASSERT_EQ(NO_ERROR, errorCode);
    

    errorCode = NoSqlDB_get(&db,&key,&val, &found);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_EQ(FALSE, found);
    
    errorCode = DynamicString_appendString(&val,"Val");
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = NoSqlDB_set(&db,&key,&val);
    ASSERT_EQ(NO_ERROR, errorCode);

    
    errorCode = NoSqlDB_get(&db,&key,&val, &found);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_EQ(TRUE, found);
    
    errorCode = NoSqlDB_del(&db,&key);
    ASSERT_EQ(NO_ERROR, errorCode);

    
    errorCode = NoSqlDB_get(&db,&key,&val, &found);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_EQ(FALSE, found);
    
    errorCode = DynamicString_empty(&key);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = NoSqlDB_close(&db);
    ASSERT_EQ(NO_ERROR, errorCode);
}