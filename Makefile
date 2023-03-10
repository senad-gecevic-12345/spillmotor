# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

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
CMAKE_SOURCE_DIR = /home/solidus/Programming/ECS/ECSAOENTHAUNTH

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/solidus/Programming/ECS/ECSAOENTHAUNTH

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/solidus/Programming/ECS/ECSAOENTHAUNTH/CMakeFiles /home/solidus/Programming/ECS/ECSAOENTHAUNTH//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/solidus/Programming/ECS/ECSAOENTHAUNTH/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named ECS

# Build rule for target.
ECS: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 ECS
.PHONY : ECS

# fast build rule for target.
ECS/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/build
.PHONY : ECS/fast

Actor.o: Actor.cpp.o
.PHONY : Actor.o

# target to build an object file
Actor.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/Actor.cpp.o
.PHONY : Actor.cpp.o

Actor.i: Actor.cpp.i
.PHONY : Actor.i

# target to preprocess a source file
Actor.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/Actor.cpp.i
.PHONY : Actor.cpp.i

Actor.s: Actor.cpp.s
.PHONY : Actor.s

# target to generate assembly for a file
Actor.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/Actor.cpp.s
.PHONY : Actor.cpp.s

Camera.o: Camera.cpp.o
.PHONY : Camera.o

# target to build an object file
Camera.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/Camera.cpp.o
.PHONY : Camera.cpp.o

Camera.i: Camera.cpp.i
.PHONY : Camera.i

# target to preprocess a source file
Camera.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/Camera.cpp.i
.PHONY : Camera.cpp.i

Camera.s: Camera.cpp.s
.PHONY : Camera.s

# target to generate assembly for a file
Camera.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/Camera.cpp.s
.PHONY : Camera.cpp.s

Math.o: Math.cpp.o
.PHONY : Math.o

# target to build an object file
Math.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/Math.cpp.o
.PHONY : Math.cpp.o

Math.i: Math.cpp.i
.PHONY : Math.i

# target to preprocess a source file
Math.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/Math.cpp.i
.PHONY : Math.cpp.i

Math.s: Math.cpp.s
.PHONY : Math.s

# target to generate assembly for a file
Math.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/Math.cpp.s
.PHONY : Math.cpp.s

MeshFunctions2.o: MeshFunctions2.cpp.o
.PHONY : MeshFunctions2.o

# target to build an object file
MeshFunctions2.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/MeshFunctions2.cpp.o
.PHONY : MeshFunctions2.cpp.o

MeshFunctions2.i: MeshFunctions2.cpp.i
.PHONY : MeshFunctions2.i

# target to preprocess a source file
MeshFunctions2.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/MeshFunctions2.cpp.i
.PHONY : MeshFunctions2.cpp.i

MeshFunctions2.s: MeshFunctions2.cpp.s
.PHONY : MeshFunctions2.s

# target to generate assembly for a file
MeshFunctions2.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/MeshFunctions2.cpp.s
.PHONY : MeshFunctions2.cpp.s

NewGui.o: NewGui.cpp.o
.PHONY : NewGui.o

# target to build an object file
NewGui.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/NewGui.cpp.o
.PHONY : NewGui.cpp.o

NewGui.i: NewGui.cpp.i
.PHONY : NewGui.i

# target to preprocess a source file
NewGui.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/NewGui.cpp.i
.PHONY : NewGui.cpp.i

NewGui.s: NewGui.cpp.s
.PHONY : NewGui.s

# target to generate assembly for a file
NewGui.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/NewGui.cpp.s
.PHONY : NewGui.cpp.s

RenderFunctions.o: RenderFunctions.cpp.o
.PHONY : RenderFunctions.o

# target to build an object file
RenderFunctions.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/RenderFunctions.cpp.o
.PHONY : RenderFunctions.cpp.o

RenderFunctions.i: RenderFunctions.cpp.i
.PHONY : RenderFunctions.i

# target to preprocess a source file
RenderFunctions.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/RenderFunctions.cpp.i
.PHONY : RenderFunctions.cpp.i

RenderFunctions.s: RenderFunctions.cpp.s
.PHONY : RenderFunctions.s

# target to generate assembly for a file
RenderFunctions.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/RenderFunctions.cpp.s
.PHONY : RenderFunctions.cpp.s

Renderer.o: Renderer.cpp.o
.PHONY : Renderer.o

# target to build an object file
Renderer.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/Renderer.cpp.o
.PHONY : Renderer.cpp.o

Renderer.i: Renderer.cpp.i
.PHONY : Renderer.i

# target to preprocess a source file
Renderer.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/Renderer.cpp.i
.PHONY : Renderer.cpp.i

Renderer.s: Renderer.cpp.s
.PHONY : Renderer.s

# target to generate assembly for a file
Renderer.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/Renderer.cpp.s
.PHONY : Renderer.cpp.s

Representation.o: Representation.cpp.o
.PHONY : Representation.o

# target to build an object file
Representation.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/Representation.cpp.o
.PHONY : Representation.cpp.o

Representation.i: Representation.cpp.i
.PHONY : Representation.i

# target to preprocess a source file
Representation.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/Representation.cpp.i
.PHONY : Representation.cpp.i

Representation.s: Representation.cpp.s
.PHONY : Representation.s

# target to generate assembly for a file
Representation.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/Representation.cpp.s
.PHONY : Representation.cpp.s

Systems.o: Systems.cpp.o
.PHONY : Systems.o

# target to build an object file
Systems.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/Systems.cpp.o
.PHONY : Systems.cpp.o

Systems.i: Systems.cpp.i
.PHONY : Systems.i

# target to preprocess a source file
Systems.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/Systems.cpp.i
.PHONY : Systems.cpp.i

Systems.s: Systems.cpp.s
.PHONY : Systems.s

# target to generate assembly for a file
Systems.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/Systems.cpp.s
.PHONY : Systems.cpp.s

TextureFunctions.o: TextureFunctions.cpp.o
.PHONY : TextureFunctions.o

# target to build an object file
TextureFunctions.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/TextureFunctions.cpp.o
.PHONY : TextureFunctions.cpp.o

TextureFunctions.i: TextureFunctions.cpp.i
.PHONY : TextureFunctions.i

# target to preprocess a source file
TextureFunctions.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/TextureFunctions.cpp.i
.PHONY : TextureFunctions.cpp.i

TextureFunctions.s: TextureFunctions.cpp.s
.PHONY : TextureFunctions.s

# target to generate assembly for a file
TextureFunctions.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/TextureFunctions.cpp.s
.PHONY : TextureFunctions.cpp.s

Utils.o: Utils.cpp.o
.PHONY : Utils.o

# target to build an object file
Utils.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/Utils.cpp.o
.PHONY : Utils.cpp.o

Utils.i: Utils.cpp.i
.PHONY : Utils.i

# target to preprocess a source file
Utils.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/Utils.cpp.i
.PHONY : Utils.cpp.i

Utils.s: Utils.cpp.s
.PHONY : Utils.s

# target to generate assembly for a file
Utils.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/Utils.cpp.s
.PHONY : Utils.cpp.s

Window.o: Window.cpp.o
.PHONY : Window.o

# target to build an object file
Window.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/Window.cpp.o
.PHONY : Window.cpp.o

Window.i: Window.cpp.i
.PHONY : Window.i

# target to preprocess a source file
Window.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/Window.cpp.i
.PHONY : Window.cpp.i

Window.s: Window.cpp.s
.PHONY : Window.s

# target to generate assembly for a file
Window.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/Window.cpp.s
.PHONY : Window.cpp.s

World.o: World.cpp.o
.PHONY : World.o

# target to build an object file
World.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/World.cpp.o
.PHONY : World.cpp.o

World.i: World.cpp.i
.PHONY : World.i

# target to preprocess a source file
World.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/World.cpp.i
.PHONY : World.cpp.i

World.s: World.cpp.s
.PHONY : World.s

# target to generate assembly for a file
World.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/World.cpp.s
.PHONY : World.cpp.s

btKinematicCharacterController.o: btKinematicCharacterController.cpp.o
.PHONY : btKinematicCharacterController.o

# target to build an object file
btKinematicCharacterController.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/btKinematicCharacterController.cpp.o
.PHONY : btKinematicCharacterController.cpp.o

btKinematicCharacterController.i: btKinematicCharacterController.cpp.i
.PHONY : btKinematicCharacterController.i

# target to preprocess a source file
btKinematicCharacterController.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/btKinematicCharacterController.cpp.i
.PHONY : btKinematicCharacterController.cpp.i

btKinematicCharacterController.s: btKinematicCharacterController.cpp.s
.PHONY : btKinematicCharacterController.s

# target to generate assembly for a file
btKinematicCharacterController.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/btKinematicCharacterController.cpp.s
.PHONY : btKinematicCharacterController.cpp.s

bullet.o: bullet.cpp.o
.PHONY : bullet.o

# target to build an object file
bullet.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/bullet.cpp.o
.PHONY : bullet.cpp.o

bullet.i: bullet.cpp.i
.PHONY : bullet.i

# target to preprocess a source file
bullet.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/bullet.cpp.i
.PHONY : bullet.cpp.i

bullet.s: bullet.cpp.s
.PHONY : bullet.s

# target to generate assembly for a file
bullet.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/bullet.cpp.s
.PHONY : bullet.cpp.s

glad.o: glad.c.o
.PHONY : glad.o

# target to build an object file
glad.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/glad.c.o
.PHONY : glad.c.o

glad.i: glad.c.i
.PHONY : glad.i

# target to preprocess a source file
glad.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/glad.c.i
.PHONY : glad.c.i

glad.s: glad.c.s
.PHONY : glad.s

# target to generate assembly for a file
glad.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/glad.c.s
.PHONY : glad.c.s

home/solidus/ECS_Libraries/imgui/backends/imgui_impl_glfw.o: home/solidus/ECS_Libraries/imgui/backends/imgui_impl_glfw.cpp.o
.PHONY : home/solidus/ECS_Libraries/imgui/backends/imgui_impl_glfw.o

# target to build an object file
home/solidus/ECS_Libraries/imgui/backends/imgui_impl_glfw.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/home/solidus/ECS_Libraries/imgui/backends/imgui_impl_glfw.cpp.o
.PHONY : home/solidus/ECS_Libraries/imgui/backends/imgui_impl_glfw.cpp.o

home/solidus/ECS_Libraries/imgui/backends/imgui_impl_glfw.i: home/solidus/ECS_Libraries/imgui/backends/imgui_impl_glfw.cpp.i
.PHONY : home/solidus/ECS_Libraries/imgui/backends/imgui_impl_glfw.i

# target to preprocess a source file
home/solidus/ECS_Libraries/imgui/backends/imgui_impl_glfw.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/home/solidus/ECS_Libraries/imgui/backends/imgui_impl_glfw.cpp.i
.PHONY : home/solidus/ECS_Libraries/imgui/backends/imgui_impl_glfw.cpp.i

home/solidus/ECS_Libraries/imgui/backends/imgui_impl_glfw.s: home/solidus/ECS_Libraries/imgui/backends/imgui_impl_glfw.cpp.s
.PHONY : home/solidus/ECS_Libraries/imgui/backends/imgui_impl_glfw.s

# target to generate assembly for a file
home/solidus/ECS_Libraries/imgui/backends/imgui_impl_glfw.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/home/solidus/ECS_Libraries/imgui/backends/imgui_impl_glfw.cpp.s
.PHONY : home/solidus/ECS_Libraries/imgui/backends/imgui_impl_glfw.cpp.s

home/solidus/ECS_Libraries/imgui/backends/imgui_impl_opengl3.o: home/solidus/ECS_Libraries/imgui/backends/imgui_impl_opengl3.cpp.o
.PHONY : home/solidus/ECS_Libraries/imgui/backends/imgui_impl_opengl3.o

# target to build an object file
home/solidus/ECS_Libraries/imgui/backends/imgui_impl_opengl3.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/home/solidus/ECS_Libraries/imgui/backends/imgui_impl_opengl3.cpp.o
.PHONY : home/solidus/ECS_Libraries/imgui/backends/imgui_impl_opengl3.cpp.o

home/solidus/ECS_Libraries/imgui/backends/imgui_impl_opengl3.i: home/solidus/ECS_Libraries/imgui/backends/imgui_impl_opengl3.cpp.i
.PHONY : home/solidus/ECS_Libraries/imgui/backends/imgui_impl_opengl3.i

# target to preprocess a source file
home/solidus/ECS_Libraries/imgui/backends/imgui_impl_opengl3.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/home/solidus/ECS_Libraries/imgui/backends/imgui_impl_opengl3.cpp.i
.PHONY : home/solidus/ECS_Libraries/imgui/backends/imgui_impl_opengl3.cpp.i

home/solidus/ECS_Libraries/imgui/backends/imgui_impl_opengl3.s: home/solidus/ECS_Libraries/imgui/backends/imgui_impl_opengl3.cpp.s
.PHONY : home/solidus/ECS_Libraries/imgui/backends/imgui_impl_opengl3.s

# target to generate assembly for a file
home/solidus/ECS_Libraries/imgui/backends/imgui_impl_opengl3.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/home/solidus/ECS_Libraries/imgui/backends/imgui_impl_opengl3.cpp.s
.PHONY : home/solidus/ECS_Libraries/imgui/backends/imgui_impl_opengl3.cpp.s

home/solidus/ECS_Libraries/imgui/imgui.o: home/solidus/ECS_Libraries/imgui/imgui.cpp.o
.PHONY : home/solidus/ECS_Libraries/imgui/imgui.o

# target to build an object file
home/solidus/ECS_Libraries/imgui/imgui.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/home/solidus/ECS_Libraries/imgui/imgui.cpp.o
.PHONY : home/solidus/ECS_Libraries/imgui/imgui.cpp.o

home/solidus/ECS_Libraries/imgui/imgui.i: home/solidus/ECS_Libraries/imgui/imgui.cpp.i
.PHONY : home/solidus/ECS_Libraries/imgui/imgui.i

# target to preprocess a source file
home/solidus/ECS_Libraries/imgui/imgui.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/home/solidus/ECS_Libraries/imgui/imgui.cpp.i
.PHONY : home/solidus/ECS_Libraries/imgui/imgui.cpp.i

home/solidus/ECS_Libraries/imgui/imgui.s: home/solidus/ECS_Libraries/imgui/imgui.cpp.s
.PHONY : home/solidus/ECS_Libraries/imgui/imgui.s

# target to generate assembly for a file
home/solidus/ECS_Libraries/imgui/imgui.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/home/solidus/ECS_Libraries/imgui/imgui.cpp.s
.PHONY : home/solidus/ECS_Libraries/imgui/imgui.cpp.s

home/solidus/ECS_Libraries/imgui/imgui_draw.o: home/solidus/ECS_Libraries/imgui/imgui_draw.cpp.o
.PHONY : home/solidus/ECS_Libraries/imgui/imgui_draw.o

# target to build an object file
home/solidus/ECS_Libraries/imgui/imgui_draw.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/home/solidus/ECS_Libraries/imgui/imgui_draw.cpp.o
.PHONY : home/solidus/ECS_Libraries/imgui/imgui_draw.cpp.o

home/solidus/ECS_Libraries/imgui/imgui_draw.i: home/solidus/ECS_Libraries/imgui/imgui_draw.cpp.i
.PHONY : home/solidus/ECS_Libraries/imgui/imgui_draw.i

# target to preprocess a source file
home/solidus/ECS_Libraries/imgui/imgui_draw.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/home/solidus/ECS_Libraries/imgui/imgui_draw.cpp.i
.PHONY : home/solidus/ECS_Libraries/imgui/imgui_draw.cpp.i

home/solidus/ECS_Libraries/imgui/imgui_draw.s: home/solidus/ECS_Libraries/imgui/imgui_draw.cpp.s
.PHONY : home/solidus/ECS_Libraries/imgui/imgui_draw.s

# target to generate assembly for a file
home/solidus/ECS_Libraries/imgui/imgui_draw.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/home/solidus/ECS_Libraries/imgui/imgui_draw.cpp.s
.PHONY : home/solidus/ECS_Libraries/imgui/imgui_draw.cpp.s

home/solidus/ECS_Libraries/imgui/imgui_tables.o: home/solidus/ECS_Libraries/imgui/imgui_tables.cpp.o
.PHONY : home/solidus/ECS_Libraries/imgui/imgui_tables.o

# target to build an object file
home/solidus/ECS_Libraries/imgui/imgui_tables.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/home/solidus/ECS_Libraries/imgui/imgui_tables.cpp.o
.PHONY : home/solidus/ECS_Libraries/imgui/imgui_tables.cpp.o

home/solidus/ECS_Libraries/imgui/imgui_tables.i: home/solidus/ECS_Libraries/imgui/imgui_tables.cpp.i
.PHONY : home/solidus/ECS_Libraries/imgui/imgui_tables.i

# target to preprocess a source file
home/solidus/ECS_Libraries/imgui/imgui_tables.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/home/solidus/ECS_Libraries/imgui/imgui_tables.cpp.i
.PHONY : home/solidus/ECS_Libraries/imgui/imgui_tables.cpp.i

home/solidus/ECS_Libraries/imgui/imgui_tables.s: home/solidus/ECS_Libraries/imgui/imgui_tables.cpp.s
.PHONY : home/solidus/ECS_Libraries/imgui/imgui_tables.s

# target to generate assembly for a file
home/solidus/ECS_Libraries/imgui/imgui_tables.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/home/solidus/ECS_Libraries/imgui/imgui_tables.cpp.s
.PHONY : home/solidus/ECS_Libraries/imgui/imgui_tables.cpp.s

home/solidus/ECS_Libraries/imgui/imgui_widgets.o: home/solidus/ECS_Libraries/imgui/imgui_widgets.cpp.o
.PHONY : home/solidus/ECS_Libraries/imgui/imgui_widgets.o

# target to build an object file
home/solidus/ECS_Libraries/imgui/imgui_widgets.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/home/solidus/ECS_Libraries/imgui/imgui_widgets.cpp.o
.PHONY : home/solidus/ECS_Libraries/imgui/imgui_widgets.cpp.o

home/solidus/ECS_Libraries/imgui/imgui_widgets.i: home/solidus/ECS_Libraries/imgui/imgui_widgets.cpp.i
.PHONY : home/solidus/ECS_Libraries/imgui/imgui_widgets.i

# target to preprocess a source file
home/solidus/ECS_Libraries/imgui/imgui_widgets.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/home/solidus/ECS_Libraries/imgui/imgui_widgets.cpp.i
.PHONY : home/solidus/ECS_Libraries/imgui/imgui_widgets.cpp.i

home/solidus/ECS_Libraries/imgui/imgui_widgets.s: home/solidus/ECS_Libraries/imgui/imgui_widgets.cpp.s
.PHONY : home/solidus/ECS_Libraries/imgui/imgui_widgets.s

# target to generate assembly for a file
home/solidus/ECS_Libraries/imgui/imgui_widgets.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/home/solidus/ECS_Libraries/imgui/imgui_widgets.cpp.s
.PHONY : home/solidus/ECS_Libraries/imgui/imgui_widgets.cpp.s

home/solidus/Programming/ECS/ECS/ObjParsing/NewMeshLoader_2.o: home/solidus/Programming/ECS/ECS/ObjParsing/NewMeshLoader_2.cpp.o
.PHONY : home/solidus/Programming/ECS/ECS/ObjParsing/NewMeshLoader_2.o

# target to build an object file
home/solidus/Programming/ECS/ECS/ObjParsing/NewMeshLoader_2.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/home/solidus/Programming/ECS/ECS/ObjParsing/NewMeshLoader_2.cpp.o
.PHONY : home/solidus/Programming/ECS/ECS/ObjParsing/NewMeshLoader_2.cpp.o

home/solidus/Programming/ECS/ECS/ObjParsing/NewMeshLoader_2.i: home/solidus/Programming/ECS/ECS/ObjParsing/NewMeshLoader_2.cpp.i
.PHONY : home/solidus/Programming/ECS/ECS/ObjParsing/NewMeshLoader_2.i

# target to preprocess a source file
home/solidus/Programming/ECS/ECS/ObjParsing/NewMeshLoader_2.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/home/solidus/Programming/ECS/ECS/ObjParsing/NewMeshLoader_2.cpp.i
.PHONY : home/solidus/Programming/ECS/ECS/ObjParsing/NewMeshLoader_2.cpp.i

home/solidus/Programming/ECS/ECS/ObjParsing/NewMeshLoader_2.s: home/solidus/Programming/ECS/ECS/ObjParsing/NewMeshLoader_2.cpp.s
.PHONY : home/solidus/Programming/ECS/ECS/ObjParsing/NewMeshLoader_2.s

# target to generate assembly for a file
home/solidus/Programming/ECS/ECS/ObjParsing/NewMeshLoader_2.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/home/solidus/Programming/ECS/ECS/ObjParsing/NewMeshLoader_2.cpp.s
.PHONY : home/solidus/Programming/ECS/ECS/ObjParsing/NewMeshLoader_2.cpp.s

home/solidus/Programming/ECS/ECS/ObjParsing/processing.o: home/solidus/Programming/ECS/ECS/ObjParsing/processing.cpp.o
.PHONY : home/solidus/Programming/ECS/ECS/ObjParsing/processing.o

# target to build an object file
home/solidus/Programming/ECS/ECS/ObjParsing/processing.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/home/solidus/Programming/ECS/ECS/ObjParsing/processing.cpp.o
.PHONY : home/solidus/Programming/ECS/ECS/ObjParsing/processing.cpp.o

home/solidus/Programming/ECS/ECS/ObjParsing/processing.i: home/solidus/Programming/ECS/ECS/ObjParsing/processing.cpp.i
.PHONY : home/solidus/Programming/ECS/ECS/ObjParsing/processing.i

# target to preprocess a source file
home/solidus/Programming/ECS/ECS/ObjParsing/processing.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/home/solidus/Programming/ECS/ECS/ObjParsing/processing.cpp.i
.PHONY : home/solidus/Programming/ECS/ECS/ObjParsing/processing.cpp.i

home/solidus/Programming/ECS/ECS/ObjParsing/processing.s: home/solidus/Programming/ECS/ECS/ObjParsing/processing.cpp.s
.PHONY : home/solidus/Programming/ECS/ECS/ObjParsing/processing.s

# target to generate assembly for a file
home/solidus/Programming/ECS/ECS/ObjParsing/processing.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/home/solidus/Programming/ECS/ECS/ObjParsing/processing.cpp.s
.PHONY : home/solidus/Programming/ECS/ECS/ObjParsing/processing.cpp.s

main.o: main.cpp.o
.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i
.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s
.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/ECS.dir/build.make CMakeFiles/ECS.dir/main.cpp.s
.PHONY : main.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... ECS"
	@echo "... Actor.o"
	@echo "... Actor.i"
	@echo "... Actor.s"
	@echo "... Camera.o"
	@echo "... Camera.i"
	@echo "... Camera.s"
	@echo "... Math.o"
	@echo "... Math.i"
	@echo "... Math.s"
	@echo "... MeshFunctions2.o"
	@echo "... MeshFunctions2.i"
	@echo "... MeshFunctions2.s"
	@echo "... NewGui.o"
	@echo "... NewGui.i"
	@echo "... NewGui.s"
	@echo "... RenderFunctions.o"
	@echo "... RenderFunctions.i"
	@echo "... RenderFunctions.s"
	@echo "... Renderer.o"
	@echo "... Renderer.i"
	@echo "... Renderer.s"
	@echo "... Representation.o"
	@echo "... Representation.i"
	@echo "... Representation.s"
	@echo "... Systems.o"
	@echo "... Systems.i"
	@echo "... Systems.s"
	@echo "... TextureFunctions.o"
	@echo "... TextureFunctions.i"
	@echo "... TextureFunctions.s"
	@echo "... Utils.o"
	@echo "... Utils.i"
	@echo "... Utils.s"
	@echo "... Window.o"
	@echo "... Window.i"
	@echo "... Window.s"
	@echo "... World.o"
	@echo "... World.i"
	@echo "... World.s"
	@echo "... btKinematicCharacterController.o"
	@echo "... btKinematicCharacterController.i"
	@echo "... btKinematicCharacterController.s"
	@echo "... bullet.o"
	@echo "... bullet.i"
	@echo "... bullet.s"
	@echo "... glad.o"
	@echo "... glad.i"
	@echo "... glad.s"
	@echo "... home/solidus/ECS_Libraries/imgui/backends/imgui_impl_glfw.o"
	@echo "... home/solidus/ECS_Libraries/imgui/backends/imgui_impl_glfw.i"
	@echo "... home/solidus/ECS_Libraries/imgui/backends/imgui_impl_glfw.s"
	@echo "... home/solidus/ECS_Libraries/imgui/backends/imgui_impl_opengl3.o"
	@echo "... home/solidus/ECS_Libraries/imgui/backends/imgui_impl_opengl3.i"
	@echo "... home/solidus/ECS_Libraries/imgui/backends/imgui_impl_opengl3.s"
	@echo "... home/solidus/ECS_Libraries/imgui/imgui.o"
	@echo "... home/solidus/ECS_Libraries/imgui/imgui.i"
	@echo "... home/solidus/ECS_Libraries/imgui/imgui.s"
	@echo "... home/solidus/ECS_Libraries/imgui/imgui_draw.o"
	@echo "... home/solidus/ECS_Libraries/imgui/imgui_draw.i"
	@echo "... home/solidus/ECS_Libraries/imgui/imgui_draw.s"
	@echo "... home/solidus/ECS_Libraries/imgui/imgui_tables.o"
	@echo "... home/solidus/ECS_Libraries/imgui/imgui_tables.i"
	@echo "... home/solidus/ECS_Libraries/imgui/imgui_tables.s"
	@echo "... home/solidus/ECS_Libraries/imgui/imgui_widgets.o"
	@echo "... home/solidus/ECS_Libraries/imgui/imgui_widgets.i"
	@echo "... home/solidus/ECS_Libraries/imgui/imgui_widgets.s"
	@echo "... home/solidus/Programming/ECS/ECS/ObjParsing/NewMeshLoader_2.o"
	@echo "... home/solidus/Programming/ECS/ECS/ObjParsing/NewMeshLoader_2.i"
	@echo "... home/solidus/Programming/ECS/ECS/ObjParsing/NewMeshLoader_2.s"
	@echo "... home/solidus/Programming/ECS/ECS/ObjParsing/processing.o"
	@echo "... home/solidus/Programming/ECS/ECS/ObjParsing/processing.i"
	@echo "... home/solidus/Programming/ECS/ECS/ObjParsing/processing.s"
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
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

