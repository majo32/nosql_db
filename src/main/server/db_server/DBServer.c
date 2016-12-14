#include <db_server/DBServer.h>
#include <reader/Reader.h>
#include <string.h>

static ErrorCode_t DBServer_parseRequest(DBServer * dbServer, DynamicString * string) {
    ErrorCode_t returnCode = ERROR_FAILED;
    char * s = NULL;
    Bool_t found = FALSE;
    char * keyStart = NULL;
    char * valStart = NULL;
    char command[4];
    size_t size;

    DynamicString key;
    DynamicString val;

    returnCode = DynamicString_getString(string, &s);

    if (NO_ERROR == returnCode) {
        returnCode = DynamicString_getLength(string, &size);
    }

    if (NO_ERROR == returnCode) {
        returnCode = DynamicString_create(&key);
    }

    if (NO_ERROR == returnCode) {
        returnCode = DynamicString_create(&val);
    }

    if (NO_ERROR == returnCode) {

        Reader_getPart(0, s, command, strlen(s), 4);
        if (size > 4) {
            keyStart = strstr(s, " ");
            if (NULL != keyStart) {
                valStart = strstr(keyStart + 1, " ");
                if (NULL != valStart) {
                    DynamicString_appendCharSequence(&key, keyStart + 1, valStart - keyStart - 1);
                    DynamicString_appendString(&val, valStart + 1);
                } else {
                    DynamicString_appendString(&key, keyStart + 1);
                }
            }

            returnCode = DynamicString_empty(string);

            if (NO_ERROR == returnCode) {
                if (strcmp("GET", command) == 0) {
                    returnCode = NoSqlDB_get(dbServer->db, &key, string, &found);
                } else if (strcmp("SET", command) == 0) {
                    returnCode = NoSqlDB_set(dbServer->db, &key, &val);
                } else if (strcmp("DEL", command) == 0) {
                    returnCode = NoSqlDB_del(dbServer->db, &key);
                } else {
                    printf("Invalid command\n");
                }
            }
            if (NO_ERROR == returnCode) {
                DynamicString_appendString(string, "\n");
            }
        }
    }

    DynamicString_destroy(&val);
    DynamicString_destroy(&key);

    return returnCode;
}

static void* DBServer_serverFunction(void * data) {
    TcpServerConnection *connection = (TcpServerConnection *) data;
    DBServer * server = (DBServer *) connection->data;
    ErrorCode_t returnCode = ERROR_FAILED;
    DynamicString string;

    returnCode = DynamicString_create(&string);
    printf("Connection open\n");
    while (NO_ERROR == returnCode) {
        returnCode = TcpServer_readLine(connection, &string);

        if (TcpServer_checkConnection(connection) == FALSE) {
            returnCode = ERROR_FAILED;
        }
        if (NO_ERROR == returnCode) {
            returnCode = DBServer_parseRequest(server, &string);
        }
        if (NO_ERROR == returnCode) {

            returnCode = TcpServer_write(connection, &string);
        }
    }
    printf("Closing connection\n");

    DynamicString_destroy(&string);
    TcpServer_closeConnection(connection);
}

ErrorCode_t DBServer_start(DBServer * dbServer, Uint32_t port, NoSqlDB * db) {
    ErrorCode_t returnCode = ERROR_FAILED;

    dbServer->db = db;

    returnCode = TcpServer_start(&(dbServer->server), port,
            DBServer_serverFunction, dbServer);

    return returnCode;
}

ErrorCode_t DBServer_stop(DBServer * dbServer) {
    ErrorCode_t returnCode = ERROR_FAILED;

    returnCode = TcpServer_stop(&(dbServer->server));

    return returnCode;
}