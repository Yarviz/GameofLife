#ifndef CANVASOBJECT_H
#define CANVASOBJECT_H

#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>

class CanvasObject
{
    public:
        CanvasObject();
        virtual ~CanvasObject();

        virtual void draw() {};

    private:
};

#endif // CANVASOBJECT_H
