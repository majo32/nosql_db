#include <file_utils/FileHandler.h>
#include <unistd.h>

ErrorCode_t FileHandler_open(FileHandler * filehandler, const char * fileName) {
    ErrorCode_t returnCode = ERROR_FAILED;

    filehandler->file = fopen(fileName, "rb+");

    returnCode = NO_ERROR;

    return returnCode;
}

ErrorCode_t FileHandler_create(FileHandler * filehandler, const char * fileName) {
    ErrorCode_t returnCode = ERROR_FAILED;

    FILE * f = fopen(fileName, "w");

    if (NULL != f) {
        int retval;

        retval = fclose(f);

        if (0U == retval) {
            returnCode = NO_ERROR;
        }

        if (NO_ERROR == returnCode) {
            returnCode = FileHandler_open(filehandler, fileName);
        }
    }

    return returnCode;
}

ErrorCode_t FileHandler_close(FileHandler * filehandler) {
    ErrorCode_t returnCode = ERROR_FAILED;
    int retval;

    retval = fclose(filehandler->file);

    if (0U == retval) {
        returnCode = NO_ERROR;
    }

    return returnCode;
}

ErrorCode_t FileHandler_exists(const char * fileName, Bool_t * exists) {
    ErrorCode_t returnCode = ERROR_FAILED;

    if (access(fileName, F_OK) != -1) {
        *exists = TRUE;
        returnCode = NO_ERROR;
    } else {
        *exists = FALSE;
        returnCode = NO_ERROR;
    }

    return returnCode;
}

ErrorCode_t FileHandler_read(FileHandler * filehandler, void * dest, size_t size) {
    ErrorCode_t returnCode = ERROR_FAILED;
    size_t retval;

    retval = fread(dest, size, 1U, filehandler->file);

    if (1U == retval) {
        returnCode = NO_ERROR;
    }

    return returnCode;
}

ErrorCode_t FileHandler_write(FileHandler * filehandler, const void * src, size_t size) {
    ErrorCode_t returnCode = ERROR_FAILED;
    size_t retval;

    retval = fwrite(src, size, 1U, filehandler->file);

    if (1U == retval) {
        returnCode = NO_ERROR;
    }
    return returnCode;
}

ErrorCode_t FileHandler_navigateToTop(FileHandler * filehandler) {
    ErrorCode_t returnCode = ERROR_FAILED;
    int retval;

    rewind(filehandler->file);

    returnCode = NO_ERROR;

    return returnCode;
}

ErrorCode_t FileHandler_navigateToBottom(FileHandler * filehandler) {
    ErrorCode_t returnCode = ERROR_FAILED;
    int retval;

    retval = fseek(filehandler->file, 0, SEEK_END);

    if (0U == retval) {
        returnCode = NO_ERROR;
    }

    return returnCode;
}

ErrorCode_t FileHandler_navigateToPos(FileHandler * filehandler, FilePosition_t pos) {
    ErrorCode_t returnCode = ERROR_FAILED;
    int retval;

    retval = fseek(filehandler->file, pos, SEEK_SET);

    if (0U == retval) {
        returnCode = NO_ERROR;
    }

    return returnCode;
}

ErrorCode_t FileHandler_getCurrentPos(FileHandler * filehandler, FilePosition_t * pos) {
    ErrorCode_t returnCode = ERROR_FAILED;

    *pos = ftell(filehandler->file);

    returnCode = NO_ERROR;

    return returnCode;
}

ErrorCode_t FileHandler_getFileSize(FileHandler * filehandler, size_t * filesize) {
    ErrorCode_t returnCode = ERROR_FAILED;
    FilePosition_t pos;
    FilePosition_t endPos;

    returnCode = FileHandler_getCurrentPos(filehandler, &pos);

    if (NO_ERROR == returnCode) {
        returnCode = FileHandler_navigateToBottom(filehandler);
        if (NO_ERROR == returnCode) {
            returnCode = FileHandler_getCurrentPos(filehandler, &endPos);
            if (NO_ERROR == returnCode) {
                returnCode = FileHandler_navigateToPos(filehandler, pos);
                if (NO_ERROR == returnCode) {
                    *filesize = endPos;
                }
            }
        }
    }

    return returnCode;
}
