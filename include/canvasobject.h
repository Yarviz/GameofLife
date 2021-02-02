#ifndef CANVASOBJECT_H
#define CANVASOBJECT_H

#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>

#include "canvas.h"

// Change x and y coordinates to OpenGL coordinates (-1.0 to 1.0)
// relative to window native resolution
#define GL_X(x)     (-1.0 + (float)(x) / (float)canvas->getWidth() * 2)
#define GL_Y(y)     (1.0 - (float)(y) / (float)canvas->getHeight() * 2)

// Change x and y coordinates to OpenGL coordinates (-1.0 to 1.0)
// relative to window current resolution
#define GL_MX(x)     (-1.0 + (float)(x) / (float)canvas->getWindowWidth() * 2)
#define GL_MY(y)     (1.0 - (float)(y) / (float)canvas->getWindowHeight() * 2)

// Class for objects to draw on window canvas
class CanvasObject
{
    public:
        CanvasObject();
        virtual ~CanvasObject();

        virtual void draw() {};

        void setAtlasXY(int x, int y);      // Set object coordinates in texture atlas

    protected:
        void setAtlasPos(int x, int y, int width, int height);  // Set object clip rectangle from texture atlas

        int       atlas_x;          // Object texture atlas coordinates
        int       atlas_y;
        int       atlas_width;      // Object texture atlas dimensions
        int       atlas_height;

        float     tex_x;            // Object texture atlas coordinates packed between ranges 0.0 to 1.0
        float     tex_y;
        float     tex_x2;
        float     tex_y2;
};

#endif // CANVASOBJECT_H
