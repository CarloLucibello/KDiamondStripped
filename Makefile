# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

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

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running interactive CMake command-line interface..."
	/usr/bin/cmake -i .
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/carlo/Dropbox/workspace/KDiamondStripped/CMakeFiles /home/carlo/Dropbox/workspace/KDiamondStripped/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/carlo/Dropbox/workspace/KDiamondStripped/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named kdiamond-stripped

# Build rule for target.
kdiamond-stripped: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 kdiamond-stripped
.PHONY : kdiamond-stripped

# fast build rule for target.
kdiamond-stripped/fast:
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/build
.PHONY : kdiamond-stripped/fast

board.o: board.cpp.o
.PHONY : board.o

# target to build an object file
board.cpp.o:
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/board.cpp.o
.PHONY : board.cpp.o

board.i: board.cpp.i
.PHONY : board.i

# target to preprocess a source file
board.cpp.i:
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/board.cpp.i
.PHONY : board.cpp.i

board.s: board.cpp.s
.PHONY : board.s

# target to generate assembly for a file
board.cpp.s:
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/board.cpp.s
.PHONY : board.cpp.s

diamond.o: diamond.cpp.o
.PHONY : diamond.o

# target to build an object file
diamond.cpp.o:
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/diamond.cpp.o
.PHONY : diamond.cpp.o

diamond.i: diamond.cpp.i
.PHONY : diamond.i

# target to preprocess a source file
diamond.cpp.i:
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/diamond.cpp.i
.PHONY : diamond.cpp.i

diamond.s: diamond.cpp.s
.PHONY : diamond.s

# target to generate assembly for a file
diamond.cpp.s:
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/diamond.cpp.s
.PHONY : diamond.cpp.s

game.o: game.cpp.o
.PHONY : game.o

# target to build an object file
game.cpp.o:
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/game.cpp.o
.PHONY : game.cpp.o

game.i: game.cpp.i
.PHONY : game.i

# target to preprocess a source file
game.cpp.i:
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/game.cpp.i
.PHONY : game.cpp.i

game.s: game.cpp.s
.PHONY : game.s

# target to generate assembly for a file
game.cpp.s:
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/game.cpp.s
.PHONY : game.cpp.s

gameparams.o: gameparams.cpp.o
.PHONY : gameparams.o

# target to build an object file
gameparams.cpp.o:
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/gameparams.cpp.o
.PHONY : gameparams.cpp.o

gameparams.i: gameparams.cpp.i
.PHONY : gameparams.i

# target to preprocess a source file
gameparams.cpp.i:
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/gameparams.cpp.i
.PHONY : gameparams.cpp.i

gameparams.s: gameparams.cpp.s
.PHONY : gameparams.s

# target to generate assembly for a file
gameparams.cpp.s:
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/gameparams.cpp.s
.PHONY : gameparams.cpp.s

gamestate.o: gamestate.cpp.o
.PHONY : gamestate.o

# target to build an object file
gamestate.cpp.o:
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/gamestate.cpp.o
.PHONY : gamestate.cpp.o

gamestate.i: gamestate.cpp.i
.PHONY : gamestate.i

# target to preprocess a source file
gamestate.cpp.i:
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/gamestate.cpp.i
.PHONY : gamestate.cpp.i

gamestate.s: gamestate.cpp.s
.PHONY : gamestate.s

# target to generate assembly for a file
gamestate.cpp.s:
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/gamestate.cpp.s
.PHONY : gamestate.cpp.s

main.o: main.cpp.o
.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i
.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s
.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) -f CMakeFiles/kdiamond-stripped.dir/build.make CMakeFiles/kdiamond-stripped.dir/main.cpp.s
.PHONY : main.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... kdiamond-stripped"
	@echo "... rebuild_cache"
	@echo "... board.o"
	@echo "... board.i"
	@echo "... board.s"
	@echo "... diamond.o"
	@echo "... diamond.i"
	@echo "... diamond.s"
	@echo "... game.o"
	@echo "... game.i"
	@echo "... game.s"
	@echo "... gameparams.o"
	@echo "... gameparams.i"
	@echo "... gameparams.s"
	@echo "... gamestate.o"
	@echo "... gamestate.i"
	@echo "... gamestate.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

