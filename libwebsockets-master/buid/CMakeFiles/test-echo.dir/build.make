# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/fabio/Scrivania/libwebsockets-master

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/fabio/Scrivania/libwebsockets-master/buid

# Include any dependencies generated for this target.
include CMakeFiles/test-echo.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test-echo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test-echo.dir/flags.make

CMakeFiles/test-echo.dir/test-server/test-echo.c.o: CMakeFiles/test-echo.dir/flags.make
CMakeFiles/test-echo.dir/test-server/test-echo.c.o: ../test-server/test-echo.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/fabio/Scrivania/libwebsockets-master/buid/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/test-echo.dir/test-server/test-echo.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/test-echo.dir/test-server/test-echo.c.o   -c /home/fabio/Scrivania/libwebsockets-master/test-server/test-echo.c

CMakeFiles/test-echo.dir/test-server/test-echo.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test-echo.dir/test-server/test-echo.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/fabio/Scrivania/libwebsockets-master/test-server/test-echo.c > CMakeFiles/test-echo.dir/test-server/test-echo.c.i

CMakeFiles/test-echo.dir/test-server/test-echo.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test-echo.dir/test-server/test-echo.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/fabio/Scrivania/libwebsockets-master/test-server/test-echo.c -o CMakeFiles/test-echo.dir/test-server/test-echo.c.s

CMakeFiles/test-echo.dir/test-server/test-echo.c.o.requires:
.PHONY : CMakeFiles/test-echo.dir/test-server/test-echo.c.o.requires

CMakeFiles/test-echo.dir/test-server/test-echo.c.o.provides: CMakeFiles/test-echo.dir/test-server/test-echo.c.o.requires
	$(MAKE) -f CMakeFiles/test-echo.dir/build.make CMakeFiles/test-echo.dir/test-server/test-echo.c.o.provides.build
.PHONY : CMakeFiles/test-echo.dir/test-server/test-echo.c.o.provides

CMakeFiles/test-echo.dir/test-server/test-echo.c.o.provides.build: CMakeFiles/test-echo.dir/test-server/test-echo.c.o

# Object files for target test-echo
test__echo_OBJECTS = \
"CMakeFiles/test-echo.dir/test-server/test-echo.c.o"

# External object files for target test-echo
test__echo_EXTERNAL_OBJECTS =

bin/libwebsockets-test-echo: CMakeFiles/test-echo.dir/test-server/test-echo.c.o
bin/libwebsockets-test-echo: CMakeFiles/test-echo.dir/build.make
bin/libwebsockets-test-echo: lib/libwebsockets.a
bin/libwebsockets-test-echo: /usr/lib/x86_64-linux-gnu/libz.so
bin/libwebsockets-test-echo: /usr/lib/x86_64-linux-gnu/libssl.so
bin/libwebsockets-test-echo: /usr/lib/x86_64-linux-gnu/libcrypto.so
bin/libwebsockets-test-echo: CMakeFiles/test-echo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable bin/libwebsockets-test-echo"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test-echo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test-echo.dir/build: bin/libwebsockets-test-echo
.PHONY : CMakeFiles/test-echo.dir/build

CMakeFiles/test-echo.dir/requires: CMakeFiles/test-echo.dir/test-server/test-echo.c.o.requires
.PHONY : CMakeFiles/test-echo.dir/requires

CMakeFiles/test-echo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test-echo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test-echo.dir/clean

CMakeFiles/test-echo.dir/depend:
	cd /home/fabio/Scrivania/libwebsockets-master/buid && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/fabio/Scrivania/libwebsockets-master /home/fabio/Scrivania/libwebsockets-master /home/fabio/Scrivania/libwebsockets-master/buid /home/fabio/Scrivania/libwebsockets-master/buid /home/fabio/Scrivania/libwebsockets-master/buid/CMakeFiles/test-echo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test-echo.dir/depend
