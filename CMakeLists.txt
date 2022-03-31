cmake_minimum_required(VERSION 3.15)

project(
  angie
  VERSION 0.1.0
  DESCRIPTION "OpenGL play project"
  LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(SRC_DIR src)
set(LIB_DIR libs)
set(EXTERN_DIR extern)

file(GLOB SRCS ${SRC_DIR}/*.cc)

add_executable(${PROJECT_NAME} ${SRC_DIR}/main.cc)

target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic
                                                 -Werror)
endif()

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

set(GLFW_DIR ${EXTERN_DIR}/glfw)
set(GLFW_BUILD_EXAMPLES
    OFF
    CACHE BOOL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS
    OFF
    CACHE BOOL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS
    OFF
    CACHE BOOL "Build the GLFW documentation")
set(GLFW_INSTALL
    OFF
    CACHE BOOL "Generate installation target")
add_subdirectory(${GLFW_DIR})
target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})

set(GLAD_DIR ${LIB_DIR}/glad)
add_library(glad ${GLAD_DIR}/src/glad.c)
target_include_directories(glad PRIVATE ${GLAD_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${GLAD_DIR}/include)
target_link_libraries(${PROJECT_NAME} glad ${CMAKE_DL_LIBS})

set(GLM_DIR ${EXTERN_DIR}/glm)
add_subdirectory(${GLM_DIR})
target_include_directories(glm PRIVATE ${GLM_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} glm)

set(SPDLOG_DIR ${EXTERN_DIR}/spdlog)
add_subdirectory(${SPDLOG_DIR})
target_link_libraries(${PROJECT_NAME} spdlog)
