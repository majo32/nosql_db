/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NoSqlDB.h
 * Author: majo
 *
 * Created on December 7, 2016, 11:06 PM
 */

#ifndef NOSQLDB_H
#define NOSQLDB_H

#include <nosql_db/NoSqlDBHandler.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        NoSqlDBHandler handler;
        DynamicString keyBuffer;
        NoSqlDBBlockSet blockSetReadBuffer;
        NoSqlDBBlockSet blockSetWriteBuffer;
    } NoSqlDB;

    ErrorCode_t NoSqlDB_open(NoSqlDB * db, const char * filename);
    ErrorCode_t NoSqlDB_close(NoSqlDB * db);
    ErrorCode_t NoSqlDB_get(NoSqlDB * db, const DynamicString * key, DynamicString * val, Bool_t * found);
    ErrorCode_t NoSqlDB_set(NoSqlDB * db, const DynamicString * key, const DynamicString * val);
    ErrorCode_t NoSqlDB_del(NoSqlDB * db, const DynamicString * key);
    
#ifdef __cplusplus
}
#endif

#endif /* NOSQLDB_H */

