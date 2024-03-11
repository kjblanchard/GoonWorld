# GoonWorld
- A C++ game that utilizes Tiled and Aseprite for the game "engine".
- Uses C self made libraries for physics, sound, and engine

## Development
- Mostly developed on macos apple silicon
- Builds on Mac locally until game is playable

## Goals
- Better at C
- Better at C++
- Better at cross-platform building with cmake (Macos, Linux, Windows)
- Learn github actions to build and publish outside developer environment
- Create a simple "Super mario bros" clone.

### Stretch Goals
- Game build on Mobile (IOS priority)

## Components
- SDL2: Low Level handling of windowing, events, input
- OpenAL: Cross-Platform sound abstraction
- Emscripten: Build for web
- Make: Streamlining building with one command
- CMake: The actual cross-platform build system.
- cppclean: Tests

## Licenses
### Full Game
- MIT
### Sounds
- [Mushroom](https://freesound.org/people/timgormly/sounds/170155/) - Attribution
- [Powerup](https://freesound.org/people/ProjectsU012/sounds/341629/) - Attribution
- [Castle](https://freesound.org/people/Sirkoto51/sounds/416632/) - Attribution

## Libraries
- [Supergoon Sound](https://github.com/icculus/mojoAL/blob/main/LICENSE.txt) - ZLIB | OpenAL implementation - Mojoal wrapper
- [Supergoon Physics](https://github.com/kjblanchard/goonPhysics) - MIT | Simple AABB physics simulation
- [SDL](https://www.libsdl.org/license.php) - ZLIB - Low level Windowing / Eventing
- [doxygen](https://doxygen.nl) - GPL | Automatic documentation

## Valgrind
valgrind --track-origins=yes --leak-check=yes --leak-resolution=low --show-leak-kinds=definite ./SupergoonDash 2>&1 | tee memcheck.txt

<!-- Linux steps -->
cmake . -B build -DBUILD_TESTING=OFF -DJSON_BuildTests=OFF
git submodule init external/sg_sound
git submodule update external/sg_sound
<!-- That is in the json library -->