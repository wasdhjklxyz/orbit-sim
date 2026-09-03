include(EmbedShaders)

add_library(gsb_gfx STATIC)
add_library(gsb::gfx ALIAS gsb_gfx)

target_sources(gsb_gfx
  PRIVATE
    circle.cpp
    geom.hpp
    gfx_internal.hpp
    gpu.cpp
    gpu.hpp
    renderer.cpp
    shaders.cpp
    sphere.cpp
  PUBLIC
    FILE_SET HEADERS
      BASE_DIRS "${PROJECT_SOURCE_DIR}/include"
      FILES "${PROJECT_SOURCE_DIR}/include/gsb/gfx.hpp")

target_link_libraries(gsb_gfx
  PUBLIC
    glm::glm
  PRIVATE
    gsb::cfg
    gsb::common)

# NOTE: Emscripten provides GLES3 implicitly (mapped onto WebGL2)
if(NOT EMSCRIPTEN)
  find_package(OpenGL REQUIRED COMPONENTS GLES3)
  target_link_libraries(gsb_gfx PRIVATE OpenGL::GLES3)
endif()

embed_shaders(gsb_gfx
  SOURCES
    "${PROJECT_SOURCE_DIR}/shaders/vert.glsl"
    "${PROJECT_SOURCE_DIR}/shaders/frag.glsl")
