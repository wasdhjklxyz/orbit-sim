cmake_minimum_required(VERSION 4.3.4) # FIXME: Only tested w/ 4.3.4
set(PROJECT vis-viva)
project(${PROJECT} CXX)

include(cmake/EmbedShaders.cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT}
  src/main.cpp src/app.cpp
  src/entity.cpp src/sim.cpp
  src/renderer.cpp src/shaders.cpp src/vbo.cpp src/vao.cpp)
target_compile_features(${PROJECT} PRIVATE cxx_std_23)
target_compile_options(${PROJECT} PRIVATE
  -Wall -Wextra -Wpedantic -Werror -Wno-experimental)

target_link_options(${PROJECT} PRIVATE
  $<$<CONFIG:Debug>:-g;-sASSERTIONS=2;-sGL_ASSERTIONS=1>
  $<$<CONFIG:Release>:-O3>)

find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(${PROJECT} PRIVATE SDL3::SDL3-static)

if(EMSCRIPTEN)
  target_link_options(${PROJECT} PRIVATE
    --emrun -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2)
  set_target_properties(${PROJECT} PROPERTIES SUFFIX ".html")
else()
  message("Emscripten is required" FATAL) # FIXME
  # find_package(SDL3 REQUIRED)
  # target_link_libraries(${PROJECT} PRIVATE SDL3::SDL3)
endif()

embed_shaders(${PROJECT} SOURCES shaders/vert.glsl shaders/frag.glsl)

set(WIDTH 1280)
set(HEIGHT 720)
set(METERS_PER_NDC 200.0)
set(NUM_VERTICES_CIRCLE 32)
configure_file(src/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)
target_include_directories(${PROJECT} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")

include(FetchContent)
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 8d1fd52e5ab5590e2c81768ace50c72bae28f2ed # 1.0.3
)
FetchContent_MakeAvailable(glm)
target_link_libraries(${PROJECT} PRIVATE glm::glm)
