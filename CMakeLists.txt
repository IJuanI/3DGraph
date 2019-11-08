# Project definition
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(3DGraph CXX C)

# Search dirs and files
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INC_DIR "${CMAKE_SOURCE_DIR}/../include")
set(LIB_DIR "${CMAKE_SOURCE_DIR}/../lib")
file(GLOB_RECURSE SOURCE_FILES ${SRC_DIR}/*.cpp ${SRC_DIR}/*.cc ${SRC_DIR}/*.c)
file(GLOB_RECURSE HEADER_FILES
	${INC_DIR}/*.h
	${INC_DIR}/*.hpp)

# Configure assets header file
configure_file(src/utils/resources.h.in src/utils/resources.h)
# include_directories(${CMAKE_BINARY_DIR}/src)

# Executable definition and properties
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${INC_DIR})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

# OpenGL
find_package(OpenGL REQUIRED)

# GLFW
set(GLFW_DIR ${LIB_DIR}/glfw)
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory(${GLFW_DIR} ${GLFW_DIR})
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")
target_link_libraries(${PROJECT_NAME} glfw)

# glad
add_library(glad ${LIB_DIR}/glad.c)
target_include_directories(glad PRIVATE ${INC_DIR})
target_link_libraries(${PROJECT_NAME} glad)

# GLM
# Nothing to do here

# ASSIMP
set(ZLIB_LIBRARIES zlibstatic)
set(ENABLE_BOOST_WORKAROUND ON)
set(BUILD_STATIC_LIB ON)
set(BUILD_ASSIMP_TOOLS ON)
set(ASSIMP_BUILD_STATIC_LIB ON)

add_subdirectory(${LIB_DIR}/assimp ${LIB_DIR}/assimp)
target_link_libraries(${PROJECT_NAME} assimp)

# STB_IMAGE
# Nothing to do here