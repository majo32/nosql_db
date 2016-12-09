#ifndef DYNAMICSTRING_H
#define DYNAMICSTRING_H

#include <Common.h>

#ifdef __cplusplus
extern "C" {
#endif
    
#define DYNAMIC_STRING_ALLOCATION_PADDING 50

    typedef struct{
        char * content;
        size_t stringLenght;
        size_t currentSize;
    } DynamicString;

    ErrorCode_t DynamicString_create(DynamicString * dstring);
    ErrorCode_t DynamicString_destroy(DynamicString * dstring);
    ErrorCode_t DynamicString_getString(const DynamicString * dstring, char ** dest);
    ErrorCode_t DynamicString_getLength(const DynamicString * dstring, size_t * len);
    ErrorCode_t DynamicString_empty(DynamicString * dstring);
    ErrorCode_t DynamicString_appendChar(DynamicString * dstring, char c);
    ErrorCode_t DynamicString_appendString(DynamicString * dstring, const char * string);
    ErrorCode_t DynamicString_appendCharSequence(DynamicString * dstring, const char * seq, size_t seqLen);

#ifdef __cplusplus
}
#endif

#endif /* DYNAMICSTRING_H */

