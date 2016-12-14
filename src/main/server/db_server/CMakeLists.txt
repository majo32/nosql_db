set( db_server_files 
    DBServer.c
)

add_library(db_server ${db_server_files} )

target_link_libraries(db_server server)
target_link_libraries(db_server reader)