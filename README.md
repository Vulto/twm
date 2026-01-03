# twm - Tiny Window Manager

A minimal X11 window manager inspired by tinywm, with suckless-style compile-time configuration.

## Features
- Mouse: Super + Left drag to move, Super + Right drag to resize
- Keyboard:
  - Super + F1: Raise window under cursor
  - Super + C: Close focused window
  - Super + Return: Spawn st terminal
  - Super + Q: Quit twm
- No runtime config - customize in `config.h` and recompile

## Requirements
- Xlib (libX11)

## Building
./c -ci 
See ./c without arguments for help on compiling
Edit nob.c for custom building

Its a old version of nob.c (Tsoding-style) build script.
