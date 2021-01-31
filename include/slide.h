#ifndef SLIDE_H
#define SLIDE_H

#include <stdio.h>
#include <string>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <cstring>

#include "canvasobject.h"
#include "canvas.h"

#define SLIDE_WIDTH     12
#define SLIDE_HEIGHT    12

using namespace std;

class Slide : public CanvasObject
{
    public:
        Slide(Canvas *canvas_);
        virtual ~Slide();

        void setXY(int _x, int _y, int _siz);
        void setSlidePos(int _x, int _x2, int min_i, int max_i);
        void uploadImage();
        void mouseClick(const int &mx, const int &my);
        void mouseRelease();
        int  mouseMove(const int &mx);
        void draw();

    private:
        void init();
        bool lookMouseInside(const int &mx, const int &my);

        Canvas  *canvas;

        static vector<uint32_t>  image;
        static bool     inited;

        float    xf1, xf2;
        float    yf1, yf2;
        float    item_ex;
        int      min_item;
        int      width, height;
        int      x, y, siz;
        int      sx1, sx2;
        bool     grab;
};

#endif // SLIDE_H
