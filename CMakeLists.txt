cmake_minimum_required(VERSION 4.3)

project(gsb
  VERSION 0.0.0 # NOTE: Intentional, no releases yet
  SPDX_LICENSE "MIT"
  DESCRIPTION "Interactive n-body gravity simulation for the browser."
  HOMEPAGE_URL "https://github.com/wasdhjklxyz/gravity-sandbox"
  LANGUAGES CXX)

option(GSB_BUILD_APP "Build the app" ${PROJECT_IS_TOP_LEVEL})
option(GSB_ENABLE_TESTING "Enable testing" ${PROJECT_IS_TOP_LEVEL})

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(FetchContent)
include(GNUInstallDirs)

if(PROJECT_IS_TOP_LEVEL)
  set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
  set(CMAKE_CXX_STANDARD 23)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
endif()

add_library(gsb_common INTERFACE)
add_library(gsb::common ALIAS gsb_common)
target_compile_options(gsb_common
  INTERFACE -Wall -Wextra -Wpedantic -Werror
    $<$<BOOL:${EMSCRIPTEN}>:-Wno-experimental>)

FetchContent_Declare(glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 8d1fd52e5ab5590e2c81768ace50c72bae28f2ed # 1.0.3
  FIND_PACKAGE_ARGS)
FetchContent_MakeAvailable(glm)

if(GSB_BUILD_APP)
  add_subdirectory(src)
endif()

if(GSB_ENABLE_TESTING AND NOT EMSCRIPTEN)
  enable_testing()
  add_subdirectory(tests)
endif()
