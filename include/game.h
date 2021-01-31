#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <memory>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <chrono>
#include <thread>
#include <cstring>
#include <string>

#include "canvas.h"
#include "cellmap.h"
#include "label.h"

using namespace std;

class Game
{
    public:
        Game(int _width, int _height);
        virtual ~Game();

        void run();

    private:
        void init();

        typedef struct {
            bool button[2];
            bool hold;
            bool inside;
            int  x;
            int  y;
        }Mouse;

        shared_ptr<Canvas>  canvas;
        CellMap    *cellmap;
        Label      *labelSize;
        Label      *labelSpeed;
        Mouse       mouse;
        int         speed;
        int         width, height;
        int         cell_siz;
};

#endif // GAME_H
