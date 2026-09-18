# BlackJack2

My first major project with C++ development.

## The Lore

I wrote this in high school as part of two class projects: My first C++ class (as terminal only), and my second (where I added SFML graphics). This was the end product upon graduation, in all its disorganized glory. I uploaded this not because it's good, but because it's the first notable thing I made with C++.

The whole thing was written in C++11 with Code::Blocks 17.12 and compiled using a class-specific dev environment, so I couldn't figure out how to migrate it... **until now.**

## Building it with CMAKE

The project now builds with CMake+SFML 2.6.1. Game code is generally untouched and the cbp file (`sasha.cbp`) is kept so you can still use Code::Blocks. You need CMake 3.21+, Git, and a C++ compiler.

Windows (via Visual Studio 2022 x64):

```bat
cmake -B build -S . -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
cd build/Release
.\sasha.exe
```

Linux (Tested and functional on Linux Mint):

```sh
sudo apt update && sudo apt install g++ cmake git \
  libxrandr-dev libxcursor-dev libxi-dev libudev-dev \
  libfreetype-dev libflac-dev libvorbis-dev libogg-dev \
  libgl1-mesa-dev libegl1-mesa-dev libopenal-dev
cmake -B build -S .
cmake --build build
cd build
./sasha
```

Note that you MUST run the exe from the build output folder (build/Release), otherwise the assets (ex. card dir) won't load.

**I would also like to note that the card assets used in this project were taken from another open source Github project which I cannot find again. Whatever it was and whoever made it, thank you!**