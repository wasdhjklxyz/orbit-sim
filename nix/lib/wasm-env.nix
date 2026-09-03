{ pkgs, sdl3 }:
{
  EM_TOOLCHAIN = "${pkgs.emscripten}/share/emscripten/cmake/Modules/Platform/Emscripten.cmake";
  SDL3_WASM_ROOT = "${sdl3}";
  glm_SRC = "${pkgs.glm.src}";
}
