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
CMAKE_SOURCE_DIR = /home/cjsatnarine/Code/Raytracer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cjsatnarine/Code/Raytracer/build/Debug

# Include any dependencies generated for this target.
include CMakeFiles/Raytracer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Raytracer.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Raytracer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Raytracer.dir/flags.make

CMakeFiles/Raytracer.dir/ImageGenerator.cpp.o: CMakeFiles/Raytracer.dir/flags.make
CMakeFiles/Raytracer.dir/ImageGenerator.cpp.o: /home/cjsatnarine/Code/Raytracer/ImageGenerator.cpp
CMakeFiles/Raytracer.dir/ImageGenerator.cpp.o: CMakeFiles/Raytracer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/cjsatnarine/Code/Raytracer/build/Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Raytracer.dir/ImageGenerator.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Raytracer.dir/ImageGenerator.cpp.o -MF CMakeFiles/Raytracer.dir/ImageGenerator.cpp.o.d -o CMakeFiles/Raytracer.dir/ImageGenerator.cpp.o -c /home/cjsatnarine/Code/Raytracer/ImageGenerator.cpp

CMakeFiles/Raytracer.dir/ImageGenerator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Raytracer.dir/ImageGenerator.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cjsatnarine/Code/Raytracer/ImageGenerator.cpp > CMakeFiles/Raytracer.dir/ImageGenerator.cpp.i

CMakeFiles/Raytracer.dir/ImageGenerator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Raytracer.dir/ImageGenerator.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cjsatnarine/Code/Raytracer/ImageGenerator.cpp -o CMakeFiles/Raytracer.dir/ImageGenerator.cpp.s

# Object files for target Raytracer
Raytracer_OBJECTS = \
"CMakeFiles/Raytracer.dir/ImageGenerator.cpp.o"

# External object files for target Raytracer
Raytracer_EXTERNAL_OBJECTS =

Raytracer: CMakeFiles/Raytracer.dir/ImageGenerator.cpp.o
Raytracer: CMakeFiles/Raytracer.dir/build.make
Raytracer: CMakeFiles/Raytracer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/cjsatnarine/Code/Raytracer/build/Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Raytracer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Raytracer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Raytracer.dir/build: Raytracer
.PHONY : CMakeFiles/Raytracer.dir/build

CMakeFiles/Raytracer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Raytracer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Raytracer.dir/clean

CMakeFiles/Raytracer.dir/depend:
	cd /home/cjsatnarine/Code/Raytracer/build/Debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cjsatnarine/Code/Raytracer /home/cjsatnarine/Code/Raytracer /home/cjsatnarine/Code/Raytracer/build/Debug /home/cjsatnarine/Code/Raytracer/build/Debug /home/cjsatnarine/Code/Raytracer/build/Debug/CMakeFiles/Raytracer.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/Raytracer.dir/depend

