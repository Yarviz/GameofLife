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

// Label class for drawing text on canvas
class Label : public CanvasObject
{
    public:
        Label(Canvas *canvas_, int _width, int _height);    // Set root canvas and text bitmap dimensions
        virtual ~Label();

        void setText(string _text, uint32_t color);         // Set text and color
        void setXY(int _x, int _y, int siz);                // Set label position and text size factor
        void uploadText();                                  // Upload text bitmap to texture atlas
        void draw();                                        // Draw label

    private:
        Canvas  *canvas;                    // Root canvas

        vector<uint32_t> text_area;         // Text bitmap
        string   text;                      // Text
        int      x, y;                      // Label position
        float    xf1, xf2;                  // Label position rectangle in OpenGL context
        float    yf1, yf2;
        int      width, height;             // Label dimensions
        int      t_width;                   // Current text width (text size * font width)
};

#endif // LABEL_H
