# GameofLife

This program is linux version of Conway's Game of Life.

X11 development libraries and GLX libraries needed (and openGL libraries): 

    sudo apt-get install libx11-dev libgl1-mesa-glx libgl

Compiled with g++ (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0

## Compiling files:

    g++ -std=c++11 -Wall -g -o gameoflife -Iinclude main.cpp src/*.cpp -lGL -lX11

## Run program:

    ./gameoflife
