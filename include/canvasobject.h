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
        void setAtlasPos(int width, int height);

        int       atlas_x;
        int       atlas_y;
        int       atlas_width;
        int       atlas_height;

        float     tex_x;
        float     tex_y;
        float     tex_x2;
        float     tex_y2;
};

#endif // CANVASOBJECT_H
