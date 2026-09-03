{ pkgs }:
let
  clang = import ../lib/clangd.nix { inherit pkgs; };
  common = import ./common.nix {
    inherit pkgs;
    inherit (clang) clangTool;
  };
in
pkgs.mkShell {
  packages = common.packages ++ [
    (clang.clangdQueryDriver "$(command -v g++)")
    (pkgs.writeShellScriptBin "cbr" ''
      cmake --preset native && \
      cmake --build --preset native -j$(nproc) && \
      ./build/native/bin/gsb
    '')
  ];

  env = common.env // {
    SDL3_NATIVE_ROOT = "${pkgs.sdl3.dev}";
    OpenGL_ROOT = "${pkgs.libglvnd}:${pkgs.libglvnd.dev}";
  };

  shellHook = ''
    ln -sf ${
      clang.clangdConfig [
        "BuiltinHeaders: QueryDriver"
        "CompilationDatabase: build/native"
      ]
    } .clangd
  '';
}
