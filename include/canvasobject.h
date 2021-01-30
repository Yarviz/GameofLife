#ifndef CANVASOBJECT_H
#define CANVASOBJECT_H

#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>

#include "canvas.h"

class CanvasObject
{
    public:
        CanvasObject();
        virtual ~CanvasObject();

        virtual void draw() {};
        void setAtlasXY(int x, int y);

    protected:
        void setAtlasSize(int width, int height);

        float     atlas_x;
        float     atlas_y;
        float     atlas_width;
        float     atlas_height;
};

#endif // CANVASOBJECT_H
