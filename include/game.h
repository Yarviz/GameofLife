#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <memory>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include "canvas.h"
#include "cellmap.h"

#define WIN_WIDTH   800
#define WIN_HEIGHT  600

using namespace std;

class Game
{
    public:
        Game();
        virtual ~Game();

        void run();

    private:
        void init();

        shared_ptr<Canvas>  canvas;
        CellMap            *cellmap;
};

#endif // GAME_H
