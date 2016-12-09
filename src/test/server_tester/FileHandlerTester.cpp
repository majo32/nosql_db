#include <gtest/gtest.h>
#include <file_utils/FileHandler.h>
#include <string.h>

TEST(FileHandlerTester, SimpleCases) {
    const char * filename = "tester.xxx";
    const char * payload = "01234567890123456789";
    char data[500];
    FilePosition_t pos;
    size_t filesize;
    FileHandler filehandler;
    
    ErrorCode_t errorCode;
    
    errorCode = FileHandler_create(&filehandler, filename);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = FileHandler_close(&filehandler);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = FileHandler_open(&filehandler, filename);
    ASSERT_EQ(NO_ERROR, errorCode);
   
    errorCode = FileHandler_getFileSize(&filehandler,&filesize);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_EQ(0U, filesize);
    
    errorCode = FileHandler_navigateToTop(&filehandler);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = FileHandler_write(&filehandler,payload,sizeof (char) * 10);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = FileHandler_navigateToTop(&filehandler);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = FileHandler_read(&filehandler,data,sizeof (char) * 10);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_EQ(0, memcmp(payload, data, sizeof (char) * 10));
    
    errorCode = FileHandler_navigateToTop(&filehandler);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = FileHandler_read(&filehandler,data,sizeof (char) * 2);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_EQ(0, memcmp(payload, data, sizeof (char) * 2));
    
    errorCode = FileHandler_getCurrentPos(&filehandler, &pos);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = FileHandler_navigateToTop(&filehandler);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = FileHandler_navigateToPos(&filehandler,pos);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = FileHandler_read(&filehandler,data,sizeof (char) * 2);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_EQ(0, memcmp(data, &(payload[2]), sizeof (char) * 2));
    
    errorCode = FileHandler_navigateToBottom(&filehandler);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = FileHandler_write(&filehandler,payload,sizeof (char) * 10);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = FileHandler_navigateToTop(&filehandler);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = FileHandler_read(&filehandler,data,sizeof (char) * 20);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_EQ(0, memcmp(payload, data, sizeof (char) * 20));
    
    errorCode = FileHandler_navigateToPos(&filehandler,pos);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = FileHandler_write(&filehandler,payload,sizeof (char) * 10);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = FileHandler_navigateToTop(&filehandler);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = FileHandler_read(&filehandler,data,sizeof (char) * 10);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_EQ(0, memcmp("0101234567", data, sizeof (char) * 10));
    
    errorCode = FileHandler_getFileSize(&filehandler,&filesize);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_EQ(20U, filesize);
    
    errorCode = FileHandler_close(&filehandler);
    ASSERT_EQ(NO_ERROR, errorCode);
    
}