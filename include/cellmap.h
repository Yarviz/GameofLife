#ifndef CELLMAP_H
#define CELLMAP_H

#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <cstring>
#include <random>

#include "canvasobject.h"
#include "canvas.h"

#define CELL_MAP_WIDTH    512
#define CELL_MAP_HEIGHT   512
#define MAX_COLORS        8
#define COLOR_WHEEL_SIZE  176

#define LIVE              1
#define DEAD              0

#define X_START           -0.9
#define X_END              0.9
#define Y_START            0.6
#define Y_END             -0.9
#define X_SIZE             1.8
#define Y_SIZE             1.5

#define CELL_COLOR         0xffffff


class CellMap : public CanvasObject
{
    public:
        CellMap(Canvas *canvas_);
        virtual ~CellMap();

        void draw();
        void updateTexture();
        void setSize(int _width, int _height);
        void randomCellMap(int rnd);
        void animateCells();
        void mouseClick(const int &x, const int &y, const int button);
        void setCellColor(uint8_t r, uint8_t g, uint8_t b);
        void setColorWheelColor(int i, bool stopped);
        void toggleAnimate();
        bool getAnimate() {return animate;}
        void clearCellMap(bool all);
        uint32_t getColor() {return colors[1];}

    private:
        void checkCell(const int &x,const int &y, const int &n_cells);
        void countCenterCells(const int &x,const int &y);
        void countEdgeCells(const int &x,const int &y);
        bool lookMouseInside(const int &x, const int &y);
        void initColorWheel();
        void updateCellMapColors();

        Canvas  *canvas;

        typedef struct {
            uint8_t anim;
            uint8_t cell[2];
        }Cell;

        typedef struct {
            uint8_t r;
            uint8_t g;
            uint8_t b;
        }RGB_Col;

        Cell     cellmap[CELL_MAP_WIDTH][CELL_MAP_HEIGHT];
        uint32_t cellpic[CELL_MAP_WIDTH * CELL_MAP_HEIGHT];
        uint32_t a_colors[MAX_COLORS];
        uint32_t colors[2];
        RGB_Col  color_wheel[COLOR_WHEEL_SIZE];
        int      add_xy[8][2];
        int      width;
        int      height;
        int      cur_gen, last_gen;
        int      cmx, cmy;
        bool     animate;
        bool     update;
};

#endif // CELLMAP_H
