#ifndef NOSQLDBALLOCATOR_H
#define NOSQLDBALLOCATOR_H

#include <Common.h>

#ifdef __cplusplus
extern "C" {
#endif
    
    ErrorCode_t Allocator_alloc(void ** dest, size_t size);
    ErrorCode_t Allocator_realloc(void ** dest, void * src, size_t size);
    ErrorCode_t Allocator_free(void * dest);
  
#ifdef __cplusplus
}
#endif

#endif /* NOSQLDBALLOCATOR_H */

