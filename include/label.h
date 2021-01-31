#ifndef LABEL_H
#define LABEL_H

#include <stdio.h>
#include <string>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <cstring>

#include "canvasobject.h"
#include "canvas.h"
#include "fonts.h"

using namespace std;

class Label : public CanvasObject
{
    public:
        Label(Canvas *canvas_, int _width, int _height);
        virtual ~Label();

        void setText(string _text, uint32_t color);
        void setXY(int _x, int _y, int siz);
        void uploadText();
        void draw();

    private:
        Canvas  *canvas;
        Fonts    font;

        vector<uint32_t> text_area;
        string   text;
        int      x, y;
        float    xf1, xf2;
        float    yf1, yf2;
        int      width, height;
        int      t_width;
};

#endif // LABEL_H
