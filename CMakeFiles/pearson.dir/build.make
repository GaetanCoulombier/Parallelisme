# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/gaetan/Documents/[01] Pro/[01] Cours/[03] Master/M2/Parallelisme/Projet/Parallelisme"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/gaetan/Documents/[01] Pro/[01] Cours/[03] Master/M2/Parallelisme/Projet/Parallelisme"

# Include any dependencies generated for this target.
include CMakeFiles/pearson.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/pearson.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/pearson.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/pearson.dir/flags.make

CMakeFiles/pearson.dir/src/pearson.cpp.o: CMakeFiles/pearson.dir/flags.make
CMakeFiles/pearson.dir/src/pearson.cpp.o: src/pearson.cpp
CMakeFiles/pearson.dir/src/pearson.cpp.o: CMakeFiles/pearson.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/home/gaetan/Documents/[01] Pro/[01] Cours/[03] Master/M2/Parallelisme/Projet/Parallelisme/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/pearson.dir/src/pearson.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/pearson.dir/src/pearson.cpp.o -MF CMakeFiles/pearson.dir/src/pearson.cpp.o.d -o CMakeFiles/pearson.dir/src/pearson.cpp.o -c "/home/gaetan/Documents/[01] Pro/[01] Cours/[03] Master/M2/Parallelisme/Projet/Parallelisme/src/pearson.cpp"

CMakeFiles/pearson.dir/src/pearson.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/pearson.dir/src/pearson.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/gaetan/Documents/[01] Pro/[01] Cours/[03] Master/M2/Parallelisme/Projet/Parallelisme/src/pearson.cpp" > CMakeFiles/pearson.dir/src/pearson.cpp.i

CMakeFiles/pearson.dir/src/pearson.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/pearson.dir/src/pearson.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/gaetan/Documents/[01] Pro/[01] Cours/[03] Master/M2/Parallelisme/Projet/Parallelisme/src/pearson.cpp" -o CMakeFiles/pearson.dir/src/pearson.cpp.s

# Object files for target pearson
pearson_OBJECTS = \
"CMakeFiles/pearson.dir/src/pearson.cpp.o"

# External object files for target pearson
pearson_EXTERNAL_OBJECTS =

bin/Release/pearson: CMakeFiles/pearson.dir/src/pearson.cpp.o
bin/Release/pearson: CMakeFiles/pearson.dir/build.make
bin/Release/pearson: CMakeFiles/pearson.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/home/gaetan/Documents/[01] Pro/[01] Cours/[03] Master/M2/Parallelisme/Projet/Parallelisme/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bin/Release/pearson"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pearson.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/pearson.dir/build: bin/Release/pearson
.PHONY : CMakeFiles/pearson.dir/build

CMakeFiles/pearson.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/pearson.dir/cmake_clean.cmake
.PHONY : CMakeFiles/pearson.dir/clean

CMakeFiles/pearson.dir/depend:
	cd "/home/gaetan/Documents/[01] Pro/[01] Cours/[03] Master/M2/Parallelisme/Projet/Parallelisme" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/gaetan/Documents/[01] Pro/[01] Cours/[03] Master/M2/Parallelisme/Projet/Parallelisme" "/home/gaetan/Documents/[01] Pro/[01] Cours/[03] Master/M2/Parallelisme/Projet/Parallelisme" "/home/gaetan/Documents/[01] Pro/[01] Cours/[03] Master/M2/Parallelisme/Projet/Parallelisme" "/home/gaetan/Documents/[01] Pro/[01] Cours/[03] Master/M2/Parallelisme/Projet/Parallelisme" "/home/gaetan/Documents/[01] Pro/[01] Cours/[03] Master/M2/Parallelisme/Projet/Parallelisme/CMakeFiles/pearson.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/pearson.dir/depend

