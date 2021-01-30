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
