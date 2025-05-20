# idk

<https://wiki.libsdl.org/SDL2/Installation>
gcc -o chickenjockey main.c `sdl2-config --cflags --libs`

cmake -S . -B build
cmake --build build
./build/sdl-helloworld
