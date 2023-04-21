cmake_minimum_required(VERSION 3.24)

project(ECS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-O0")
#set(CMAKE_CXX_FLAGS "-O2")

set(IMGUI_SOURCES
    /home/solidus/ECS_Libraries/imgui/imgui.cpp
    /home/solidus/ECS_Libraries/imgui/imgui_draw.cpp
    /home/solidus/ECS_Libraries/imgui/imgui_widgets.cpp
    /home/solidus/ECS_Libraries/imgui/imgui_tables.cpp
)

# gui_imgui.cpp
set(SOURCES 
    main.cpp
    bullet.cpp
    Actor.cpp
    Camera.cpp
    glad.c
    Math.cpp
    MeshFunctions2.cpp
    Renderer.cpp
    RenderFunctions.cpp
    Systems.cpp
    TextureFunctions.cpp
    Utils.cpp
    Window.cpp
    World.cpp
    NewGui.cpp
    Representation.cpp
    btKinematicCharacterController.cpp
    /home/solidus/ECS_Libraries/imgui/imgui.cpp
    /home/solidus/ECS_Libraries/imgui/imgui_draw.cpp
    /home/solidus/ECS_Libraries/imgui/imgui_widgets.cpp
    /home/solidus/ECS_Libraries/imgui/imgui_tables.cpp
    /home/solidus/ECS_Libraries/imgui/backends/imgui_impl_opengl3.cpp
    /home/solidus/ECS_Libraries/imgui/backends/imgui_impl_glfw.cpp
    /home/solidus/Programming/ECS/ECS/ObjParsing/NewMeshLoader_2.cpp
    /home/solidus/Programming/ECS/ECS/ObjParsing/processing.cpp
)

    #./ObjParsing/NewMeshLoader_2.cpp
    #./ObjParsing/processing.cpp
# add_executable(ECS ${IMGUI_SOURCES})
# add_executable(ECS ${SOURCES} ${IMGUI_SOURCES})
add_executable(ECS ${SOURCES})

# need to build it
target_include_directories(ECS PUBLIC ./ObjParsing/)
target_include_directories(ECS PUBLIC /home/solidus/ECS_Libraries/bullet3/src)
target_include_directories(ECS PUBLIC /home/solidus/ECS_Libraries/glad_opengl_4_6_core/include/)
target_include_directories(ECS PUBLIC /home/solidus/ECS_Libraries/glfw/include/)
target_include_directories(ECS PUBLIC /home/solidus/ECS_Libraries/assimp/include/)
target_include_directories(ECS PUBLIC /home/solidus/ECS_Libraries/imgui)
target_include_directories(ECS PUBLIC /home/solidus/ECS_Libraries/imgui/backends)
target_include_directories(ECS PUBLIC /home/solidus/ECS_Libraries/entt/src/)
target_include_directories(ECS PUBLIC /home/solidus/ECS_Libraries/glm/)

#target_link_libraries(ECS /home/solidus/ECS_Libraries/bullet3/src/Bullet3Collision/libBullet3Collision.a)
#target_link_libraries(ECS /home/solidus/ECS_Libraries/bullet3/src/Bullet3Common/libBullet3Common.a)
#target_link_libraries(ECS /home/solidus/ECS_Libraries/bullet3/src/Bullet3Dynamics/libBullet3Dynamics.a)
#target_link_libraries(ECS /home/solidus/ECS_Libraries/bullet3/src/Bullet3Geometry/libBullet3Geometry.a)
#target_link_libraries(ECS /home/solidus/ECS_Libraries/bullet3/src/Bullet3OpenCL/libBullet3OpenCL_clew.a)
#target_link_libraries(ECS /home/solidus/ECS_Libraries/bullet3/src/BulletInverseDynamics/libBulletInverseDynamics.a)

# target_link_libraries(ECS /home/solidus/Programming/Ubuntu/MeshLoader/OBJ_READER.o)
target_link_libraries(ECS /home/solidus/ECS_Libraries/glfw/src/libglfw3.a)

# here order matters
target_link_libraries(ECS /home/solidus/ECS_Libraries/bullet3/src/BulletSoftBody/libBulletSoftBody.a)
target_link_libraries(ECS /home/solidus/ECS_Libraries/bullet3/src/BulletDynamics/libBulletDynamics.a)
target_link_libraries(ECS /home/solidus/ECS_Libraries/bullet3/src/BulletCollision/libBulletCollision.a)
target_link_libraries(ECS /home/solidus/ECS_Libraries/bullet3/src/LinearMath/libLinearMath.a)
target_link_libraries(ECS /home/solidus/ECS_Libraries/assimp/bin/libassimp.so)

#target_compile_options(ECS PRIVATE -Wall -Wextra -Wpedantic -Werror)
#target_compile_options(ECS PRIVATE -g -fno-stack-protector -z execstack -no-pie)
target_compile_options(ECS PRIVATE -g)
#target_compile_options(ECS PRIVATE -Wall)
