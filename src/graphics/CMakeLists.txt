set(GRAPHICS_SRCS
  renderer.cc
  mesh.cc
  vertexbuffer.cc
  vertexarray.cc
  indexbuffer.cc
  shader.cc
  texture.cc
)

add_library(graphics ${GRAPHICS_SRCS})

target_include_directories(graphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
