# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

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
CMAKE_COMMAND = /home/hengliy/Downloads/CLion-2017.3.3/clion-2017.3.3/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/hengliy/Downloads/CLion-2017.3.3/clion-2017.3.3/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/hengliy/codeHW/final/codeHW/ecs

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hengliy/codeHW/final/codeHW/ecs/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/ecs.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ecs.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ecs.dir/flags.make

CMakeFiles/ecs.dir/ExpoentialSmooth.cpp.o: CMakeFiles/ecs.dir/flags.make
CMakeFiles/ecs.dir/ExpoentialSmooth.cpp.o: ../ExpoentialSmooth.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hengliy/codeHW/final/codeHW/ecs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ecs.dir/ExpoentialSmooth.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ecs.dir/ExpoentialSmooth.cpp.o -c /home/hengliy/codeHW/final/codeHW/ecs/ExpoentialSmooth.cpp

CMakeFiles/ecs.dir/ExpoentialSmooth.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ecs.dir/ExpoentialSmooth.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hengliy/codeHW/final/codeHW/ecs/ExpoentialSmooth.cpp > CMakeFiles/ecs.dir/ExpoentialSmooth.cpp.i

CMakeFiles/ecs.dir/ExpoentialSmooth.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ecs.dir/ExpoentialSmooth.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hengliy/codeHW/final/codeHW/ecs/ExpoentialSmooth.cpp -o CMakeFiles/ecs.dir/ExpoentialSmooth.cpp.s

CMakeFiles/ecs.dir/ExpoentialSmooth.cpp.o.requires:

.PHONY : CMakeFiles/ecs.dir/ExpoentialSmooth.cpp.o.requires

CMakeFiles/ecs.dir/ExpoentialSmooth.cpp.o.provides: CMakeFiles/ecs.dir/ExpoentialSmooth.cpp.o.requires
	$(MAKE) -f CMakeFiles/ecs.dir/build.make CMakeFiles/ecs.dir/ExpoentialSmooth.cpp.o.provides.build
.PHONY : CMakeFiles/ecs.dir/ExpoentialSmooth.cpp.o.provides

CMakeFiles/ecs.dir/ExpoentialSmooth.cpp.o.provides.build: CMakeFiles/ecs.dir/ExpoentialSmooth.cpp.o


CMakeFiles/ecs.dir/PolynomialCurveFitting.cpp.o: CMakeFiles/ecs.dir/flags.make
CMakeFiles/ecs.dir/PolynomialCurveFitting.cpp.o: ../PolynomialCurveFitting.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hengliy/codeHW/final/codeHW/ecs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/ecs.dir/PolynomialCurveFitting.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ecs.dir/PolynomialCurveFitting.cpp.o -c /home/hengliy/codeHW/final/codeHW/ecs/PolynomialCurveFitting.cpp

CMakeFiles/ecs.dir/PolynomialCurveFitting.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ecs.dir/PolynomialCurveFitting.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hengliy/codeHW/final/codeHW/ecs/PolynomialCurveFitting.cpp > CMakeFiles/ecs.dir/PolynomialCurveFitting.cpp.i

CMakeFiles/ecs.dir/PolynomialCurveFitting.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ecs.dir/PolynomialCurveFitting.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hengliy/codeHW/final/codeHW/ecs/PolynomialCurveFitting.cpp -o CMakeFiles/ecs.dir/PolynomialCurveFitting.cpp.s

CMakeFiles/ecs.dir/PolynomialCurveFitting.cpp.o.requires:

.PHONY : CMakeFiles/ecs.dir/PolynomialCurveFitting.cpp.o.requires

CMakeFiles/ecs.dir/PolynomialCurveFitting.cpp.o.provides: CMakeFiles/ecs.dir/PolynomialCurveFitting.cpp.o.requires
	$(MAKE) -f CMakeFiles/ecs.dir/build.make CMakeFiles/ecs.dir/PolynomialCurveFitting.cpp.o.provides.build
.PHONY : CMakeFiles/ecs.dir/PolynomialCurveFitting.cpp.o.provides

CMakeFiles/ecs.dir/PolynomialCurveFitting.cpp.o.provides.build: CMakeFiles/ecs.dir/PolynomialCurveFitting.cpp.o


CMakeFiles/ecs.dir/date.cpp.o: CMakeFiles/ecs.dir/flags.make
CMakeFiles/ecs.dir/date.cpp.o: ../date.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hengliy/codeHW/final/codeHW/ecs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/ecs.dir/date.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ecs.dir/date.cpp.o -c /home/hengliy/codeHW/final/codeHW/ecs/date.cpp

CMakeFiles/ecs.dir/date.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ecs.dir/date.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hengliy/codeHW/final/codeHW/ecs/date.cpp > CMakeFiles/ecs.dir/date.cpp.i

CMakeFiles/ecs.dir/date.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ecs.dir/date.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hengliy/codeHW/final/codeHW/ecs/date.cpp -o CMakeFiles/ecs.dir/date.cpp.s

CMakeFiles/ecs.dir/date.cpp.o.requires:

.PHONY : CMakeFiles/ecs.dir/date.cpp.o.requires

CMakeFiles/ecs.dir/date.cpp.o.provides: CMakeFiles/ecs.dir/date.cpp.o.requires
	$(MAKE) -f CMakeFiles/ecs.dir/build.make CMakeFiles/ecs.dir/date.cpp.o.provides.build
.PHONY : CMakeFiles/ecs.dir/date.cpp.o.provides

CMakeFiles/ecs.dir/date.cpp.o.provides.build: CMakeFiles/ecs.dir/date.cpp.o


CMakeFiles/ecs.dir/ecs.cpp.o: CMakeFiles/ecs.dir/flags.make
CMakeFiles/ecs.dir/ecs.cpp.o: ../ecs.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hengliy/codeHW/final/codeHW/ecs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/ecs.dir/ecs.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ecs.dir/ecs.cpp.o -c /home/hengliy/codeHW/final/codeHW/ecs/ecs.cpp

CMakeFiles/ecs.dir/ecs.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ecs.dir/ecs.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hengliy/codeHW/final/codeHW/ecs/ecs.cpp > CMakeFiles/ecs.dir/ecs.cpp.i

CMakeFiles/ecs.dir/ecs.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ecs.dir/ecs.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hengliy/codeHW/final/codeHW/ecs/ecs.cpp -o CMakeFiles/ecs.dir/ecs.cpp.s

CMakeFiles/ecs.dir/ecs.cpp.o.requires:

.PHONY : CMakeFiles/ecs.dir/ecs.cpp.o.requires

CMakeFiles/ecs.dir/ecs.cpp.o.provides: CMakeFiles/ecs.dir/ecs.cpp.o.requires
	$(MAKE) -f CMakeFiles/ecs.dir/build.make CMakeFiles/ecs.dir/ecs.cpp.o.provides.build
.PHONY : CMakeFiles/ecs.dir/ecs.cpp.o.provides

CMakeFiles/ecs.dir/ecs.cpp.o.provides.build: CMakeFiles/ecs.dir/ecs.cpp.o


CMakeFiles/ecs.dir/fit.cpp.o: CMakeFiles/ecs.dir/flags.make
CMakeFiles/ecs.dir/fit.cpp.o: ../fit.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hengliy/codeHW/final/codeHW/ecs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/ecs.dir/fit.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ecs.dir/fit.cpp.o -c /home/hengliy/codeHW/final/codeHW/ecs/fit.cpp

CMakeFiles/ecs.dir/fit.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ecs.dir/fit.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hengliy/codeHW/final/codeHW/ecs/fit.cpp > CMakeFiles/ecs.dir/fit.cpp.i

CMakeFiles/ecs.dir/fit.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ecs.dir/fit.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hengliy/codeHW/final/codeHW/ecs/fit.cpp -o CMakeFiles/ecs.dir/fit.cpp.s

CMakeFiles/ecs.dir/fit.cpp.o.requires:

.PHONY : CMakeFiles/ecs.dir/fit.cpp.o.requires

CMakeFiles/ecs.dir/fit.cpp.o.provides: CMakeFiles/ecs.dir/fit.cpp.o.requires
	$(MAKE) -f CMakeFiles/ecs.dir/build.make CMakeFiles/ecs.dir/fit.cpp.o.provides.build
.PHONY : CMakeFiles/ecs.dir/fit.cpp.o.provides

CMakeFiles/ecs.dir/fit.cpp.o.provides.build: CMakeFiles/ecs.dir/fit.cpp.o


CMakeFiles/ecs.dir/init.cpp.o: CMakeFiles/ecs.dir/flags.make
CMakeFiles/ecs.dir/init.cpp.o: ../init.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hengliy/codeHW/final/codeHW/ecs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/ecs.dir/init.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ecs.dir/init.cpp.o -c /home/hengliy/codeHW/final/codeHW/ecs/init.cpp

CMakeFiles/ecs.dir/init.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ecs.dir/init.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hengliy/codeHW/final/codeHW/ecs/init.cpp > CMakeFiles/ecs.dir/init.cpp.i

CMakeFiles/ecs.dir/init.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ecs.dir/init.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hengliy/codeHW/final/codeHW/ecs/init.cpp -o CMakeFiles/ecs.dir/init.cpp.s

CMakeFiles/ecs.dir/init.cpp.o.requires:

.PHONY : CMakeFiles/ecs.dir/init.cpp.o.requires

CMakeFiles/ecs.dir/init.cpp.o.provides: CMakeFiles/ecs.dir/init.cpp.o.requires
	$(MAKE) -f CMakeFiles/ecs.dir/build.make CMakeFiles/ecs.dir/init.cpp.o.provides.build
.PHONY : CMakeFiles/ecs.dir/init.cpp.o.provides

CMakeFiles/ecs.dir/init.cpp.o.provides.build: CMakeFiles/ecs.dir/init.cpp.o


CMakeFiles/ecs.dir/io.cpp.o: CMakeFiles/ecs.dir/flags.make
CMakeFiles/ecs.dir/io.cpp.o: ../io.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hengliy/codeHW/final/codeHW/ecs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/ecs.dir/io.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ecs.dir/io.cpp.o -c /home/hengliy/codeHW/final/codeHW/ecs/io.cpp

CMakeFiles/ecs.dir/io.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ecs.dir/io.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hengliy/codeHW/final/codeHW/ecs/io.cpp > CMakeFiles/ecs.dir/io.cpp.i

CMakeFiles/ecs.dir/io.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ecs.dir/io.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hengliy/codeHW/final/codeHW/ecs/io.cpp -o CMakeFiles/ecs.dir/io.cpp.s

CMakeFiles/ecs.dir/io.cpp.o.requires:

.PHONY : CMakeFiles/ecs.dir/io.cpp.o.requires

CMakeFiles/ecs.dir/io.cpp.o.provides: CMakeFiles/ecs.dir/io.cpp.o.requires
	$(MAKE) -f CMakeFiles/ecs.dir/build.make CMakeFiles/ecs.dir/io.cpp.o.provides.build
.PHONY : CMakeFiles/ecs.dir/io.cpp.o.provides

CMakeFiles/ecs.dir/io.cpp.o.provides.build: CMakeFiles/ecs.dir/io.cpp.o


CMakeFiles/ecs.dir/predict.cpp.o: CMakeFiles/ecs.dir/flags.make
CMakeFiles/ecs.dir/predict.cpp.o: ../predict.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hengliy/codeHW/final/codeHW/ecs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/ecs.dir/predict.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ecs.dir/predict.cpp.o -c /home/hengliy/codeHW/final/codeHW/ecs/predict.cpp

CMakeFiles/ecs.dir/predict.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ecs.dir/predict.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hengliy/codeHW/final/codeHW/ecs/predict.cpp > CMakeFiles/ecs.dir/predict.cpp.i

CMakeFiles/ecs.dir/predict.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ecs.dir/predict.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hengliy/codeHW/final/codeHW/ecs/predict.cpp -o CMakeFiles/ecs.dir/predict.cpp.s

CMakeFiles/ecs.dir/predict.cpp.o.requires:

.PHONY : CMakeFiles/ecs.dir/predict.cpp.o.requires

CMakeFiles/ecs.dir/predict.cpp.o.provides: CMakeFiles/ecs.dir/predict.cpp.o.requires
	$(MAKE) -f CMakeFiles/ecs.dir/build.make CMakeFiles/ecs.dir/predict.cpp.o.provides.build
.PHONY : CMakeFiles/ecs.dir/predict.cpp.o.provides

CMakeFiles/ecs.dir/predict.cpp.o.provides.build: CMakeFiles/ecs.dir/predict.cpp.o


# Object files for target ecs
ecs_OBJECTS = \
"CMakeFiles/ecs.dir/ExpoentialSmooth.cpp.o" \
"CMakeFiles/ecs.dir/PolynomialCurveFitting.cpp.o" \
"CMakeFiles/ecs.dir/date.cpp.o" \
"CMakeFiles/ecs.dir/ecs.cpp.o" \
"CMakeFiles/ecs.dir/fit.cpp.o" \
"CMakeFiles/ecs.dir/init.cpp.o" \
"CMakeFiles/ecs.dir/io.cpp.o" \
"CMakeFiles/ecs.dir/predict.cpp.o"

# External object files for target ecs
ecs_EXTERNAL_OBJECTS =

../bin/ecs: CMakeFiles/ecs.dir/ExpoentialSmooth.cpp.o
../bin/ecs: CMakeFiles/ecs.dir/PolynomialCurveFitting.cpp.o
../bin/ecs: CMakeFiles/ecs.dir/date.cpp.o
../bin/ecs: CMakeFiles/ecs.dir/ecs.cpp.o
../bin/ecs: CMakeFiles/ecs.dir/fit.cpp.o
../bin/ecs: CMakeFiles/ecs.dir/init.cpp.o
../bin/ecs: CMakeFiles/ecs.dir/io.cpp.o
../bin/ecs: CMakeFiles/ecs.dir/predict.cpp.o
../bin/ecs: CMakeFiles/ecs.dir/build.make
../bin/ecs: CMakeFiles/ecs.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hengliy/codeHW/final/codeHW/ecs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX executable ../bin/ecs"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ecs.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ecs.dir/build: ../bin/ecs

.PHONY : CMakeFiles/ecs.dir/build

CMakeFiles/ecs.dir/requires: CMakeFiles/ecs.dir/ExpoentialSmooth.cpp.o.requires
CMakeFiles/ecs.dir/requires: CMakeFiles/ecs.dir/PolynomialCurveFitting.cpp.o.requires
CMakeFiles/ecs.dir/requires: CMakeFiles/ecs.dir/date.cpp.o.requires
CMakeFiles/ecs.dir/requires: CMakeFiles/ecs.dir/ecs.cpp.o.requires
CMakeFiles/ecs.dir/requires: CMakeFiles/ecs.dir/fit.cpp.o.requires
CMakeFiles/ecs.dir/requires: CMakeFiles/ecs.dir/init.cpp.o.requires
CMakeFiles/ecs.dir/requires: CMakeFiles/ecs.dir/io.cpp.o.requires
CMakeFiles/ecs.dir/requires: CMakeFiles/ecs.dir/predict.cpp.o.requires

.PHONY : CMakeFiles/ecs.dir/requires

CMakeFiles/ecs.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ecs.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ecs.dir/clean

CMakeFiles/ecs.dir/depend:
	cd /home/hengliy/codeHW/final/codeHW/ecs/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hengliy/codeHW/final/codeHW/ecs /home/hengliy/codeHW/final/codeHW/ecs /home/hengliy/codeHW/final/codeHW/ecs/cmake-build-debug /home/hengliy/codeHW/final/codeHW/ecs/cmake-build-debug /home/hengliy/codeHW/final/codeHW/ecs/cmake-build-debug/CMakeFiles/ecs.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ecs.dir/depend

