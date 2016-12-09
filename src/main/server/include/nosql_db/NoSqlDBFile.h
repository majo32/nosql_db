#ifndef NOSQLDBFILE_H
#define NOSQLDBFILE_H

#include <file_utils/FileHandler.h>
#include <nosql_db/NoSqlDBUtils.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct{
    
    } NoSqlDBFileHeader;
    
    typedef struct{
        FileHandler fileHandler;
    } NoSqlDBFile;
    
    ErrorCode_t NoSqlDBFile_open(NoSqlDBFile * filehandler, const char * filename);
    
    ErrorCode_t NoSqlDBFile_create(NoSqlDBFile * filehandler, const char * filename);
    
    ErrorCode_t NoSqlDBFile_close(NoSqlDBFile * filehandler);
    
    ErrorCode_t NoSqlDBFile_hasNextBlock(NoSqlDBFile * filehandler, Bool_t * endOfFile);
    
    ErrorCode_t NoSqlDBFile_readNextBlock(NoSqlDBFile * filehandler, NoSqlDBBlock * block);
    
    ErrorCode_t NoSqlDBFile_writeNextBlock(NoSqlDBFile * filehandler, const NoSqlDBBlock * block);
    
    ErrorCode_t NoSqlDBFile_navigateToTop(NoSqlDBFile * filehandler);

    ErrorCode_t NoSqlDBFile_navigateToBottom(NoSqlDBFile * filehandler);
    
    ErrorCode_t NoSqlDBFile_navigateToPos(NoSqlDBFile * filehandler, FilePosition_t pos);
    
    ErrorCode_t NoSqlDBFile_getPos(NoSqlDBFile * filehandler, FilePosition_t * pos);


#ifdef __cplusplus
}
#endif

#endif /* NOSQLDBFILE_H */

