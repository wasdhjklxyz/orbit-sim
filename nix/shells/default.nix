{ pkgs }:
{
  default = import ./native.nix { inherit pkgs; };

  web = import ./web.nix {
    inherit pkgs;
    sdl3 = import ../packages/sdl3-wasm.nix { inherit pkgs; };
  };
}
