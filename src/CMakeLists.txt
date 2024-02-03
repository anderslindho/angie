set(APP_SRCS
  application.cc
)

add_library(application ${APP_SRCS})

target_include_directories(application PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
