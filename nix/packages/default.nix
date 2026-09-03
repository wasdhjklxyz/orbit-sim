{ pkgs }:
let
  sdl3 = import ./sdl3-wasm.nix { inherit pkgs; };
in
{
  sdl3-wasm = sdl3;

  default = pkgs.buildEmscriptenPackage {
    name = "orbit-sim";
    src = pkgs.lib.cleanSource ../../.;

    nativeBuildInputs = with pkgs; [
      cmake
      ninja
    ];
    buildInputs = [ sdl3 ];

    env = import ../lib/wasm-env.nix { inherit pkgs sdl3; };

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
  };
}
