# 3D Snake

A simple 3D snake prototype built with SDL2 and OpenGL.

## Prerequisites

- A C11 compatible compiler (e.g. `gcc`, `clang`, or MSVC on Windows).
- [SDL2](https://www.libsdl.org/) development libraries.
- OpenGL (2.1 or newer). On Windows link against `opengl32`, on Linux link against `GL`, and on macOS use the `OpenGL` framework.

## Building

The project ships with a `Makefile`. On Linux or macOS you can compile the game with:

```bash
make
```

If SDL2 is not in the default search path, set `SDL_CFLAGS` and `SDL_LIBS` when invoking make:

```bash
make SDL_CFLAGS="$(sdl2-config --cflags)" SDL_LIBS="$(sdl2-config --libs)"
```

On Windows with MSYS2/MinGW you can use the same Makefile. When using another build environment, create a project/solution that compiles the sources in `src/` and links against SDL2 and OpenGL.

## Running

After a successful build the executable `./3dsnake` (or `3dsnake.exe` on Windows) is produced in the project root:

```bash
./3dsnake
```

### Controls

- **Arrow Keys**: Move the snake along the X and Y axes.
- **Page Up/Page Down**: Move the snake along the Z axis.
- **W/S**: Tilt the camera up/down.
- **A/D**: Rotate the camera left/right.
- **Q/E**: Zoom the camera in/out.
- **Space**: Pause/resume.
- **R**: Restart the game.
- **Enter**: Restart after game over.
- **Escape**: Quit.

Collect food to grow the snake and increase your score. Colliding with the grid boundaries or your own body ends the game.
