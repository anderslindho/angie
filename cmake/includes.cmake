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
