#ifndef BUTTON_H
#define BUTTON_H

#include <stdio.h>
#include <string>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <cstring>

#include "canvasobject.h"
#include "canvas.h"
#include "fonts.h"

#define BUTTON_WIDTH     64
#define BUTTON_HEIGHT    32

using namespace std;

class Button : public CanvasObject
{
    public:
        Button(Canvas *canvas_, int _width, int _height, string _text, uint32_t _text_col);
        virtual ~Button();

        void setXY(int _x, int _y, int _siz);
        void uploadImage();
        void setState(bool _state);
        void setText(const string &txt);
        void setTextColor(uint32_t col);
        void mouseClick(const int &mx, const int &my);
        bool mouseRelease();
        int  mouseMove(const int &mx, const int &my);
        void draw();

    private:
        void init();
        bool lookMouseInside(const int &mx, const int &my);

        Canvas  *canvas;

        vector<uint32_t>  image;
        string   text;
        uint32_t text_col[2];
        float    xf1, xf2;
        float    yf1, yf2;
        int      width, height;
        int      x, y, siz;
        bool     grab;
        bool     state;
};

#endif // BUTTON_H
