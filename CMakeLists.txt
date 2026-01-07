cmake_minimum_required(VERSION 3.10)
project(Software_Renderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE src)

add_subdirectory(external/minifb)
target_link_libraries(Software_Renderer minifb)

target_include_directories(Software_Renderer PRIVATE external/minifb/include)	