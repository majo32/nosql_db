/* 
 * File:   TcpServer.h
 * Author: majo
 *
 * Created on December 14, 2016, 10:12 AM
 */

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

#include <Common.h>
#include <dynamic_string/DynamicString.h>


#ifdef __cplusplus
extern "C" {
#endif

#define MAX_CONNECTIONS 100
#define SOCKET_BUFFER_SIZE 250
#define MAX_BUFFER 1024

    typedef struct {
        int socketFd;
        pthread_t thread;
        Bool_t running;
        void * data;
    } TcpServerConnection;
    
    typedef void* (*TcpServerFunction) (void*);

    typedef struct {
        Uint32_t port;
        int socketFd;
        socklen_t clientLen;
        struct sockaddr_in serverAddr;
        struct sockaddr_in clientAddr;
        TcpServerConnection connections [MAX_CONNECTIONS];
        TcpServerFunction serverFunction;
        pthread_t serverThread;
        void * data;

    } TcpServer;



    ErrorCode_t TcpServer_start(TcpServer * server, Uint32_t port, 
            TcpServerFunction fn, void * data);
    ErrorCode_t TcpServer_join(TcpServer * server);
    ErrorCode_t TcpServer_stop(TcpServer * server);
    
    ErrorCode_t TcpServer_closeConnection(TcpServerConnection * connection);
    ErrorCode_t TcpServer_readLine(TcpServerConnection * connection, DynamicString * string);
    ErrorCode_t TcpServer_write(TcpServerConnection * connection, const DynamicString * string);
    Bool_t TcpServer_checkConnection(TcpServerConnection * connection);

#ifdef __cplusplus
}
#endif

#endif /* TCPSERVER_H */

