# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/dorian/code/blokboi

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dorian/code/blokboi

# Include any dependencies generated for this target.
include CMakeFiles/blokboi.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/blokboi.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/blokboi.dir/flags.make

CMakeFiles/blokboi.dir/src/game.cpp.o: CMakeFiles/blokboi.dir/flags.make
CMakeFiles/blokboi.dir/src/game.cpp.o: src/game.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dorian/code/blokboi/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/blokboi.dir/src/game.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/blokboi.dir/src/game.cpp.o -c /home/dorian/code/blokboi/src/game.cpp

CMakeFiles/blokboi.dir/src/game.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/blokboi.dir/src/game.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dorian/code/blokboi/src/game.cpp > CMakeFiles/blokboi.dir/src/game.cpp.i

CMakeFiles/blokboi.dir/src/game.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/blokboi.dir/src/game.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dorian/code/blokboi/src/game.cpp -o CMakeFiles/blokboi.dir/src/game.cpp.s

# Object files for target blokboi
blokboi_OBJECTS = \
"CMakeFiles/blokboi.dir/src/game.cpp.o"

# External object files for target blokboi
blokboi_EXTERNAL_OBJECTS =

blokboi: CMakeFiles/blokboi.dir/src/game.cpp.o
blokboi: CMakeFiles/blokboi.dir/build.make
blokboi: CMakeFiles/blokboi.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dorian/code/blokboi/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable blokboi"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/blokboi.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/blokboi.dir/build: blokboi

.PHONY : CMakeFiles/blokboi.dir/build

CMakeFiles/blokboi.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/blokboi.dir/cmake_clean.cmake
.PHONY : CMakeFiles/blokboi.dir/clean

CMakeFiles/blokboi.dir/depend:
	cd /home/dorian/code/blokboi && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dorian/code/blokboi /home/dorian/code/blokboi /home/dorian/code/blokboi /home/dorian/code/blokboi /home/dorian/code/blokboi/CMakeFiles/blokboi.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/blokboi.dir/depend
