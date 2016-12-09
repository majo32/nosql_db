set( dynamic_string_files 
    DynamicString.c
)

add_library(dynamic_string ${dynamic_string_files} )

target_link_libraries(dynamic_string allocator)
