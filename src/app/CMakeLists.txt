find_package(SDL3 CONFIG REQUIRED)

add_library(gsb_app STATIC)
add_library(gsb::app ALIAS gsb_app)

target_sources(gsb_app
  PRIVATE
    app.cpp
  PUBLIC
    FILE_SET HEADERS
      BASE_DIRS "${PROJECT_SOURCE_DIR}/include"
      FILES "${PROJECT_SOURCE_DIR}/include/gsb/app.hpp")

target_link_libraries(gsb_app
  PUBLIC
    gsb::gfx
    gsb::sim
    SDL3::SDL3 # FIXME: app.hpp leaks SDL types
  PRIVATE
    gsb::cfg
    gsb::common)
