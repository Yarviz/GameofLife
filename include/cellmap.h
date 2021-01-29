#ifndef CELLMAP_H
#define CELLMAP_H

#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <cstring>
#include <random>

#include "canvasobject.h"
#include "canvas.h"

#define CELL_MAP_WIDTH    256
#define CELL_MAP_HEIGHT   256
#define MAX_COLORS        8

class CellMap : public CanvasObject
{
    public:
        CellMap(Canvas *canvas_);
        virtual ~CellMap();

        void draw();
        void setSize(int _width, int _height);
        void initCellMap();

    private:

        Canvas  *canvas;

        int      width;
        int      height;
        uint8_t  cellmap[2][CELL_MAP_WIDTH][CELL_MAP_HEIGHT];
        uint32_t cellpic[CELL_MAP_WIDTH * CELL_MAP_HEIGHT];
        uint32_t colors[MAX_COLORS];
};

#endif // CELLMAP_H
