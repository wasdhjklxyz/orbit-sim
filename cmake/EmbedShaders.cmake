set(_EMBED_SHADERS_SCRIPT
  "${CMAKE_CURRENT_LIST_DIR}/embed_shaders.cmake" CACHE INTERNAL "")

function(embed_shaders LINK_TARGET)
  cmake_parse_arguments(ARG "" "" "SOURCES" ${ARGN})

  set(_tgt "${LINK_TARGET}_embed_shaders")
  string(MAKE_C_IDENTIFIER "${_tgt}" _tgt)

  if(NOT EXISTS "${_EMBED_SHADERS_SCRIPT}")
    message(FATAL_ERROR "[embed_shaders] Script not found: ${_EMBED_SHADERS_SCRIPT}")
  endif()

  set(_hdrs "")
  foreach(_src IN LISTS ARG_SOURCES)
    if(NOT IS_ABSOLUTE "${_src}")
      set(_src "${CMAKE_CURRENT_SOURCE_DIR}/${_src}")
    endif()
    if(NOT EXISTS "${_src}")
      message(FATAL_ERROR "[embed_shaders] File not found: ${_src}")
    endif()

    get_filename_component(_name "${_src}" NAME)
    string(MAKE_C_IDENTIFIER "${_name}" _sym)
    set(_hdr "${CMAKE_CURRENT_BINARY_DIR}/generated/shaders/${_name}.hpp")

    add_custom_command(
      OUTPUT "${_hdr}"
      COMMAND ${CMAKE_COMMAND}
        -DGLSL_FILE=${_src}
        -DGLSL_HDR=${_hdr}
        -DGLSL_SYMBOL=${_sym}
        -P "${_EMBED_SHADERS_SCRIPT}"
      DEPENDS "${_src}" "${_EMBED_SHADERS_SCRIPT}"
      COMMENT "[embed_shaders] ${_src} -> ${_hdr}"
      VERBATIM)

    list(APPEND _hdrs "${_hdr}")
  endforeach()

  add_custom_target(${_tgt} DEPENDS ${_hdrs})
  target_sources(${LINK_TARGET} PRIVATE ${_hdrs})
  target_include_directories(${LINK_TARGET} PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}/generated")
endfunction()
