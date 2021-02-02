# GameofLife

This program is linux version of Conway's Game of Life.

X11 development libraries needed: **sudo apt install libx11-dev**

Compiled with g++ (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0

## Compiling files:

**g++ -std=c++11 -Wall -g -o gameoflife -Iinclude main.cpp src/*.cpp -lGL -lX11**

## Run program:

**./gameoflife**
