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

// Slide default dimensions
#define SLIDE_WIDTH     12
#define SLIDE_HEIGHT    12

// Class for sliding object on canvas
class Slide : public CanvasObject
{
    public:
        Slide(Canvas *canvas_);     // Set root canvas
        virtual ~Slide();

        void setXY(int _x, int _y, int _siz);                       // Set position and size factor
        void setSlidePos(int _x, int _x2, int min_i, int max_i);    // Set slides min/max positions and scalable variable min/max values
        void uploadImage();                                         // Upload bitmap on texture atlas
        void mouseClick(const int &mx, const int &my);              // Mouse callback functions
        void mouseRelease();
        int  mouseMove(const int &mx);
        void draw();                                                // Drawing on canvas

    private:
        void init();                                                // Initialize slide bitmap
        bool lookMouseInside(const int &mx, const int &my);         // Check if mouse inside slide rectangle

        Canvas  *canvas;                // Root canvas

        static vector<uint32_t>  image; // Slide bitmap (every object have same bitmap)
        static bool     inited;         // Flag for initialized bitmap

        float    xf1, xf2;              // Slide position rectangle in OpenGL context
        float    yf1, yf2;
        float    item_ex;               // Factor to calculate variable value from slide position
        int      min_item;              // Slide variable minimum value
        int      width, height;         // Slide dimensions, position and size factor
        int      x, y, siz;
        int      sx1, sx2;
        bool     grab;                  // Flag if slide is grabbed
};

#endif // SLIDE_H
