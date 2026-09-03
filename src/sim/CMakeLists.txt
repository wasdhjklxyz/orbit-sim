add_library(gsb_sim STATIC)
add_library(gsb::sim ALIAS gsb_sim)

target_sources(gsb_sim
  PRIVATE
    sim.cpp
  PUBLIC
    FILE_SET HEADERS
      BASE_DIRS "${PROJECT_SOURCE_DIR}/include"
      FILES "${PROJECT_SOURCE_DIR}/include/gsb/sim.hpp")

target_link_libraries(gsb_sim
  PUBLIC
    glm::glm
  PRIVATE
    gsb::cfg
    gsb::common)
