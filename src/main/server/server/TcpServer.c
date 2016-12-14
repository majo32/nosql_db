#include <server/TcpServer.h>

static ErrorCode_t TcpServer_findFreeConnection(TcpServer * server, TcpServerConnection ** connection) {
    ErrorCode_t returnCode = ERROR_FAILED;
    Uint32_t i;
    Bool_t found = FALSE;

    for (i = 0; (i < MAX_CONNECTIONS) && (FALSE == found); i++) {
        if (TcpServer_checkConnection(&(server->connections[i])) == FALSE) {
            TcpServer_closeConnection(&(server->connections[i]));
        }
        if (FALSE == server->connections[i].running) {
            *connection = &(server->connections[i]);
            found = TRUE;
        }
    }
    if (TRUE == found) {
        returnCode = NO_ERROR;
    } else {
        returnCode = ERROR_OBJECT_NOT_FOUND;
    }
    return returnCode;
}

static void * TcpServer_serverThreadFunction(void * data) {
    ErrorCode_t returnCode = ERROR_FAILED;
    TcpServer * server = (TcpServer *) data;
    TcpServerConnection * connection;
    int sockfd;
    while (1) {
        sockfd = accept(server->socketFd, (struct sockaddr*) &server->clientAddr, &server->clientLen);
        if (sockfd < 0) {
            perror("ERROR: Could not accept");
        } else {
            returnCode = TcpServer_findFreeConnection(server, &connection);
            if (NO_ERROR == returnCode) {
                connection->socketFd = sockfd;
                connection->running = TRUE;
                connection->data = server->data;
                if (pthread_create(&(connection->thread), NULL, server->serverFunction,
                        (void*) connection) < 0) {
                    TcpServer_closeConnection(connection);
                    perror("ERROR: Could not create thread");
                }
            } else {
                close(sockfd);
                printf("WARNINNG: Too many connections\n");
            }
        }
    }
}

ErrorCode_t TcpServer_start(TcpServer * server, Uint32_t port,
        TcpServerFunction fn, void * data) {
    ErrorCode_t returnCode = ERROR_FAILED;
    Uint32_t i;

    server->port = port;
    server->data = data;

    bzero(&(server->serverAddr), sizeof (server->serverAddr));

    server->serverAddr.sin_family = AF_INET;
    server->serverAddr.sin_addr.s_addr = INADDR_ANY;
    server->serverAddr.sin_port = htons(port);

    server->serverFunction = fn;

    server->socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->socketFd < 0) {
        returnCode = ERROR_CREATE_ERROR;
    } else {
        returnCode = NO_ERROR;
    }
    if (NO_ERROR == returnCode) {
        if (bind(server->socketFd, (struct sockaddr*) &(server->serverAddr),
                sizeof (server->serverAddr)) < 0) {
            returnCode = ERROR_CREATE_ERROR;
        } else {
            returnCode = NO_ERROR;
        }
    }
    if (NO_ERROR == returnCode) {
        listen(server->socketFd, SOCKET_BUFFER_SIZE);
        server->clientLen = sizeof (server->clientAddr);

        for (i = 0; i < MAX_CONNECTIONS; i++) {
            server->connections[i].running = FALSE;
        }
        if (pthread_create(&(server->serverThread), NULL, TcpServer_serverThreadFunction,
                (void*) server) < 0) {
            returnCode = ERROR_CREATE_ERROR;
        }
    }
    return returnCode;
}

ErrorCode_t TcpServer_join(TcpServer * server) {
    ErrorCode_t returnCode = ERROR_FAILED;

    if (pthread_join(server->serverThread, NULL) < 0) {
        returnCode = ERROR_FAILED;
    } else {
        returnCode = NO_ERROR;
    }

    return returnCode;
}

ErrorCode_t TcpServer_stop(TcpServer * server) {
    ErrorCode_t returnCode = ERROR_FAILED;
    Uint32_t i;

    returnCode = NO_ERROR;


    for (i = 0; i < MAX_CONNECTIONS; i++) {
        TcpServer_closeConnection(&(server->connections[i]));
    }

    pthread_cancel(server->serverThread);
    close(server->socketFd);

    return returnCode;
}

ErrorCode_t TcpServer_closeConnection(TcpServerConnection * connection) {
    ErrorCode_t returnCode = ERROR_FAILED;

    returnCode = NO_ERROR;

    connection->running = FALSE;
    pthread_cancel(connection->thread);
    close(connection->socketFd);

    return returnCode;
}

ErrorCode_t TcpServer_readLine(TcpServerConnection * connection, DynamicString * string) {
    ErrorCode_t returnCode = ERROR_FAILED;
    char c;
    int readRetval;
    Bool_t end = FALSE;
    returnCode = DynamicString_empty(string);

    do {
        if (TcpServer_checkConnection(connection) == FALSE) {
            returnCode = ERROR_FAILED;
        } else {
            readRetval = read(connection->socketFd, &c, 1);
            if (readRetval < 0) {
                returnCode = ERROR_FAILED;
            } else if (readRetval == 0) {
                end = TRUE;
            } else {
                if (NO_ERROR == returnCode) {
                    if ((c != '\n') && (c != '\r')) {
                        returnCode = DynamicString_appendChar(string, c);
                    }
                }
            }
        }
    } while ((NO_ERROR == returnCode) && (c != '\n') && (c != '\r') && (FALSE == end));
    return returnCode;
}

ErrorCode_t TcpServer_write(TcpServerConnection * connection, const DynamicString * string) {
    ErrorCode_t returnCode = ERROR_FAILED;
    char * s;
    size_t size;

    returnCode = DynamicString_getLength(string, &size);

    if (NO_ERROR == returnCode) {
        returnCode = DynamicString_getString(string, &s);
    }

    if (write(connection->socketFd, s, size * sizeof (char)) < 0) {
        returnCode = ERROR_FAILED;
    }

    return returnCode;
}

Bool_t TcpServer_checkConnection(TcpServerConnection * connection) {
    if (fcntl(connection->socketFd, F_GETFD) != -1 || errno != EBADF) {
        return TRUE;
    } else {
        return FALSE;
    }
}