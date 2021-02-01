/*
                    Conway's Game of Life program by Jussi Laitala in 2021, version 1.0


    Game of Life is cellular automaton devised by the British mathematician John Horton Conway in 1970.
    This programs logic follows same rules and cells on the map can be placed or erased with the mouse.
    Program ends by pressing escape or closing the window.

    This program is free to use, copy or modify.


*/


#include <iostream>
#include "game.h"

#define WIN_WIDTH   800
#define WIN_HEIGHT  900

int main()
{
    Game *game = new Game(WIN_WIDTH, WIN_HEIGHT);

    game->run();
    delete(game);

    return 0;
}
