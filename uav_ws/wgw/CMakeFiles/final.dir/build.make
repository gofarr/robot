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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/odroid/wgw

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/odroid/wgw

# Include any dependencies generated for this target.
include CMakeFiles/final.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/final.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/final.dir/flags.make

CMakeFiles/final.dir/final.cpp.o: CMakeFiles/final.dir/flags.make
CMakeFiles/final.dir/final.cpp.o: final.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/odroid/wgw/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/final.dir/final.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/final.dir/final.cpp.o -c /home/odroid/wgw/final.cpp

CMakeFiles/final.dir/final.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/final.dir/final.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/odroid/wgw/final.cpp > CMakeFiles/final.dir/final.cpp.i

CMakeFiles/final.dir/final.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/final.dir/final.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/odroid/wgw/final.cpp -o CMakeFiles/final.dir/final.cpp.s

CMakeFiles/final.dir/final.cpp.o.requires:
.PHONY : CMakeFiles/final.dir/final.cpp.o.requires

CMakeFiles/final.dir/final.cpp.o.provides: CMakeFiles/final.dir/final.cpp.o.requires
	$(MAKE) -f CMakeFiles/final.dir/build.make CMakeFiles/final.dir/final.cpp.o.provides.build
.PHONY : CMakeFiles/final.dir/final.cpp.o.provides

CMakeFiles/final.dir/final.cpp.o.provides.build: CMakeFiles/final.dir/final.cpp.o

# Object files for target final
final_OBJECTS = \
"CMakeFiles/final.dir/final.cpp.o"

# External object files for target final
final_EXTERNAL_OBJECTS =

final: CMakeFiles/final.dir/final.cpp.o
final: CMakeFiles/final.dir/build.make
final: /usr/lib/arm-linux-gnueabihf/libopencv_videostab.so.2.4.8
final: /usr/lib/arm-linux-gnueabihf/libopencv_video.so.2.4.8
final: /usr/lib/arm-linux-gnueabihf/libopencv_ts.so.2.4.8
final: /usr/lib/arm-linux-gnueabihf/libopencv_superres.so.2.4.8
final: /usr/lib/arm-linux-gnueabihf/libopencv_stitching.so.2.4.8
final: /usr/lib/arm-linux-gnueabihf/libopencv_photo.so.2.4.8
final: /usr/lib/arm-linux-gnueabihf/libopencv_ocl.so.2.4.8
final: /usr/lib/arm-linux-gnueabihf/libopencv_objdetect.so.2.4.8
final: /usr/lib/arm-linux-gnueabihf/libopencv_ml.so.2.4.8
final: /usr/lib/arm-linux-gnueabihf/libopencv_legacy.so.2.4.8
final: /usr/lib/arm-linux-gnueabihf/libopencv_imgproc.so.2.4.8
final: /usr/lib/arm-linux-gnueabihf/libopencv_highgui.so.2.4.8
final: /usr/lib/arm-linux-gnueabihf/libopencv_gpu.so.2.4.8
final: /usr/lib/arm-linux-gnueabihf/libopencv_flann.so.2.4.8
final: /usr/lib/arm-linux-gnueabihf/libopencv_features2d.so.2.4.8
final: /usr/lib/arm-linux-gnueabihf/libopencv_core.so.2.4.8
final: /usr/lib/arm-linux-gnueabihf/libopencv_contrib.so.2.4.8
final: /usr/lib/arm-linux-gnueabihf/libopencv_calib3d.so.2.4.8
final: /usr/lib/arm-linux-gnueabihf/libopencv_photo.so.2.4.8
final: /usr/lib/arm-linux-gnueabihf/libopencv_legacy.so.2.4.8
final: /usr/lib/arm-linux-gnueabihf/libopencv_video.so.2.4.8
final: /usr/lib/arm-linux-gnueabihf/libopencv_objdetect.so.2.4.8
final: /usr/lib/arm-linux-gnueabihf/libopencv_ml.so.2.4.8
final: /usr/lib/arm-linux-gnueabihf/libopencv_calib3d.so.2.4.8
final: /usr/lib/arm-linux-gnueabihf/libopencv_features2d.so.2.4.8
final: /usr/lib/arm-linux-gnueabihf/libopencv_highgui.so.2.4.8
final: /usr/lib/arm-linux-gnueabihf/libopencv_imgproc.so.2.4.8
final: /usr/lib/arm-linux-gnueabihf/libopencv_flann.so.2.4.8
final: /usr/lib/arm-linux-gnueabihf/libopencv_core.so.2.4.8
final: CMakeFiles/final.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable final"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/final.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/final.dir/build: final
.PHONY : CMakeFiles/final.dir/build

CMakeFiles/final.dir/requires: CMakeFiles/final.dir/final.cpp.o.requires
.PHONY : CMakeFiles/final.dir/requires

CMakeFiles/final.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/final.dir/cmake_clean.cmake
.PHONY : CMakeFiles/final.dir/clean

CMakeFiles/final.dir/depend:
	cd /home/odroid/wgw && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/odroid/wgw /home/odroid/wgw /home/odroid/wgw /home/odroid/wgw /home/odroid/wgw/CMakeFiles/final.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/final.dir/depend
