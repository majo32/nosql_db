/* 
 * File:   DBServer.h
 * Author: majo
 *
 * Created on December 14, 2016, 1:23 PM
 */

#ifndef DBSERVER_H
#define DBSERVER_H

#include <nosql_db/NoSqlDB.h>
#include <server/TcpServer.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct{
        TcpServer server;
        NoSqlDB * db;
    }DBServer;
    
ErrorCode_t  DBServer_start(DBServer * dbServer, Uint32_t port, NoSqlDB * db);
ErrorCode_t  DBServer_stop(DBServer * dbServer);

#ifdef __cplusplus
}
#endif

#endif /* DBSERVER_H */

