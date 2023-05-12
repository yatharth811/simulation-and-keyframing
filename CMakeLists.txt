cmake_minimum_required(VERSION 3.0)

project(a1)
set(CMAKE_CXX_STANDARD 11)

find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

add_library(col781 src/hw.cpp src/camera.cpp src/cloth.cpp src/obstacles.cpp)
target_link_libraries(col781 GLEW::GLEW glm::glm OpenGL::GL SDL2::SDL2)

add_executable(example src/example.cpp)
target_link_libraries(example col781)

add_executable(simulation src/simulation.cpp)
target_link_libraries(simulation col781)

add_executable(collision examples/collision.cpp)
target_link_libraries(collision col781)

add_executable(cloth examples/cloth.cpp)
target_link_libraries(cloth col781)

add_executable(cloth_pbd examples/cloth_pbd.cpp)
target_link_libraries(cloth_pbd col781)

add_executable(cylinder examples/cylinder.cpp)
target_link_libraries(cylinder col781)

add_executable(skeleton examples/skeleton.cpp)
target_link_libraries(skeleton col781)

