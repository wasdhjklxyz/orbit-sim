{
  pkgs,
  llvm ? pkgs.emscripten.llvmEnv,
}:
let
  inherit (pkgs)
    writeText
    writeShellScriptBin
    ;
in
{
  clangTool = t: writeShellScriptBin t ''exec ${llvm}/bin/${t} "$@"'';

  clangdQueryDriver =
    driver:
    writeShellScriptBin "clangd" ''
      exec ${llvm}/bin/clangd --query-driver=${driver} "$@"
    '';

  clangdConfig =
    compileFlags:
    writeText "clangd-config" ''
      CompileFlags:
      ${pkgs.lib.concatMapStringsSep "\n" (l: "  " + l) compileFlags}
      Diagnostics:
        UnusedIncludes: Strict
        MissingIncludes: Strict
    '';
}
