#include <allocator/Allocator.h>
#include <stdlib.h>


ErrorCode_t Allocator_alloc(void ** dest, size_t size) {
    ErrorCode_t returnCode = ERROR_FAILED;

    *dest = malloc(size);
    
    if(NULL != (*dest)){
        returnCode = NO_ERROR;
    }
    
    return returnCode;
}

ErrorCode_t Allocator_realloc(void ** dest, void * src, size_t size) {
    ErrorCode_t returnCode = ERROR_FAILED;

    *dest = realloc(src, size);
    
    if((NULL == (*dest))){
        if(0 == size){
            returnCode = NO_ERROR;
        }
    }else{
        returnCode = NO_ERROR;
    }
    
    return returnCode;
}

ErrorCode_t Allocator_free(void * dest) {
    ErrorCode_t returnCode = ERROR_FAILED;
    
    free(dest);
    
    returnCode = NO_ERROR;

    return returnCode;
}