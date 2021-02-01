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
#include "slide.h"
#include "button.h"
#include "fonts.h"

using namespace std;

class Game
{
    public:
        Game(int _width, int _height);
        virtual ~Game();

        void run();

    private:
        void   init();
        string toHex(uint32_t value, int len);

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
        Label      *labelColor;
        Slide      *slideSize;
        Slide      *slideSpeed;
        Slide      *slideColor;
        Button     *buttonShadows;
        Button     *buttonClear;
        Button     *buttonRandom;
        Button     *buttonStop;
        Button     *buttonStep;

        Mouse       mouse;
        int         speed;
        int         width, height;
        int         cell_siz;
};

#endif // GAME_H
