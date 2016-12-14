#include <dynamic_string/DynamicString.h>
#include <allocator/Allocator.h>
#include <string.h>

static ErrorCode_t DynamicString_allocateIfNeeded(DynamicString * dstring,
        size_t len);

ErrorCode_t DynamicString_create(DynamicString * dstring) {
    ErrorCode_t returnCode = ERROR_FAILED;

    dstring->content = NULL;
    dstring->currentSize = 0;
    dstring->stringLenght = 0;

    returnCode = DynamicString_allocateIfNeeded(dstring, DYNAMIC_STRING_ALLOCATION_PADDING);
    
    if(NO_ERROR == returnCode){
        dstring->content[0] = '\0'; 
    }

    return returnCode;
}

ErrorCode_t DynamicString_getString(const DynamicString * dstring, char ** dest) {
    ErrorCode_t returnCode = ERROR_FAILED;

    if (dstring->content != NULL) {
        *dest = dstring->content;
        returnCode = NO_ERROR;
    } else {
        returnCode = ERROR_INVALID_INPUT_PARAMETER;
    }

    return returnCode;
}
ErrorCode_t DynamicString_getLength(const DynamicString * dstring, size_t * len){
    ErrorCode_t returnCode = ERROR_FAILED;

    if (dstring->content != NULL) {
        *len = dstring->stringLenght;
        returnCode = NO_ERROR;
    } else {
        *len = 0;
        returnCode = NO_ERROR;;
    }

    return returnCode;
}
ErrorCode_t DynamicString_empty(DynamicString * dstring) {
    ErrorCode_t returnCode = ERROR_FAILED;

    if (dstring->content != NULL) {
        dstring->stringLenght = 0;
        dstring->content[0] = '\0';
        returnCode = NO_ERROR;
    } else {
        returnCode = ERROR_INVALID_INPUT_PARAMETER;
    }

    return returnCode;
}

static ErrorCode_t DynamicString_allocateIfNeeded(DynamicString * dstring,
        size_t len) {
    ErrorCode_t returnCode = ERROR_FAILED;

    if (dstring->currentSize < (dstring->stringLenght + len + 1U)) {
        size_t newSize = (dstring->stringLenght +
                len + 1U + DYNAMIC_STRING_ALLOCATION_PADDING) *
                sizeof (char);
        returnCode = Allocator_realloc((void **) &(dstring->content), dstring->content, newSize);
        if (NO_ERROR == returnCode) {
            dstring->currentSize = newSize;
        }
    } else {
        returnCode = NO_ERROR;
    }

    return returnCode;
}

static ErrorCode_t DynamicString_append(DynamicString * dstring,
        const char * c, size_t len) {
    ErrorCode_t returnCode = ERROR_FAILED;

    returnCode = DynamicString_allocateIfNeeded(dstring, len);

    if (NO_ERROR == returnCode) {
        memcpy(&(dstring->content[dstring->stringLenght]), c, len * sizeof (char));
        dstring->content[dstring->stringLenght + len] = '\0';
        dstring->stringLenght = dstring->stringLenght + len;
    }

    return returnCode;
}

ErrorCode_t DynamicString_appendChar(DynamicString * dstring, char c) {
    ErrorCode_t returnCode = ERROR_FAILED;

    returnCode = DynamicString_append(dstring, &c, 1U);

    return returnCode;
}

ErrorCode_t DynamicString_appendString(DynamicString * dstring, const char * string) {
    ErrorCode_t returnCode = ERROR_FAILED;

    returnCode = DynamicString_append(dstring, string, strlen(string));

    return returnCode;
}

ErrorCode_t DynamicString_appendCharSequence(DynamicString * dstring, const char * seq, size_t seqLen) {
    ErrorCode_t returnCode = ERROR_FAILED;

    returnCode = DynamicString_append(dstring, seq, seqLen);

    return returnCode;
}

ErrorCode_t DynamicString_destroy(DynamicString * dstring) {
    ErrorCode_t returnCode = ERROR_FAILED;

    returnCode = Allocator_free((void *) dstring->content);
    
    if (NO_ERROR == returnCode) {
        dstring->content = NULL;
        dstring->currentSize = 0;
        dstring->stringLenght = 0;
    }

    return returnCode;
}