cmake_minimum_required (VERSION 3.1)
project (contender)

# configure a header file to pass some of the CMake settings
# to the source code
#configure_file (
 # "${PROJECT_SOURCE_DIR}/TutorialConfig.h.in"
  #"${PROJECT_BINARY_DIR}/TutorialConfig.h"
  #)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h

#include_directories("${PROJECT_BINARY_DIR}")

add_subdirectory(src)

#add custom libraries to project
include_directories(${PROJECT_SOURCE_DIR}/src)
link_directories(${PROJECT_SOURCE_DIR}/src)


#add sqlite3 to project
include_directories(/usr/local/include/sqlite3)
link_directories(/usr/local/lib)

#add Threads package
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads)


# add the executables
add_executable(contender ${PROJECT_SOURCE_DIR}/app/contender.cpp)
target_link_libraries(contender contact)

#add exe for testing sqlite
add_executable(sqlitetest ${PROJECT_SOURCE_DIR}/app/sqlitetest.cpp)
target_link_libraries(sqlitetest Threads::Threads)  #link threads to exe
target_link_libraries(sqlitetest sqlitewrapper)
target_link_libraries(sqlitetest sqlite3)           #link sqlite3 library
target_link_libraries(sqlitetest ${CMAKE_DL_LIBS})  #link dllibs for sql3
