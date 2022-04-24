cmake_minimum_required(VERSION 3.14)

project(
  angie
  VERSION 0.1.0
  DESCRIPTION "OpenGL play project"
  LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(SRC_DIR src)
set(LIB_DIR libs)
set(EXTERN_DIR extern)

set(SRCS
  ${SRC_DIR}/main.cc
  ${SRC_DIR}/application.cc
  ${SRC_DIR}/renderer.cc
  ${SRC_DIR}/vertexbuffer.cc
  ${SRC_DIR}/indexbuffer.cc
  ${SRC_DIR}/shader.cc
)

add_executable(${PROJECT_NAME} ${SRCS})

target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic
                                                 -Werror)
endif()

include(cmake/includes.cmake)

target_link_libraries(
  ${PROJECT_NAME}
  glfw ${GLFW_LIBRARIES}
  glbinding::glbinding
  glm
  spdlog
)
