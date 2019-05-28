add_library(contact contact.cpp)
add_library(sqlitewrapper sqlitewrapper.cpp)



#add sqlite3 folders
include_directories(/usr/local/include/sqlite3)
link_directories(/usr/local/lib)
#link sqlite to sqlitewrapper
target_link_libraries(sqlitewrapper sqlite3)
target_link_libraries(sqlitewrapper contact)
