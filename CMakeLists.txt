cmake_minimum_required(VERSION 4.3.4) # FIXME: Only tested w/ 4.3.4
project(orbit-sim CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(hello hello_world.cpp)
target_compile_features(hello PRIVATE cxx_std_23)

if(EMSCRIPTEN)
  target_compile_options(hello PRIVATE --use-port=sdl3)
  target_link_options(hello PRIVATE --use-port=sdl3)
  set_target_properties(hello PROPERTIES SUFFIX ".html")
else()
  find_package(SDL3 REQUIRED)
endif()

target_include_directories(hello PRIVATE ${SDL3_INCLUDE_DIRS})
target_link_libraries(hello PRIVATE ${SDL3_LIBRARIES})
