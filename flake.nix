{
  description = "C++ clang based project flake.";

  inputs = {
    nixpkgs-unstable.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { nixpkgs-unstable, ... }:
    let
      pkgs = nixpkgs-unstable.legacyPackages."x86_64-linux";
    in
  {
    devShells."x86_64-linux".default = pkgs.mkShell {
      packages = with pkgs; [
        gcc # GNU c/c++ compiler.
        clang # Clang c/c++ compiler.
        clang-tools 
        cmake # Builder.
        gnumake # Executable generator.
        gdb # C/C++ debugger.
        valgrind # Memory and memory leak debugger.

        # Nix tools
        nil # Analysis assistant.
        nixd # Nix language server.
      ];
      inputsFrom = [];
      shellHook = ''
        clang++ --version
      '';
      CC="clang";
      CXX="clang++";
      CMAKE_GENERATOR="Unix Makefiles";
    };
  };
}
