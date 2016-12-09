#include <gtest/gtest.h>
#include <dynamic_string/DynamicString.h>


TEST(DynamicStringTester, SimpleCases)
{
    DynamicString dstring;
    
    ErrorCode_t errorCode;
    
    errorCode = DynamicString_create(&dstring);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = DynamicString_appendString(&dstring,"Hello");
    ASSERT_EQ(NO_ERROR, errorCode);
    
    char * c;
    
    errorCode = DynamicString_getString(&dstring,&c);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_STREQ(c,"Hello");
    
    errorCode = DynamicString_appendString(&dstring," Hello");
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = DynamicString_getString(&dstring,&c);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_STREQ(c,"Hello Hello");
    
    size_t len;
    
    errorCode = DynamicString_getLength(&dstring,&len);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_EQ(len,11);
    
    errorCode = DynamicString_appendChar(&dstring,'H');
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = DynamicString_getLength(&dstring,&len);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_EQ(len,12);
    
    errorCode = DynamicString_appendCharSequence(&dstring,"Haaaaa",3);
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = DynamicString_getLength(&dstring,&len);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_EQ(len,15);
    
    
    errorCode = DynamicString_empty(&dstring);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_STREQ(c,"");
    
    errorCode = DynamicString_appendString(&dstring," Hello");
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = DynamicString_appendString(&dstring," Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello");
    ASSERT_EQ(NO_ERROR, errorCode);
    
    errorCode = DynamicString_getString(&dstring,&c);
    ASSERT_EQ(NO_ERROR, errorCode);
    ASSERT_STREQ(c," Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello");
}