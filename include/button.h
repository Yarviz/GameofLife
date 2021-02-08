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

// Button default dimensions
#define BUTTON_WIDTH     64
#define BUTTON_HEIGHT    32

// Class for canvas buttons
class Button : public CanvasObject
{
    public: // Set buttons root canvas, dimensions, text and text color
        Button(Canvas *canvas_, int _width, int _height, string _text, uint32_t _text_col);
        virtual ~Button();

        void setXY(int _x, int _y, int _siz);            // Set button position and size factor
        void uploadImage();                              // Upload bitmap to texture atlas
        void setState(bool _state);                      // Set button active/inactive
        void setText(const string &txt);                 // Set button text
        void setTextColor(uint32_t col);                 // Set text color
        void mouseClick(const int &mx, const int &my);   // Mouse click callback
        bool mouseRelease(const int &mx, const int &my); // Mouse release callback
        int  mouseMove(const int &mx, const int &my);    // Mouse move callback
        void draw();                                     // Drawing on canvas

    private:
        void init();                                            // Initialize button bitmap (redraw after changing text/text color)
        bool lookMouseInside(const int &mx, const int &my);     // Check if mouse inside button rectangle

        Canvas  *canvas;            // Root canvas

        vector<uint32_t>  image;    // Button bitmap
        string   text;              // Button text
        uint32_t text_col[2];       // Text color for normal and pressed buttons
        float    xf1, xf2;          // Button position rectangle in OpenGL context
        float    yf1, yf2;
        int      width, height;     // Button dimensions, position and size factor
        int      x, y, siz;
        bool     grab;              // Flag for mouse grab
        bool     state;             // Button state (active/inactive)
};

#endif // BUTTON_H
