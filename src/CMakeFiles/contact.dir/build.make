# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nordisch/Documents/Projects/contender

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nordisch/Documents/Projects/contender

# Include any dependencies generated for this target.
include src/CMakeFiles/contact.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/contact.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/contact.dir/flags.make

src/CMakeFiles/contact.dir/contact.cpp.o: src/CMakeFiles/contact.dir/flags.make
src/CMakeFiles/contact.dir/contact.cpp.o: src/contact.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nordisch/Documents/Projects/contender/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/contact.dir/contact.cpp.o"
	cd /home/nordisch/Documents/Projects/contender/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/contact.dir/contact.cpp.o -c /home/nordisch/Documents/Projects/contender/src/contact.cpp

src/CMakeFiles/contact.dir/contact.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/contact.dir/contact.cpp.i"
	cd /home/nordisch/Documents/Projects/contender/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nordisch/Documents/Projects/contender/src/contact.cpp > CMakeFiles/contact.dir/contact.cpp.i

src/CMakeFiles/contact.dir/contact.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/contact.dir/contact.cpp.s"
	cd /home/nordisch/Documents/Projects/contender/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nordisch/Documents/Projects/contender/src/contact.cpp -o CMakeFiles/contact.dir/contact.cpp.s

src/CMakeFiles/contact.dir/contact.cpp.o.requires:

.PHONY : src/CMakeFiles/contact.dir/contact.cpp.o.requires

src/CMakeFiles/contact.dir/contact.cpp.o.provides: src/CMakeFiles/contact.dir/contact.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/contact.dir/build.make src/CMakeFiles/contact.dir/contact.cpp.o.provides.build
.PHONY : src/CMakeFiles/contact.dir/contact.cpp.o.provides

src/CMakeFiles/contact.dir/contact.cpp.o.provides.build: src/CMakeFiles/contact.dir/contact.cpp.o


# Object files for target contact
contact_OBJECTS = \
"CMakeFiles/contact.dir/contact.cpp.o"

# External object files for target contact
contact_EXTERNAL_OBJECTS =

src/libcontact.a: src/CMakeFiles/contact.dir/contact.cpp.o
src/libcontact.a: src/CMakeFiles/contact.dir/build.make
src/libcontact.a: src/CMakeFiles/contact.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nordisch/Documents/Projects/contender/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libcontact.a"
	cd /home/nordisch/Documents/Projects/contender/src && $(CMAKE_COMMAND) -P CMakeFiles/contact.dir/cmake_clean_target.cmake
	cd /home/nordisch/Documents/Projects/contender/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/contact.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/contact.dir/build: src/libcontact.a

.PHONY : src/CMakeFiles/contact.dir/build

src/CMakeFiles/contact.dir/requires: src/CMakeFiles/contact.dir/contact.cpp.o.requires

.PHONY : src/CMakeFiles/contact.dir/requires

src/CMakeFiles/contact.dir/clean:
	cd /home/nordisch/Documents/Projects/contender/src && $(CMAKE_COMMAND) -P CMakeFiles/contact.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/contact.dir/clean

src/CMakeFiles/contact.dir/depend:
	cd /home/nordisch/Documents/Projects/contender && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nordisch/Documents/Projects/contender /home/nordisch/Documents/Projects/contender/src /home/nordisch/Documents/Projects/contender /home/nordisch/Documents/Projects/contender/src /home/nordisch/Documents/Projects/contender/src/CMakeFiles/contact.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/contact.dir/depend

