# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/joaoroberto/CLionProjects/ProjetoFinalEletronicaDigital

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/joaoroberto/CLionProjects/ProjetoFinalEletronicaDigital/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/TrabalhoFinalED.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/TrabalhoFinalED.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/TrabalhoFinalED.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TrabalhoFinalED.dir/flags.make

CMakeFiles/TrabalhoFinalED.dir/main.c.o: CMakeFiles/TrabalhoFinalED.dir/flags.make
CMakeFiles/TrabalhoFinalED.dir/main.c.o: /Users/joaoroberto/CLionProjects/ProjetoFinalEletronicaDigital/main.c
CMakeFiles/TrabalhoFinalED.dir/main.c.o: CMakeFiles/TrabalhoFinalED.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/joaoroberto/CLionProjects/ProjetoFinalEletronicaDigital/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/TrabalhoFinalED.dir/main.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/TrabalhoFinalED.dir/main.c.o -MF CMakeFiles/TrabalhoFinalED.dir/main.c.o.d -o CMakeFiles/TrabalhoFinalED.dir/main.c.o -c /Users/joaoroberto/CLionProjects/ProjetoFinalEletronicaDigital/main.c

CMakeFiles/TrabalhoFinalED.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/TrabalhoFinalED.dir/main.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/joaoroberto/CLionProjects/ProjetoFinalEletronicaDigital/main.c > CMakeFiles/TrabalhoFinalED.dir/main.c.i

CMakeFiles/TrabalhoFinalED.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/TrabalhoFinalED.dir/main.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/joaoroberto/CLionProjects/ProjetoFinalEletronicaDigital/main.c -o CMakeFiles/TrabalhoFinalED.dir/main.c.s

CMakeFiles/TrabalhoFinalED.dir/filaDeComandos.c.o: CMakeFiles/TrabalhoFinalED.dir/flags.make
CMakeFiles/TrabalhoFinalED.dir/filaDeComandos.c.o: /Users/joaoroberto/CLionProjects/ProjetoFinalEletronicaDigital/filaDeComandos.c
CMakeFiles/TrabalhoFinalED.dir/filaDeComandos.c.o: CMakeFiles/TrabalhoFinalED.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/joaoroberto/CLionProjects/ProjetoFinalEletronicaDigital/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/TrabalhoFinalED.dir/filaDeComandos.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/TrabalhoFinalED.dir/filaDeComandos.c.o -MF CMakeFiles/TrabalhoFinalED.dir/filaDeComandos.c.o.d -o CMakeFiles/TrabalhoFinalED.dir/filaDeComandos.c.o -c /Users/joaoroberto/CLionProjects/ProjetoFinalEletronicaDigital/filaDeComandos.c

CMakeFiles/TrabalhoFinalED.dir/filaDeComandos.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/TrabalhoFinalED.dir/filaDeComandos.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/joaoroberto/CLionProjects/ProjetoFinalEletronicaDigital/filaDeComandos.c > CMakeFiles/TrabalhoFinalED.dir/filaDeComandos.c.i

CMakeFiles/TrabalhoFinalED.dir/filaDeComandos.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/TrabalhoFinalED.dir/filaDeComandos.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/joaoroberto/CLionProjects/ProjetoFinalEletronicaDigital/filaDeComandos.c -o CMakeFiles/TrabalhoFinalED.dir/filaDeComandos.c.s

# Object files for target TrabalhoFinalED
TrabalhoFinalED_OBJECTS = \
"CMakeFiles/TrabalhoFinalED.dir/main.c.o" \
"CMakeFiles/TrabalhoFinalED.dir/filaDeComandos.c.o"

# External object files for target TrabalhoFinalED
TrabalhoFinalED_EXTERNAL_OBJECTS =

TrabalhoFinalED: CMakeFiles/TrabalhoFinalED.dir/main.c.o
TrabalhoFinalED: CMakeFiles/TrabalhoFinalED.dir/filaDeComandos.c.o
TrabalhoFinalED: CMakeFiles/TrabalhoFinalED.dir/build.make
TrabalhoFinalED: CMakeFiles/TrabalhoFinalED.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/joaoroberto/CLionProjects/ProjetoFinalEletronicaDigital/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable TrabalhoFinalED"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TrabalhoFinalED.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TrabalhoFinalED.dir/build: TrabalhoFinalED
.PHONY : CMakeFiles/TrabalhoFinalED.dir/build

CMakeFiles/TrabalhoFinalED.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TrabalhoFinalED.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TrabalhoFinalED.dir/clean

CMakeFiles/TrabalhoFinalED.dir/depend:
	cd /Users/joaoroberto/CLionProjects/ProjetoFinalEletronicaDigital/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/joaoroberto/CLionProjects/ProjetoFinalEletronicaDigital /Users/joaoroberto/CLionProjects/ProjetoFinalEletronicaDigital /Users/joaoroberto/CLionProjects/ProjetoFinalEletronicaDigital/cmake-build-debug /Users/joaoroberto/CLionProjects/ProjetoFinalEletronicaDigital/cmake-build-debug /Users/joaoroberto/CLionProjects/ProjetoFinalEletronicaDigital/cmake-build-debug/CMakeFiles/TrabalhoFinalED.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/TrabalhoFinalED.dir/depend

