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
CMAKE_SOURCE_DIR = /home/carlo/Dropbox/workspace/KDiamondStripped

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/carlo/Dropbox/workspace/KDiamondStripped

# Include any dependencies generated for this target.
include CMakeFiles/kdiamond-stripped.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/kdiamond-stripped.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/kdiamond-stripped.dir/flags.make

CMakeFiles/kdiamond-stripped.dir/main.cpp.o: CMakeFiles/kdiamond-stripped.dir/flags.make
CMakeFiles/kdiamond-stripped.dir/main.cpp.o: main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/carlo/Dropbox/workspace/KDiamondStripped/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/kdiamond-stripped.dir/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/kdiamond-stripped.dir/main.cpp.o -c /home/carlo/Dropbox/workspace/KDiamondStripped/main.cpp

CMakeFiles/kdiamond-stripped.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kdiamond-stripped.dir/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/carlo/Dropbox/workspace/KDiamondStripped/main.cpp > CMakeFiles/kdiamond-stripped.dir/main.cpp.i

CMakeFiles/kdiamond-stripped.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kdiamond-stripped.dir/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/carlo/Dropbox/workspace/KDiamondStripped/main.cpp -o CMakeFiles/kdiamond-stripped.dir/main.cpp.s

CMakeFiles/kdiamond-stripped.dir/main.cpp.o.requires:
.PHONY : CMakeFiles/kdiamond-stripped.dir/main.cpp.o.requires

CMakeFiles/kdiamond-stripped.dir/main.cpp.o.provides: CMakeFiles/kdiamond-stripped.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/kdiamond-stripped.dir/main.cpp.o.provides

CMakeFiles/kdiamond-stripped.dir/main.cpp.o.provides.build: CMakeFiles/kdiamond-stripped.dir/main.cpp.o

CMakeFiles/kdiamond-stripped.dir/benchmarksuite.cpp.o: CMakeFiles/kdiamond-stripped.dir/flags.make
CMakeFiles/kdiamond-stripped.dir/benchmarksuite.cpp.o: benchmarksuite.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/carlo/Dropbox/workspace/KDiamondStripped/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/kdiamond-stripped.dir/benchmarksuite.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/kdiamond-stripped.dir/benchmarksuite.cpp.o -c /home/carlo/Dropbox/workspace/KDiamondStripped/benchmarksuite.cpp

CMakeFiles/kdiamond-stripped.dir/benchmarksuite.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kdiamond-stripped.dir/benchmarksuite.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/carlo/Dropbox/workspace/KDiamondStripped/benchmarksuite.cpp > CMakeFiles/kdiamond-stripped.dir/benchmarksuite.cpp.i

CMakeFiles/kdiamond-stripped.dir/benchmarksuite.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kdiamond-stripped.dir/benchmarksuite.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/carlo/Dropbox/workspace/KDiamondStripped/benchmarksuite.cpp -o CMakeFiles/kdiamond-stripped.dir/benchmarksuite.cpp.s

CMakeFiles/kdiamond-stripped.dir/benchmarksuite.cpp.o.requires:
.PHONY : CMakeFiles/kdiamond-stripped.dir/benchmarksuite.cpp.o.requires

CMakeFiles/kdiamond-stripped.dir/benchmarksuite.cpp.o.provides: CMakeFiles/kdiamond-stripped.dir/benchmarksuite.cpp.o.requires
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/benchmarksuite.cpp.o.provides.build
.PHONY : CMakeFiles/kdiamond-stripped.dir/benchmarksuite.cpp.o.provides

CMakeFiles/kdiamond-stripped.dir/benchmarksuite.cpp.o.provides.build: CMakeFiles/kdiamond-stripped.dir/benchmarksuite.cpp.o

CMakeFiles/kdiamond-stripped.dir/gamestate.cpp.o: CMakeFiles/kdiamond-stripped.dir/flags.make
CMakeFiles/kdiamond-stripped.dir/gamestate.cpp.o: gamestate.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/carlo/Dropbox/workspace/KDiamondStripped/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/kdiamond-stripped.dir/gamestate.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/kdiamond-stripped.dir/gamestate.cpp.o -c /home/carlo/Dropbox/workspace/KDiamondStripped/gamestate.cpp

CMakeFiles/kdiamond-stripped.dir/gamestate.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kdiamond-stripped.dir/gamestate.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/carlo/Dropbox/workspace/KDiamondStripped/gamestate.cpp > CMakeFiles/kdiamond-stripped.dir/gamestate.cpp.i

CMakeFiles/kdiamond-stripped.dir/gamestate.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kdiamond-stripped.dir/gamestate.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/carlo/Dropbox/workspace/KDiamondStripped/gamestate.cpp -o CMakeFiles/kdiamond-stripped.dir/gamestate.cpp.s

CMakeFiles/kdiamond-stripped.dir/gamestate.cpp.o.requires:
.PHONY : CMakeFiles/kdiamond-stripped.dir/gamestate.cpp.o.requires

CMakeFiles/kdiamond-stripped.dir/gamestate.cpp.o.provides: CMakeFiles/kdiamond-stripped.dir/gamestate.cpp.o.requires
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/gamestate.cpp.o.provides.build
.PHONY : CMakeFiles/kdiamond-stripped.dir/gamestate.cpp.o.provides

CMakeFiles/kdiamond-stripped.dir/gamestate.cpp.o.provides.build: CMakeFiles/kdiamond-stripped.dir/gamestate.cpp.o

CMakeFiles/kdiamond-stripped.dir/gameparams.cpp.o: CMakeFiles/kdiamond-stripped.dir/flags.make
CMakeFiles/kdiamond-stripped.dir/gameparams.cpp.o: gameparams.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/carlo/Dropbox/workspace/KDiamondStripped/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/kdiamond-stripped.dir/gameparams.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/kdiamond-stripped.dir/gameparams.cpp.o -c /home/carlo/Dropbox/workspace/KDiamondStripped/gameparams.cpp

CMakeFiles/kdiamond-stripped.dir/gameparams.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kdiamond-stripped.dir/gameparams.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/carlo/Dropbox/workspace/KDiamondStripped/gameparams.cpp > CMakeFiles/kdiamond-stripped.dir/gameparams.cpp.i

CMakeFiles/kdiamond-stripped.dir/gameparams.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kdiamond-stripped.dir/gameparams.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/carlo/Dropbox/workspace/KDiamondStripped/gameparams.cpp -o CMakeFiles/kdiamond-stripped.dir/gameparams.cpp.s

CMakeFiles/kdiamond-stripped.dir/gameparams.cpp.o.requires:
.PHONY : CMakeFiles/kdiamond-stripped.dir/gameparams.cpp.o.requires

CMakeFiles/kdiamond-stripped.dir/gameparams.cpp.o.provides: CMakeFiles/kdiamond-stripped.dir/gameparams.cpp.o.requires
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/gameparams.cpp.o.provides.build
.PHONY : CMakeFiles/kdiamond-stripped.dir/gameparams.cpp.o.provides

CMakeFiles/kdiamond-stripped.dir/gameparams.cpp.o.provides.build: CMakeFiles/kdiamond-stripped.dir/gameparams.cpp.o

CMakeFiles/kdiamond-stripped.dir/game.cpp.o: CMakeFiles/kdiamond-stripped.dir/flags.make
CMakeFiles/kdiamond-stripped.dir/game.cpp.o: game.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/carlo/Dropbox/workspace/KDiamondStripped/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/kdiamond-stripped.dir/game.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/kdiamond-stripped.dir/game.cpp.o -c /home/carlo/Dropbox/workspace/KDiamondStripped/game.cpp

CMakeFiles/kdiamond-stripped.dir/game.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kdiamond-stripped.dir/game.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/carlo/Dropbox/workspace/KDiamondStripped/game.cpp > CMakeFiles/kdiamond-stripped.dir/game.cpp.i

CMakeFiles/kdiamond-stripped.dir/game.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kdiamond-stripped.dir/game.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/carlo/Dropbox/workspace/KDiamondStripped/game.cpp -o CMakeFiles/kdiamond-stripped.dir/game.cpp.s

CMakeFiles/kdiamond-stripped.dir/game.cpp.o.requires:
.PHONY : CMakeFiles/kdiamond-stripped.dir/game.cpp.o.requires

CMakeFiles/kdiamond-stripped.dir/game.cpp.o.provides: CMakeFiles/kdiamond-stripped.dir/game.cpp.o.requires
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/game.cpp.o.provides.build
.PHONY : CMakeFiles/kdiamond-stripped.dir/game.cpp.o.provides

CMakeFiles/kdiamond-stripped.dir/game.cpp.o.provides.build: CMakeFiles/kdiamond-stripped.dir/game.cpp.o

CMakeFiles/kdiamond-stripped.dir/board.cpp.o: CMakeFiles/kdiamond-stripped.dir/flags.make
CMakeFiles/kdiamond-stripped.dir/board.cpp.o: board.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/carlo/Dropbox/workspace/KDiamondStripped/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/kdiamond-stripped.dir/board.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/kdiamond-stripped.dir/board.cpp.o -c /home/carlo/Dropbox/workspace/KDiamondStripped/board.cpp

CMakeFiles/kdiamond-stripped.dir/board.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kdiamond-stripped.dir/board.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/carlo/Dropbox/workspace/KDiamondStripped/board.cpp > CMakeFiles/kdiamond-stripped.dir/board.cpp.i

CMakeFiles/kdiamond-stripped.dir/board.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kdiamond-stripped.dir/board.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/carlo/Dropbox/workspace/KDiamondStripped/board.cpp -o CMakeFiles/kdiamond-stripped.dir/board.cpp.s

CMakeFiles/kdiamond-stripped.dir/board.cpp.o.requires:
.PHONY : CMakeFiles/kdiamond-stripped.dir/board.cpp.o.requires

CMakeFiles/kdiamond-stripped.dir/board.cpp.o.provides: CMakeFiles/kdiamond-stripped.dir/board.cpp.o.requires
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/board.cpp.o.provides.build
.PHONY : CMakeFiles/kdiamond-stripped.dir/board.cpp.o.provides

CMakeFiles/kdiamond-stripped.dir/board.cpp.o.provides.build: CMakeFiles/kdiamond-stripped.dir/board.cpp.o

CMakeFiles/kdiamond-stripped.dir/diamond.cpp.o: CMakeFiles/kdiamond-stripped.dir/flags.make
CMakeFiles/kdiamond-stripped.dir/diamond.cpp.o: diamond.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/carlo/Dropbox/workspace/KDiamondStripped/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/kdiamond-stripped.dir/diamond.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/kdiamond-stripped.dir/diamond.cpp.o -c /home/carlo/Dropbox/workspace/KDiamondStripped/diamond.cpp

CMakeFiles/kdiamond-stripped.dir/diamond.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kdiamond-stripped.dir/diamond.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/carlo/Dropbox/workspace/KDiamondStripped/diamond.cpp > CMakeFiles/kdiamond-stripped.dir/diamond.cpp.i

CMakeFiles/kdiamond-stripped.dir/diamond.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kdiamond-stripped.dir/diamond.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/carlo/Dropbox/workspace/KDiamondStripped/diamond.cpp -o CMakeFiles/kdiamond-stripped.dir/diamond.cpp.s

CMakeFiles/kdiamond-stripped.dir/diamond.cpp.o.requires:
.PHONY : CMakeFiles/kdiamond-stripped.dir/diamond.cpp.o.requires

CMakeFiles/kdiamond-stripped.dir/diamond.cpp.o.provides: CMakeFiles/kdiamond-stripped.dir/diamond.cpp.o.requires
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/diamond.cpp.o.provides.build
.PHONY : CMakeFiles/kdiamond-stripped.dir/diamond.cpp.o.provides

CMakeFiles/kdiamond-stripped.dir/diamond.cpp.o.provides.build: CMakeFiles/kdiamond-stripped.dir/diamond.cpp.o

CMakeFiles/kdiamond-stripped.dir/randomcolor.cpp.o: CMakeFiles/kdiamond-stripped.dir/flags.make
CMakeFiles/kdiamond-stripped.dir/randomcolor.cpp.o: randomcolor.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/carlo/Dropbox/workspace/KDiamondStripped/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/kdiamond-stripped.dir/randomcolor.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/kdiamond-stripped.dir/randomcolor.cpp.o -c /home/carlo/Dropbox/workspace/KDiamondStripped/randomcolor.cpp

CMakeFiles/kdiamond-stripped.dir/randomcolor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kdiamond-stripped.dir/randomcolor.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/carlo/Dropbox/workspace/KDiamondStripped/randomcolor.cpp > CMakeFiles/kdiamond-stripped.dir/randomcolor.cpp.i

CMakeFiles/kdiamond-stripped.dir/randomcolor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kdiamond-stripped.dir/randomcolor.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/carlo/Dropbox/workspace/KDiamondStripped/randomcolor.cpp -o CMakeFiles/kdiamond-stripped.dir/randomcolor.cpp.s

CMakeFiles/kdiamond-stripped.dir/randomcolor.cpp.o.requires:
.PHONY : CMakeFiles/kdiamond-stripped.dir/randomcolor.cpp.o.requires

CMakeFiles/kdiamond-stripped.dir/randomcolor.cpp.o.provides: CMakeFiles/kdiamond-stripped.dir/randomcolor.cpp.o.requires
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/randomcolor.cpp.o.provides.build
.PHONY : CMakeFiles/kdiamond-stripped.dir/randomcolor.cpp.o.provides

CMakeFiles/kdiamond-stripped.dir/randomcolor.cpp.o.provides.build: CMakeFiles/kdiamond-stripped.dir/randomcolor.cpp.o

CMakeFiles/kdiamond-stripped.dir/mask.cpp.o: CMakeFiles/kdiamond-stripped.dir/flags.make
CMakeFiles/kdiamond-stripped.dir/mask.cpp.o: mask.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/carlo/Dropbox/workspace/KDiamondStripped/CMakeFiles $(CMAKE_PROGRESS_9)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/kdiamond-stripped.dir/mask.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/kdiamond-stripped.dir/mask.cpp.o -c /home/carlo/Dropbox/workspace/KDiamondStripped/mask.cpp

CMakeFiles/kdiamond-stripped.dir/mask.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kdiamond-stripped.dir/mask.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/carlo/Dropbox/workspace/KDiamondStripped/mask.cpp > CMakeFiles/kdiamond-stripped.dir/mask.cpp.i

CMakeFiles/kdiamond-stripped.dir/mask.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kdiamond-stripped.dir/mask.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/carlo/Dropbox/workspace/KDiamondStripped/mask.cpp -o CMakeFiles/kdiamond-stripped.dir/mask.cpp.s

CMakeFiles/kdiamond-stripped.dir/mask.cpp.o.requires:
.PHONY : CMakeFiles/kdiamond-stripped.dir/mask.cpp.o.requires

CMakeFiles/kdiamond-stripped.dir/mask.cpp.o.provides: CMakeFiles/kdiamond-stripped.dir/mask.cpp.o.requires
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/mask.cpp.o.provides.build
.PHONY : CMakeFiles/kdiamond-stripped.dir/mask.cpp.o.provides

CMakeFiles/kdiamond-stripped.dir/mask.cpp.o.provides.build: CMakeFiles/kdiamond-stripped.dir/mask.cpp.o

CMakeFiles/kdiamond-stripped.dir/figure.cpp.o: CMakeFiles/kdiamond-stripped.dir/flags.make
CMakeFiles/kdiamond-stripped.dir/figure.cpp.o: figure.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/carlo/Dropbox/workspace/KDiamondStripped/CMakeFiles $(CMAKE_PROGRESS_10)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/kdiamond-stripped.dir/figure.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/kdiamond-stripped.dir/figure.cpp.o -c /home/carlo/Dropbox/workspace/KDiamondStripped/figure.cpp

CMakeFiles/kdiamond-stripped.dir/figure.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kdiamond-stripped.dir/figure.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/carlo/Dropbox/workspace/KDiamondStripped/figure.cpp > CMakeFiles/kdiamond-stripped.dir/figure.cpp.i

CMakeFiles/kdiamond-stripped.dir/figure.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kdiamond-stripped.dir/figure.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/carlo/Dropbox/workspace/KDiamondStripped/figure.cpp -o CMakeFiles/kdiamond-stripped.dir/figure.cpp.s

CMakeFiles/kdiamond-stripped.dir/figure.cpp.o.requires:
.PHONY : CMakeFiles/kdiamond-stripped.dir/figure.cpp.o.requires

CMakeFiles/kdiamond-stripped.dir/figure.cpp.o.provides: CMakeFiles/kdiamond-stripped.dir/figure.cpp.o.requires
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/figure.cpp.o.provides.build
.PHONY : CMakeFiles/kdiamond-stripped.dir/figure.cpp.o.provides

CMakeFiles/kdiamond-stripped.dir/figure.cpp.o.provides.build: CMakeFiles/kdiamond-stripped.dir/figure.cpp.o

CMakeFiles/kdiamond-stripped.dir/player.cpp.o: CMakeFiles/kdiamond-stripped.dir/flags.make
CMakeFiles/kdiamond-stripped.dir/player.cpp.o: player.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/carlo/Dropbox/workspace/KDiamondStripped/CMakeFiles $(CMAKE_PROGRESS_11)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/kdiamond-stripped.dir/player.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/kdiamond-stripped.dir/player.cpp.o -c /home/carlo/Dropbox/workspace/KDiamondStripped/player.cpp

CMakeFiles/kdiamond-stripped.dir/player.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kdiamond-stripped.dir/player.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/carlo/Dropbox/workspace/KDiamondStripped/player.cpp > CMakeFiles/kdiamond-stripped.dir/player.cpp.i

CMakeFiles/kdiamond-stripped.dir/player.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kdiamond-stripped.dir/player.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/carlo/Dropbox/workspace/KDiamondStripped/player.cpp -o CMakeFiles/kdiamond-stripped.dir/player.cpp.s

CMakeFiles/kdiamond-stripped.dir/player.cpp.o.requires:
.PHONY : CMakeFiles/kdiamond-stripped.dir/player.cpp.o.requires

CMakeFiles/kdiamond-stripped.dir/player.cpp.o.provides: CMakeFiles/kdiamond-stripped.dir/player.cpp.o.requires
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/player.cpp.o.provides.build
.PHONY : CMakeFiles/kdiamond-stripped.dir/player.cpp.o.provides

CMakeFiles/kdiamond-stripped.dir/player.cpp.o.provides.build: CMakeFiles/kdiamond-stripped.dir/player.cpp.o

CMakeFiles/kdiamond-stripped.dir/point.cpp.o: CMakeFiles/kdiamond-stripped.dir/flags.make
CMakeFiles/kdiamond-stripped.dir/point.cpp.o: point.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/carlo/Dropbox/workspace/KDiamondStripped/CMakeFiles $(CMAKE_PROGRESS_12)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/kdiamond-stripped.dir/point.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/kdiamond-stripped.dir/point.cpp.o -c /home/carlo/Dropbox/workspace/KDiamondStripped/point.cpp

CMakeFiles/kdiamond-stripped.dir/point.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kdiamond-stripped.dir/point.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/carlo/Dropbox/workspace/KDiamondStripped/point.cpp > CMakeFiles/kdiamond-stripped.dir/point.cpp.i

CMakeFiles/kdiamond-stripped.dir/point.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kdiamond-stripped.dir/point.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/carlo/Dropbox/workspace/KDiamondStripped/point.cpp -o CMakeFiles/kdiamond-stripped.dir/point.cpp.s

CMakeFiles/kdiamond-stripped.dir/point.cpp.o.requires:
.PHONY : CMakeFiles/kdiamond-stripped.dir/point.cpp.o.requires

CMakeFiles/kdiamond-stripped.dir/point.cpp.o.provides: CMakeFiles/kdiamond-stripped.dir/point.cpp.o.requires
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/point.cpp.o.provides.build
.PHONY : CMakeFiles/kdiamond-stripped.dir/point.cpp.o.provides

CMakeFiles/kdiamond-stripped.dir/point.cpp.o.provides.build: CMakeFiles/kdiamond-stripped.dir/point.cpp.o

# Object files for target kdiamond-stripped
kdiamond__stripped_OBJECTS = \
"CMakeFiles/kdiamond-stripped.dir/main.cpp.o" \
"CMakeFiles/kdiamond-stripped.dir/benchmarksuite.cpp.o" \
"CMakeFiles/kdiamond-stripped.dir/gamestate.cpp.o" \
"CMakeFiles/kdiamond-stripped.dir/gameparams.cpp.o" \
"CMakeFiles/kdiamond-stripped.dir/game.cpp.o" \
"CMakeFiles/kdiamond-stripped.dir/board.cpp.o" \
"CMakeFiles/kdiamond-stripped.dir/diamond.cpp.o" \
"CMakeFiles/kdiamond-stripped.dir/randomcolor.cpp.o" \
"CMakeFiles/kdiamond-stripped.dir/mask.cpp.o" \
"CMakeFiles/kdiamond-stripped.dir/figure.cpp.o" \
"CMakeFiles/kdiamond-stripped.dir/player.cpp.o" \
"CMakeFiles/kdiamond-stripped.dir/point.cpp.o"

# External object files for target kdiamond-stripped
kdiamond__stripped_EXTERNAL_OBJECTS =

kdiamond-stripped: CMakeFiles/kdiamond-stripped.dir/main.cpp.o
kdiamond-stripped: CMakeFiles/kdiamond-stripped.dir/benchmarksuite.cpp.o
kdiamond-stripped: CMakeFiles/kdiamond-stripped.dir/gamestate.cpp.o
kdiamond-stripped: CMakeFiles/kdiamond-stripped.dir/gameparams.cpp.o
kdiamond-stripped: CMakeFiles/kdiamond-stripped.dir/game.cpp.o
kdiamond-stripped: CMakeFiles/kdiamond-stripped.dir/board.cpp.o
kdiamond-stripped: CMakeFiles/kdiamond-stripped.dir/diamond.cpp.o
kdiamond-stripped: CMakeFiles/kdiamond-stripped.dir/randomcolor.cpp.o
kdiamond-stripped: CMakeFiles/kdiamond-stripped.dir/mask.cpp.o
kdiamond-stripped: CMakeFiles/kdiamond-stripped.dir/figure.cpp.o
kdiamond-stripped: CMakeFiles/kdiamond-stripped.dir/player.cpp.o
kdiamond-stripped: CMakeFiles/kdiamond-stripped.dir/point.cpp.o
kdiamond-stripped: CMakeFiles/kdiamond-stripped.dir/build.make
kdiamond-stripped: CMakeFiles/kdiamond-stripped.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable kdiamond-stripped"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/kdiamond-stripped.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/kdiamond-stripped.dir/build: kdiamond-stripped
.PHONY : CMakeFiles/kdiamond-stripped.dir/build

CMakeFiles/kdiamond-stripped.dir/requires: CMakeFiles/kdiamond-stripped.dir/main.cpp.o.requires
CMakeFiles/kdiamond-stripped.dir/requires: CMakeFiles/kdiamond-stripped.dir/benchmarksuite.cpp.o.requires
CMakeFiles/kdiamond-stripped.dir/requires: CMakeFiles/kdiamond-stripped.dir/gamestate.cpp.o.requires
CMakeFiles/kdiamond-stripped.dir/requires: CMakeFiles/kdiamond-stripped.dir/gameparams.cpp.o.requires
CMakeFiles/kdiamond-stripped.dir/requires: CMakeFiles/kdiamond-stripped.dir/game.cpp.o.requires
CMakeFiles/kdiamond-stripped.dir/requires: CMakeFiles/kdiamond-stripped.dir/board.cpp.o.requires
CMakeFiles/kdiamond-stripped.dir/requires: CMakeFiles/kdiamond-stripped.dir/diamond.cpp.o.requires
CMakeFiles/kdiamond-stripped.dir/requires: CMakeFiles/kdiamond-stripped.dir/randomcolor.cpp.o.requires
CMakeFiles/kdiamond-stripped.dir/requires: CMakeFiles/kdiamond-stripped.dir/mask.cpp.o.requires
CMakeFiles/kdiamond-stripped.dir/requires: CMakeFiles/kdiamond-stripped.dir/figure.cpp.o.requires
CMakeFiles/kdiamond-stripped.dir/requires: CMakeFiles/kdiamond-stripped.dir/player.cpp.o.requires
CMakeFiles/kdiamond-stripped.dir/requires: CMakeFiles/kdiamond-stripped.dir/point.cpp.o.requires
.PHONY : CMakeFiles/kdiamond-stripped.dir/requires

CMakeFiles/kdiamond-stripped.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/kdiamond-stripped.dir/cmake_clean.cmake
.PHONY : CMakeFiles/kdiamond-stripped.dir/clean

CMakeFiles/kdiamond-stripped.dir/depend:
	cd /home/carlo/Dropbox/workspace/KDiamondStripped && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/carlo/Dropbox/workspace/KDiamondStripped /home/carlo/Dropbox/workspace/KDiamondStripped /home/carlo/Dropbox/workspace/KDiamondStripped /home/carlo/Dropbox/workspace/KDiamondStripped /home/carlo/Dropbox/workspace/KDiamondStripped/CMakeFiles/kdiamond-stripped.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/kdiamond-stripped.dir/depend

