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
CMAKE_SOURCE_DIR = /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build

# Include any dependencies generated for this target.
include lib/CMakeFiles/simulation.dir/depend.make

# Include the progress variables for this target.
include lib/CMakeFiles/simulation.dir/progress.make

# Include the compile flags for this target's objects.
include lib/CMakeFiles/simulation.dir/flags.make

lib/CMakeFiles/simulation.dir/DAG.cpp.o: lib/CMakeFiles/simulation.dir/flags.make
lib/CMakeFiles/simulation.dir/DAG.cpp.o: ../lib/DAG.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object lib/CMakeFiles/simulation.dir/DAG.cpp.o"
	cd /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/lib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/simulation.dir/DAG.cpp.o -c /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/lib/DAG.cpp

lib/CMakeFiles/simulation.dir/DAG.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simulation.dir/DAG.cpp.i"
	cd /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/lib/DAG.cpp > CMakeFiles/simulation.dir/DAG.cpp.i

lib/CMakeFiles/simulation.dir/DAG.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simulation.dir/DAG.cpp.s"
	cd /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/lib/DAG.cpp -o CMakeFiles/simulation.dir/DAG.cpp.s

lib/CMakeFiles/simulation.dir/DAG.cpp.o.requires:

.PHONY : lib/CMakeFiles/simulation.dir/DAG.cpp.o.requires

lib/CMakeFiles/simulation.dir/DAG.cpp.o.provides: lib/CMakeFiles/simulation.dir/DAG.cpp.o.requires
	$(MAKE) -f lib/CMakeFiles/simulation.dir/build.make lib/CMakeFiles/simulation.dir/DAG.cpp.o.provides.build
.PHONY : lib/CMakeFiles/simulation.dir/DAG.cpp.o.provides

lib/CMakeFiles/simulation.dir/DAG.cpp.o.provides.build: lib/CMakeFiles/simulation.dir/DAG.cpp.o


lib/CMakeFiles/simulation.dir/RUNNABLE.cpp.o: lib/CMakeFiles/simulation.dir/flags.make
lib/CMakeFiles/simulation.dir/RUNNABLE.cpp.o: ../lib/RUNNABLE.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object lib/CMakeFiles/simulation.dir/RUNNABLE.cpp.o"
	cd /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/lib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/simulation.dir/RUNNABLE.cpp.o -c /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/lib/RUNNABLE.cpp

lib/CMakeFiles/simulation.dir/RUNNABLE.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simulation.dir/RUNNABLE.cpp.i"
	cd /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/lib/RUNNABLE.cpp > CMakeFiles/simulation.dir/RUNNABLE.cpp.i

lib/CMakeFiles/simulation.dir/RUNNABLE.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simulation.dir/RUNNABLE.cpp.s"
	cd /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/lib/RUNNABLE.cpp -o CMakeFiles/simulation.dir/RUNNABLE.cpp.s

lib/CMakeFiles/simulation.dir/RUNNABLE.cpp.o.requires:

.PHONY : lib/CMakeFiles/simulation.dir/RUNNABLE.cpp.o.requires

lib/CMakeFiles/simulation.dir/RUNNABLE.cpp.o.provides: lib/CMakeFiles/simulation.dir/RUNNABLE.cpp.o.requires
	$(MAKE) -f lib/CMakeFiles/simulation.dir/build.make lib/CMakeFiles/simulation.dir/RUNNABLE.cpp.o.provides.build
.PHONY : lib/CMakeFiles/simulation.dir/RUNNABLE.cpp.o.provides

lib/CMakeFiles/simulation.dir/RUNNABLE.cpp.o.provides.build: lib/CMakeFiles/simulation.dir/RUNNABLE.cpp.o


lib/CMakeFiles/simulation.dir/TASK.cpp.o: lib/CMakeFiles/simulation.dir/flags.make
lib/CMakeFiles/simulation.dir/TASK.cpp.o: ../lib/TASK.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object lib/CMakeFiles/simulation.dir/TASK.cpp.o"
	cd /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/lib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/simulation.dir/TASK.cpp.o -c /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/lib/TASK.cpp

lib/CMakeFiles/simulation.dir/TASK.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simulation.dir/TASK.cpp.i"
	cd /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/lib/TASK.cpp > CMakeFiles/simulation.dir/TASK.cpp.i

lib/CMakeFiles/simulation.dir/TASK.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simulation.dir/TASK.cpp.s"
	cd /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/lib/TASK.cpp -o CMakeFiles/simulation.dir/TASK.cpp.s

lib/CMakeFiles/simulation.dir/TASK.cpp.o.requires:

.PHONY : lib/CMakeFiles/simulation.dir/TASK.cpp.o.requires

lib/CMakeFiles/simulation.dir/TASK.cpp.o.provides: lib/CMakeFiles/simulation.dir/TASK.cpp.o.requires
	$(MAKE) -f lib/CMakeFiles/simulation.dir/build.make lib/CMakeFiles/simulation.dir/TASK.cpp.o.provides.build
.PHONY : lib/CMakeFiles/simulation.dir/TASK.cpp.o.provides

lib/CMakeFiles/simulation.dir/TASK.cpp.o.provides.build: lib/CMakeFiles/simulation.dir/TASK.cpp.o


lib/CMakeFiles/simulation.dir/communication.cpp.o: lib/CMakeFiles/simulation.dir/flags.make
lib/CMakeFiles/simulation.dir/communication.cpp.o: ../lib/communication.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object lib/CMakeFiles/simulation.dir/communication.cpp.o"
	cd /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/lib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/simulation.dir/communication.cpp.o -c /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/lib/communication.cpp

lib/CMakeFiles/simulation.dir/communication.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simulation.dir/communication.cpp.i"
	cd /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/lib/communication.cpp > CMakeFiles/simulation.dir/communication.cpp.i

lib/CMakeFiles/simulation.dir/communication.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simulation.dir/communication.cpp.s"
	cd /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/lib/communication.cpp -o CMakeFiles/simulation.dir/communication.cpp.s

lib/CMakeFiles/simulation.dir/communication.cpp.o.requires:

.PHONY : lib/CMakeFiles/simulation.dir/communication.cpp.o.requires

lib/CMakeFiles/simulation.dir/communication.cpp.o.provides: lib/CMakeFiles/simulation.dir/communication.cpp.o.requires
	$(MAKE) -f lib/CMakeFiles/simulation.dir/build.make lib/CMakeFiles/simulation.dir/communication.cpp.o.provides.build
.PHONY : lib/CMakeFiles/simulation.dir/communication.cpp.o.provides

lib/CMakeFiles/simulation.dir/communication.cpp.o.provides.build: lib/CMakeFiles/simulation.dir/communication.cpp.o


lib/CMakeFiles/simulation.dir/mapping.cpp.o: lib/CMakeFiles/simulation.dir/flags.make
lib/CMakeFiles/simulation.dir/mapping.cpp.o: ../lib/mapping.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object lib/CMakeFiles/simulation.dir/mapping.cpp.o"
	cd /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/lib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/simulation.dir/mapping.cpp.o -c /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/lib/mapping.cpp

lib/CMakeFiles/simulation.dir/mapping.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simulation.dir/mapping.cpp.i"
	cd /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/lib/mapping.cpp > CMakeFiles/simulation.dir/mapping.cpp.i

lib/CMakeFiles/simulation.dir/mapping.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simulation.dir/mapping.cpp.s"
	cd /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/lib/mapping.cpp -o CMakeFiles/simulation.dir/mapping.cpp.s

lib/CMakeFiles/simulation.dir/mapping.cpp.o.requires:

.PHONY : lib/CMakeFiles/simulation.dir/mapping.cpp.o.requires

lib/CMakeFiles/simulation.dir/mapping.cpp.o.provides: lib/CMakeFiles/simulation.dir/mapping.cpp.o.requires
	$(MAKE) -f lib/CMakeFiles/simulation.dir/build.make lib/CMakeFiles/simulation.dir/mapping.cpp.o.provides.build
.PHONY : lib/CMakeFiles/simulation.dir/mapping.cpp.o.provides

lib/CMakeFiles/simulation.dir/mapping.cpp.o.provides.build: lib/CMakeFiles/simulation.dir/mapping.cpp.o


lib/CMakeFiles/simulation.dir/simulation.cpp.o: lib/CMakeFiles/simulation.dir/flags.make
lib/CMakeFiles/simulation.dir/simulation.cpp.o: ../lib/simulation.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object lib/CMakeFiles/simulation.dir/simulation.cpp.o"
	cd /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/lib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/simulation.dir/simulation.cpp.o -c /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/lib/simulation.cpp

lib/CMakeFiles/simulation.dir/simulation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simulation.dir/simulation.cpp.i"
	cd /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/lib/simulation.cpp > CMakeFiles/simulation.dir/simulation.cpp.i

lib/CMakeFiles/simulation.dir/simulation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simulation.dir/simulation.cpp.s"
	cd /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/lib/simulation.cpp -o CMakeFiles/simulation.dir/simulation.cpp.s

lib/CMakeFiles/simulation.dir/simulation.cpp.o.requires:

.PHONY : lib/CMakeFiles/simulation.dir/simulation.cpp.o.requires

lib/CMakeFiles/simulation.dir/simulation.cpp.o.provides: lib/CMakeFiles/simulation.dir/simulation.cpp.o.requires
	$(MAKE) -f lib/CMakeFiles/simulation.dir/build.make lib/CMakeFiles/simulation.dir/simulation.cpp.o.provides.build
.PHONY : lib/CMakeFiles/simulation.dir/simulation.cpp.o.provides

lib/CMakeFiles/simulation.dir/simulation.cpp.o.provides.build: lib/CMakeFiles/simulation.dir/simulation.cpp.o


# Object files for target simulation
simulation_OBJECTS = \
"CMakeFiles/simulation.dir/DAG.cpp.o" \
"CMakeFiles/simulation.dir/RUNNABLE.cpp.o" \
"CMakeFiles/simulation.dir/TASK.cpp.o" \
"CMakeFiles/simulation.dir/communication.cpp.o" \
"CMakeFiles/simulation.dir/mapping.cpp.o" \
"CMakeFiles/simulation.dir/simulation.cpp.o"

# External object files for target simulation
simulation_EXTERNAL_OBJECTS =

lib/libsimulation.a: lib/CMakeFiles/simulation.dir/DAG.cpp.o
lib/libsimulation.a: lib/CMakeFiles/simulation.dir/RUNNABLE.cpp.o
lib/libsimulation.a: lib/CMakeFiles/simulation.dir/TASK.cpp.o
lib/libsimulation.a: lib/CMakeFiles/simulation.dir/communication.cpp.o
lib/libsimulation.a: lib/CMakeFiles/simulation.dir/mapping.cpp.o
lib/libsimulation.a: lib/CMakeFiles/simulation.dir/simulation.cpp.o
lib/libsimulation.a: lib/CMakeFiles/simulation.dir/build.make
lib/libsimulation.a: lib/CMakeFiles/simulation.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX static library libsimulation.a"
	cd /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/lib && $(CMAKE_COMMAND) -P CMakeFiles/simulation.dir/cmake_clean_target.cmake
	cd /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/lib && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/simulation.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/CMakeFiles/simulation.dir/build: lib/libsimulation.a

.PHONY : lib/CMakeFiles/simulation.dir/build

lib/CMakeFiles/simulation.dir/requires: lib/CMakeFiles/simulation.dir/DAG.cpp.o.requires
lib/CMakeFiles/simulation.dir/requires: lib/CMakeFiles/simulation.dir/RUNNABLE.cpp.o.requires
lib/CMakeFiles/simulation.dir/requires: lib/CMakeFiles/simulation.dir/TASK.cpp.o.requires
lib/CMakeFiles/simulation.dir/requires: lib/CMakeFiles/simulation.dir/communication.cpp.o.requires
lib/CMakeFiles/simulation.dir/requires: lib/CMakeFiles/simulation.dir/mapping.cpp.o.requires
lib/CMakeFiles/simulation.dir/requires: lib/CMakeFiles/simulation.dir/simulation.cpp.o.requires

.PHONY : lib/CMakeFiles/simulation.dir/requires

lib/CMakeFiles/simulation.dir/clean:
	cd /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/lib && $(CMAKE_COMMAND) -P CMakeFiles/simulation.dir/cmake_clean.cmake
.PHONY : lib/CMakeFiles/simulation.dir/clean

lib/CMakeFiles/simulation.dir/depend:
	cd /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/lib /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/lib /home/avees/AUTOSRA_TASK_E2E_SIMULATIUON/build/lib/CMakeFiles/simulation.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/CMakeFiles/simulation.dir/depend

