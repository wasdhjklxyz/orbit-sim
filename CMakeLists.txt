cmake_minimum_required(VERSION 4.3.4) # FIXME: Only tested w/ 4.3.4
set(PROJECT orbit-sim)
project(${PROJECT} CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT} src/main.cpp src/app.cpp)
target_compile_features(${PROJECT} PRIVATE cxx_std_23)
target_compile_options(${PROJECT} PRIVATE
  -Wall -Wextra -Wpedantic -Werror -Wno-experimental)

if(EMSCRIPTEN)
  target_compile_options(${PROJECT} PRIVATE --use-port=sdl3)
  target_link_options(${PROJECT} PRIVATE
    --use-port=sdl3 -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2)
  set_target_properties(${PROJECT} PROPERTIES SUFFIX ".html")
else()
  message("Emscripten is required" FATAL) # FIXME
  # find_package(SDL3 REQUIRED)
  # target_link_libraries(${PROJECT} PRIVATE SDL3::SDL3)
endif()
