# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_COMMAND = /home/pastad/lib/mxe/usr/x86_64-unknown-linux-gnu/bin/cmake

# The command to remove a file.
RM = /home/pastad/lib/mxe/usr/x86_64-unknown-linux-gnu/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pastad/repos/pastadEngine/external/tinyxml2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pastad/repos/pastadEngine/external/tinyxml2

# Include any dependencies generated for this target.
include CMakeFiles/test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test.dir/flags.make

CMakeFiles/test.dir/xmltest.cpp.obj: CMakeFiles/test.dir/flags.make
CMakeFiles/test.dir/xmltest.cpp.obj: CMakeFiles/test.dir/includes_CXX.rsp
CMakeFiles/test.dir/xmltest.cpp.obj: xmltest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pastad/repos/pastadEngine/external/tinyxml2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test.dir/xmltest.cpp.obj"
	/home/pastad/lib/mxe/usr/bin/x86_64-w64-mingw32.static-g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test.dir/xmltest.cpp.obj -c /home/pastad/repos/pastadEngine/external/tinyxml2/xmltest.cpp

CMakeFiles/test.dir/xmltest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/xmltest.cpp.i"
	/home/pastad/lib/mxe/usr/bin/x86_64-w64-mingw32.static-g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pastad/repos/pastadEngine/external/tinyxml2/xmltest.cpp > CMakeFiles/test.dir/xmltest.cpp.i

CMakeFiles/test.dir/xmltest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/xmltest.cpp.s"
	/home/pastad/lib/mxe/usr/bin/x86_64-w64-mingw32.static-g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pastad/repos/pastadEngine/external/tinyxml2/xmltest.cpp -o CMakeFiles/test.dir/xmltest.cpp.s

CMakeFiles/test.dir/xmltest.cpp.obj.requires:

.PHONY : CMakeFiles/test.dir/xmltest.cpp.obj.requires

CMakeFiles/test.dir/xmltest.cpp.obj.provides: CMakeFiles/test.dir/xmltest.cpp.obj.requires
	$(MAKE) -f CMakeFiles/test.dir/build.make CMakeFiles/test.dir/xmltest.cpp.obj.provides.build
.PHONY : CMakeFiles/test.dir/xmltest.cpp.obj.provides

CMakeFiles/test.dir/xmltest.cpp.obj.provides.build: CMakeFiles/test.dir/xmltest.cpp.obj


# Object files for target test
test_OBJECTS = \
"CMakeFiles/test.dir/xmltest.cpp.obj"

# External object files for target test
test_EXTERNAL_OBJECTS =

test.exe: CMakeFiles/test.dir/xmltest.cpp.obj
test.exe: CMakeFiles/test.dir/build.make
test.exe: libtinyxml2.dll.a
test.exe: CMakeFiles/test.dir/linklibs.rsp
test.exe: CMakeFiles/test.dir/objects1.rsp
test.exe: CMakeFiles/test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pastad/repos/pastadEngine/external/tinyxml2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test.dir/build: test.exe

.PHONY : CMakeFiles/test.dir/build

CMakeFiles/test.dir/requires: CMakeFiles/test.dir/xmltest.cpp.obj.requires

.PHONY : CMakeFiles/test.dir/requires

CMakeFiles/test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test.dir/clean

CMakeFiles/test.dir/depend:
	cd /home/pastad/repos/pastadEngine/external/tinyxml2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pastad/repos/pastadEngine/external/tinyxml2 /home/pastad/repos/pastadEngine/external/tinyxml2 /home/pastad/repos/pastadEngine/external/tinyxml2 /home/pastad/repos/pastadEngine/external/tinyxml2 /home/pastad/repos/pastadEngine/external/tinyxml2/CMakeFiles/test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test.dir/depend
