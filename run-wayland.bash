#!/usr/bin/env bash

export XDG_SESSION_TYPE=x11
export GDK_BACKEND=wayland
export SDL_VIDEODRIVER=wayland

./build/dooz
