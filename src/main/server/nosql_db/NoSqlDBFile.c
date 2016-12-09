#include <nosql_db/NoSqlDBFile.h>

ErrorCode_t NoSqlDBFile_open(NoSqlDBFile * filehandler, const char * filename) {
    ErrorCode_t returnCode = ERROR_FAILED;

    returnCode = FileHandler_open(&(filehandler->fileHandler), filename);

    return returnCode;
}

ErrorCode_t NoSqlDBFile_create(NoSqlDBFile * filehandler, const char * filename) {
    ErrorCode_t returnCode = ERROR_FAILED;

    returnCode = FileHandler_create(&(filehandler->fileHandler), filename);

    return returnCode;
}

ErrorCode_t NoSqlDBFile_close(NoSqlDBFile * filehandler) {
    ErrorCode_t returnCode = ERROR_FAILED;

    returnCode = FileHandler_close(&(filehandler->fileHandler));

    return returnCode;
}

ErrorCode_t NoSqlDBFile_hasNextBlock(NoSqlDBFile * filehandler, Bool_t * endOfFile) {
    ErrorCode_t returnCode = ERROR_FAILED;
    size_t filesize;
    FilePosition_t pos;

    returnCode = FileHandler_getFileSize(&(filehandler->fileHandler), &filesize);
    if (NO_ERROR == returnCode) {
        returnCode = FileHandler_getCurrentPos(&(filehandler->fileHandler), &pos);
        if (NO_ERROR == returnCode) {
            if ((pos + sizeof (NoSqlDBBlock)) <= filesize) {
                *endOfFile = FALSE;
            } else {
                *endOfFile = TRUE;
            }
        }
    }

    return returnCode;
}

ErrorCode_t NoSqlDBFile_readNextBlock(NoSqlDBFile * filehandler, NoSqlDBBlock * block) {
    ErrorCode_t returnCode = ERROR_FAILED;

    returnCode = FileHandler_read(&(filehandler->fileHandler), block, sizeof (NoSqlDBBlock));

    return returnCode;
}

ErrorCode_t NoSqlDBFile_writeNextBlock(NoSqlDBFile * filehandler, const NoSqlDBBlock * block) {
    ErrorCode_t returnCode = ERROR_FAILED;

    returnCode = FileHandler_write(&(filehandler->fileHandler), block, sizeof (NoSqlDBBlock));

    return returnCode;
}

ErrorCode_t NoSqlDBFile_navigateToTop(NoSqlDBFile * filehandler){
    ErrorCode_t returnCode = ERROR_FAILED;

    returnCode = FileHandler_navigateToTop(&(filehandler->fileHandler));

    return returnCode;
}

ErrorCode_t NoSqlDBFile_navigateToBottom(NoSqlDBFile * filehandler){
    ErrorCode_t returnCode = ERROR_FAILED;

    returnCode = FileHandler_navigateToBottom(&(filehandler->fileHandler));

    return returnCode;
}

ErrorCode_t NoSqlDBFile_navigateToPos(NoSqlDBFile * filehandler, FilePosition_t pos){
    ErrorCode_t returnCode = ERROR_FAILED;

    returnCode = FileHandler_navigateToPos(&(filehandler->fileHandler),pos);

    return returnCode;
}

ErrorCode_t NoSqlDBFile_getPos(NoSqlDBFile * filehandler, FilePosition_t * pos){
    ErrorCode_t returnCode = ERROR_FAILED;

    returnCode = FileHandler_getCurrentPos(&(filehandler->fileHandler), pos);

    return returnCode;
}

