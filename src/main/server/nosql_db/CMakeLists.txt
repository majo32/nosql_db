set( nosql_db_files 
    NoSqlDBBlockSet.c
    NoSqlDBFile.c
    NoSqlDBHandler.c
    NoSqlDBUtils.c
    NoSqlDB.c
)

add_library(nosql_db ${nosql_db_files} )

target_link_libraries(nosql_db file_utils)
target_link_libraries(nosql_db dynamic_string)