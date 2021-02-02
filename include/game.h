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
        string toHex(const uint32_t &value, int len);

        typedef struct {                // Mouse info
            bool button[2];
            bool hold;
            bool inside;
            int  x;
            int  y;
        }Mouse;

        shared_ptr<Canvas>  canvas;     // Canvas object for drawing (OpenGL 1.4 is used)
        CellMap    *cellmap;            // Object for cell map

        Label      *labelSize;          // Label objects for the canvas
        Label      *labelSpeed;
        Label      *labelColor;
        Slide      *slideSize;          // Slide objects for the canvas
        Slide      *slideSpeed;
        Slide      *slideColor;
        Button     *buttonShadows;      // Buttons for the canvas
        Button     *buttonClear;
        Button     *buttonRandom;
        Button     *buttonStop;
        Button     *buttonStep;

        Mouse       mouse;              // Mouse info
        int         speed;              // Cell animation speed (Frames per second)
        int         width, height;      // Game window size
        int         cell_siz;           // Cell map dimensions [cell_size * cell_size]
};

#endif // GAME_H
