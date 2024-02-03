cmake_minimum_required(VERSION 3.14)

project(
  angie
  VERSION 0.1.0
  DESCRIPTION "OpenGL play project"
  LANGUAGES CXX C
)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(SRC_DIR src)
set(LIB_DIR libs)
set(EXTERN_DIR extern)

set(SRCS
  ${SRC_DIR}/main.cc
  ${SRC_DIR}/application.cc
  ${SRC_DIR}/window.cc
  ${SRC_DIR}/cameracontroller.cc
  ${SRC_DIR}/camera.cc
  ${SRC_DIR}/graphics/renderer.cc
  ${SRC_DIR}/graphics/mesh.cc
  ${SRC_DIR}/graphics/vertexbuffer.cc
  ${SRC_DIR}/graphics/vertexarray.cc
  ${SRC_DIR}/graphics/indexbuffer.cc
  ${SRC_DIR}/graphics/shader.cc
  ${SRC_DIR}/graphics/texture.cc
)

add_executable(${PROJECT_NAME} ${SRCS})

target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

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

set(GLBINDING_DIR ${EXTERN_DIR}/glbinding)
add_subdirectory(${GLBINDING_DIR})

set(GLM_DIR ${EXTERN_DIR}/glm)
add_subdirectory(${GLM_DIR})
target_include_directories(glm PRIVATE ${GLM_INCLUDE_DIRS})

set(STB_DIR ${LIB_DIR}/stb)
target_include_directories(${PROJECT_NAME} PRIVATE ${STB_DIR}/include)

set(SPDLOG_DIR ${EXTERN_DIR}/spdlog)
add_subdirectory(${SPDLOG_DIR})

target_link_libraries(
  ${PROJECT_NAME}
  glfw ${GLFW_LIBRARIES}
  glbinding::glbinding
  glm
  spdlog
)
