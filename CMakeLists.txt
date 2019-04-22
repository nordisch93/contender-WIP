cmake_minimum_required (VERSION 2.6)
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

include_directories(${PROJECT_SOURCE_DIR}/include/contender)
link_directories(${PROJECT_SOURCE_DIR}/include/contender)

# add the executable
add_executable(contender ${PROJECT_SOURCE_DIR}/app/contender.cpp)
target_link_libraries(contender contact)