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
      eachSystem = nixpkgs.lib.genAttrs (import systems);
    in
    {
      devShells = eachSystem (
        system:
        let
          pkgs = nixpkgs.legacyPackages.${system};
        in
        {
          default = pkgs.mkShell {
            packages = with pkgs; [
              emscripten
              cmake
              (writeShellScriptBin "clangd" ''
                exec ${pkgs.emscripten.llvmEnv}/bin/clangd \
                  --query-driver='${pkgs.emscripten}/bin/em++' "$@"
              '')
            ];
          };
        }
      );
    };
}
