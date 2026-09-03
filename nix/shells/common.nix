{ pkgs, clangTool }:
{
  packages = with pkgs; [
    cmake
    ninja
    (clangTool "clang-format")
    (clangTool "clang-include-cleaner")
  ];
  env = {
    glm_ROOT = "${pkgs.glm}";
  };
}
