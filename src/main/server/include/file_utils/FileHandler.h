#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <Common.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
extern "C" {
#endif
    
    typedef Ulong_t FilePosition_t;

    typedef struct {
        FILE * file;
    } FileHandler;
    
    ErrorCode_t FileHandler_open(FileHandler * filehandler, const char * fileName);
    
    ErrorCode_t FileHandler_create(FileHandler * filehandler, const char * fileName);
    
    ErrorCode_t FileHandler_close(FileHandler * filehandler);
    
    ErrorCode_t FileHandler_exists(const char * fileName, Bool_t * exists);

    ErrorCode_t FileHandler_read(FileHandler * filehandler, void * dest, size_t size);
    
    ErrorCode_t FileHandler_write(FileHandler * filehandler, const void * src, size_t size);
    
    ErrorCode_t FileHandler_navigateToTop(FileHandler * filehandler);

    ErrorCode_t FileHandler_navigateToBottom(FileHandler * filehandler);
    
    ErrorCode_t FileHandler_navigateToPos(FileHandler * filehandler, FilePosition_t pos);

    ErrorCode_t FileHandler_getCurrentPos(FileHandler * filehandler, FilePosition_t * pos);
    
    ErrorCode_t FileHandler_getFileSize(FileHandler * filehandler, size_t * filesize);

#ifdef __cplusplus
}
#endif

#endif /* FILEHANDLER_H */

