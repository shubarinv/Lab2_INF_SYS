 cmake_minimum_required(VERSION 3.19)
 project(Lab2_InfSys)

 set(CMAKE_CXX_STANDARD 20)
 add_subdirectory(SDL2)
 add_executable(Lab2_InfSys main.cpp a_star.hpp node.hpp matrix.hpp)
 target_link_libraries(${PROJECT_NAME} SDL2)