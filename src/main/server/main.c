/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/* 
 * File:   main.c
 * Author: majo
 *
 * Created on December 14, 2016, 12:00 PM
 */

#include <server/TcpServer.h>
#include <db_server/DBServer.h>
#include <reader/Reader.h>

#define LINE_LEN 100
#define COMMAND_LEN 4

int main(int argc, char** argv) {
    DBServer server;
    const char * filename = "db.dat";
    NoSqlDB db;
    FileHandler filehandler;
    Bool_t exists;
    char input[LINE_LEN];
    char command[COMMAND_LEN];

    if (argc < 2) {
        printf("USAGE: %s port_num\nExiting \n", argv[0]);
        return 1;
    }


    if (FileHandler_exists(filename, &exists) == NO_ERROR) {
        if (FALSE == exists) {
            FileHandler_create(&filehandler, filename);
            FileHandler_close(&filehandler);
        }
    } else {
        printf("Error file!\n");
        exit(1);
    }

    if (NoSqlDB_open(&db, filename) != NO_ERROR) {
        printf("Error file!\n");
        exit(1);
    }

    if (DBServer_start(&server, atoi(argv[1]), &db)) {
        printf("Error server!\n");
        exit(1);
    }

    while (1) {
        Reader_readLine(input, LINE_LEN);
        Reader_getPart(0, input, command, LINE_LEN, COMMAND_LEN);
        if (strcmp(command, "END") == 0) {
            break;
        }
    }


    DBServer_stop(&server);
}

