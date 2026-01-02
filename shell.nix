{ pkgs ? import <nixpkgs> { } }:
let
    inherit (pkgs) mkShell;
    inherit (pkgs.lib) makeLibraryPath;

    dependencyLibraries = with pkgs; [
        glfw
        raylib
        libGL
        xorg.libX11
        xorg.libXcursor
        xorg.libXrandr
        xorg.libXinerama
        xorg.libXi
        mesa
        libglvnd
    ];
in
    mkShell {
        packages = (with pkgs; [
            llvmPackages.clang
            gnumake
            gdb
        ]) ++ dependencyLibraries;

        shellHook = ''
        export CC=clang
        export CXX=clang++

        export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${ makeLibraryPath dependencyLibraries }
        export XDG_SESSION_TYPE=x11
        export GDK_BACKEND=wayland
        export SDL_VIDEODRIVER=wayland
        '';
    }
