add_subdirectory(cfg)
add_subdirectory(sim)
add_subdirectory(gfx)
add_subdirectory(app)

add_executable(gsb main.cpp)

target_link_libraries(gsb
  PRIVATE
    gsb::app
    gsb::cfg
    gsb::common)

if(EMSCRIPTEN)
  target_link_options(gsb
    PRIVATE
      --emrun
      -sMIN_WEBGL_VERSION=2
      -sMAX_WEBGL_VERSION=2
      $<$<CONFIG:Debug>:-sASSERTIONS=2;-sGL_ASSERTIONS=1>
      $<$<CONFIG:Release>:-O3>)

  set_target_properties(gsb PROPERTIES SUFFIX ".html")

  install(
    TARGETS gsb
    RUNTIME DESTINATION "${CMAKE_INSTALL_DATADIR}/gsb")
  install(
    FILES
      "$<TARGET_FILE_DIR:gsb>/$<TARGET_FILE_BASE_NAME:gsb>.js"
      "$<TARGET_FILE_DIR:gsb>/$<TARGET_FILE_BASE_NAME:gsb>.wasm"
    DESTINATION
      "${CMAKE_INSTALL_DATADIR}/gsb")
else()
  # NOTE: Keep link-time lib dirs in installed rpath
  #       CMake otherwise strips them and the binary cant find SDL3/GLES
  set_target_properties(gsb PROPERTIES INSTALL_RPATH_USE_LINK_PATH ON)

  install(TARGETS gsb RUNTIME)
endif()
