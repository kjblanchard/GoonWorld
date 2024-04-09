# GoonWorld
- A C++ game that utilizes Tiled and Aseprite for the game "engine".
- Uses C self made libraries for physics, sound, and engine, found in the Libraries section
- ![Build All Platforms]( https://github.com/kjblanchard/GoonWorld/actions/workflows/test.yml/badge.svg)
- ![Status Picture](https://github.com/kjblanchard/GoonWorld/blob/master/img/status.gif?raw=true)
- Play the game here, or likely watch its "progress" [Supergoon.com](https://world.supergoon.com)
- Currently have 2 macos executables, as libpng cannot handle multiple architectures currently with a mac universal build, [issue here]()

## Tiled
- Configure the background, entities, etc
- ![Tiled](https://github.com/kjblanchard/GoonWorld/blob/master/img/tiled.png?raw=true)

## Aseprite
- Configure the Animations, time per frame, Sprites, etc
- ![Tiled](https://github.com/kjblanchard/GoonWorld/blob/master/img/aseprite.png?raw=true)

## Development
- Mostly developed on macos apple silicon
- Builds on Mac locally until game is playable, build on Windows, Universal Mac, Ubuntu, and Emscripten via github actions

## Goals
- Better at C
- Better at C++
- Learn about low-level programming for pictures (libpng) sound (ogg/openal), text (freetype) instead of using pre-made libraries
- Better at cross-platform building with cmake (Macos, Linux, Windows, Emscripten)
- Learn github actions to build and publish outside developer environment
- Create music in DAW
- Create Pixel art in Aseprite
- Create a simple "Super mario bros" clone.

### Stretch Goals
- Game build on Mobile (IOS priority)

## Components
- SDL2: Low Level handling of windowing, events, input
- OpenAL: Cross-Platform sound abstraction
- Emscripten: Build for web
- Make: Streamlining building with one command
- CMake: The actual cross-platform build system.
- GarageBand: DAW

## Licenses
### Full Game
- MIT
### Sounds
- [Mushroom](https://freesound.org/people/timgormly/sounds/170155/) - Attribution
- [Powerup](https://freesound.org/people/ProjectsU012/sounds/341629/) - Attribution
- [Fireball](https://freesound.org/people/adh.dreaming/sounds/615576/) - Creative Commons
- [Level2-ChiptuneBeat](https://freesound.org/people/YipYep/sounds/679167/) - Attribution
- [Level3-RetroFunk](https://freesound.org/people/sonically_sound/sounds/624874/) - Attribution
- [Level4-Castle](https://freesound.org/people/Sirkoto51/sounds/416632/) - Attribution

## Libraries
- [Supergoon Engine](https://github.com/supergoongaming/sg_engine) - MIT | Custom Physics, OpenAL wrapper, libpng/freetype wrapper, SDL wrapper, etc
- [SDL](https://www.libsdl.org/license.php) - ZLIB - Low level Windowing / Eventing
- [nlohmann json](https://github.com/nlohmann/json) - MIT - json parsing for appsettings file

## OpenGL
- [GladLink](https://glad.dav1d.de/#language=c&specification=gl&api=gl%3D3.3&api=gles1%3Dnone&api=gles2%3D2.0&api=glsc2%3Dnone&profile=core&loader=on)

## Valgrind Checks
- valgrind --track-origins=yes --leak-check=yes --leak-resolution=low --show-leak-kinds=definite ./SupergoonWorld 2>&1 | tee memcheck.txt