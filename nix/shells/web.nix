{ pkgs, sdl3 }:
let
  clang = import ../lib/clangd.nix { inherit pkgs; };
  common = import ./common.nix {
    inherit pkgs;
    inherit (clang) clangTool;
  };
in
pkgs.mkShell {
  packages = common.packages ++ [
    pkgs.emscripten
    (clang.clangdQueryDriver "${pkgs.emscripten}/bin/em++")
    (pkgs.writeShellScriptBin "cbr" ''
      set -e
      cmake --preset "''${1:-debug}"
      cmake --build --preset "''${1:-debug}" -j$(nproc)
      ${pkgs.emscripten}/bin/emrun \
        --verbose build/''${1:-debug}/bin/*.html
    '')
  ];

  env = common.env // import ../lib/wasm-env.nix { inherit pkgs sdl3; };

  shellHook = ''
    ln -sf ${
      clang.clangdConfig [
        "Compiler: em++"
        "BuiltinHeaders: QueryDriver"
        "Remove: [--use-port=*, -sUSE_SDL*]"
        "CompilationDatabase: build/debug"
      ]
    } .clangd
  '';
}
