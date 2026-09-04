set(WIDTH 1280)
set(HEIGHT 720)
set(NUM_SECTORS_CIRCLE 32)
set(NUM_SECTORS_SPHERE 36)
set(NUM_STACKS_SPHERE 18)
set(MAX_ENTITIES 16)
set(G 200) # 6.6743e-11
set(PERSPECTIVE_FOV 60.0)
set(PERSPECTIVE_NEAR 1.0)
set(PERSPECTIVE_FAR 1000.0)

configure_file(config.h.in
  "${CMAKE_CURRENT_BINARY_DIR}/include/gsb/config.h" @ONLY)

add_library(gsb_cfg INTERFACE)
add_library(gsb::cfg ALIAS gsb_cfg)

target_sources(gsb_cfg 
  INTERFACE
  FILE_SET HEADERS
    BASE_DIRS "${CMAKE_CURRENT_BINARY_DIR}/include"
    FILES "${CMAKE_CURRENT_BINARY_DIR}/include/gsb/config.h")
