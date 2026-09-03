{
  pkgs,
}:
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
})
