#ifndef NOSQLDBUTILS_H
#define NOSQLDBUTILS_H

#include <Common.h>
#ifdef __cplusplus
extern "C" {
#endif

#define NOSQLDBBLOCK_SIZE 50

    typedef enum {
        NOSQL_DB_BLOCK_STATUS_UNASSIGNED,
        NOSQL_DB_BLOCK_STATUS_ASSIGNED,
        NOSQL_DB_BLOCK_STATUS_HEAD
    } NoSqlDBBlockStatus;

    typedef struct {
        NoSqlDBBlockStatus status;
        size_t size;
        char data[NOSQLDBBLOCK_SIZE];
    } NoSqlDBBlock;
    
#ifdef __cplusplus
}
#endif 
#endif /* NOSQLDBUTILS_H */

