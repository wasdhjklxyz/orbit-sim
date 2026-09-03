{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    systems.url = "github:nix-systems/default";
  };

  outputs = { self, nixpkgs, systems, }:
    let
      eachSystem = f: nixpkgs.lib.genAttrs (import systems)
        (system: f nixpkgs.legacyPackages.${system});
    in
    {
      packages = eachSystem (pkgs: import ./nix/packages { inherit pkgs; });
      devShells = eachSystem (pkgs: import ./nix/shells { inherit pkgs; });
    };
}
