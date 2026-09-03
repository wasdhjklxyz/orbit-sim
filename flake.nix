{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    systems.url = "github:nix-systems/default";
  };

  outputs =
    {
      self,
      nixpkgs,
      systems,
    }:
    let
      inherit (nixpkgs) lib;
      eachSystem = lib.genAttrs (import systems);

      sdl3-wasm =
        system:
        let
          pkgs = nixpkgs.legacyPackages.${system};
        in
        pkgs.buildEmscriptenPackage (finalAttrs: {
          pname = "sdl3-wasm";
          version = "3.4.2";

          src = pkgs.fetchFromGitHub {
            owner = "libsdl-org";
            repo = "SDL";
            rev = "release-${finalAttrs.version}";
            hash = "sha256-ev0QiKyj0O6gtk7cK/V0X5Noft0Zo/fMS+oM6emwynE=";
          };

          nativeBuildInputs = with pkgs; [
            cmake
            ninja
          ];

          configurePhase = ''
            runHook preConfigure
            emcmake cmake -B build -S . -G Ninja \
              -DCMAKE_BUILD_TYPE=Release \
              -DCMAKE_INSTALL_PREFIX=$out \
              -DSDL_SHARED=OFF -DSDL_STATIC=ON \
              -DSDL_TESTS=OFF -DSDL_EXAMPLES=OFF
            runHook postConfigure
          '';

          buildPhase = ''
            runHook preBuild
            cmake --build build -j$NIX_BUILD_CORES
            runHook postBuild
          '';

          installPhase = ''
            runHook preInstall
            cmake --install build
            runHook postInstall
          '';

          # NOTE: Required for buildEmscriptenPackage
          checkPhase = ''
            runHook preCheck
            runHook postCheck
          '';
        });
    in
    {
      packages = eachSystem (
        system:
        let
          pkgs = nixpkgs.legacyPackages.${system};
          sdl3 = sdl3-wasm system;
        in
        {
          sdl3-wasm = sdl3;

          default = pkgs.buildEmscriptenPackage (finalAttrs: {
            name = "orbit-sim";
            src = lib.cleanSource ./.;

            nativeBuildInputs = with pkgs; [
              cmake
              ninja
            ];
            buildInputs = [ sdl3 ];

            env = {
              EM_TOOLCHAIN = "${pkgs.emscripten}/share/emscripten/cmake/Modules/Platform/Emscripten.cmake";
              SDL3_WASM_ROOT = "${sdl3}";
              glm_ROOT = "${pkgs.glm}";
              glm_SRC = "${pkgs.glm.src}";
            };

            configurePhase = ''
              runHook preConfigure
              cmake --preset release -DCMAKE_INSTALL_PREFIX=$out
              runHook postConfigure
            '';

            buildPhase = ''
              runHook preBuild
              cmake --build --preset release -j$NIX_BUILD_CORES
              runHook postBuild
            '';

            installPhase = ''
              runHook preInstall
              cmake --install build/release
              runHook postInstall
            '';

            # NOTE: Required for buildEmscriptenPackage
            checkPhase = ''
              runHook preCheck
              runHook postCheck
            '';
          });
        }
      );

      devShells = eachSystem (
        system:
        let
          pkgs = nixpkgs.legacyPackages.${system};
          sdl3 = sdl3-wasm system;
        in
        {
          default = pkgs.mkShell {
            packages = with pkgs; [
              sdl3
              emscripten
              cmake
              ninja
              (writeShellScriptBin "clangd" ''
                exec ${pkgs.emscripten.llvmEnv}/bin/clangd \
                  --query-driver='${pkgs.emscripten}/bin/em++' "$@"
              '')
              (writeShellScriptBin "clang-format" ''
                exec ${pkgs.emscripten.llvmEnv}/bin/clang-format "$@"
              '')
              (writeShellScriptBin "clang-include-cleaner" ''
                exec ${pkgs.emscripten.llvmEnv}/bin/clang-include-cleaner "$@"
              '')
              (writeShellScriptBin "cbr" ''
                set -e
                ${pkgs.emscripten}/bin/emcmake cmake \
                  -DCMAKE_CXX_SCAN_FOR_MODULES=OFF \
                  --preset "''${1:-debug}"
                ${pkgs.emscripten}/bin/emcmake cmake \
                  --build --preset "''${1:-debug}" -j$(nproc)
                ${pkgs.emscripten}/bin/emrun \
                  --verbose build/''${1:-debug}/bin/*.html
              '')
              (writeShellScriptBin "cbr-native" ''
                cmake --preset native && \
                cmake --build --preset native -j$(nproc) && \
                ./build/native/bin/gsb
              '')
            ];
            env = {
              EM_TOOLCHAIN = "${pkgs.emscripten}/share/emscripten/cmake/Modules/Platform/Emscripten.cmake";
              SDL3_WASM_ROOT = "${sdl3}";
              SDL3_NATIVE_ROOT = "${pkgs.sdl3.dev}";
              OpenGL_ROOT = "${pkgs.libglvnd}:${pkgs.libglvnd.dev}";
              glm_ROOT = "${pkgs.glm}";
              glm_SRC = "${pkgs.glm.src}";
            };
          };
        }
      );
    };
}
