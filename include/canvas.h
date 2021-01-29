#ifndef CANVAS_H
#define CANVAS_H

#include <iostream>
#include <memory>
#include <vector>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>

#include "canvasobject.h"

#define TEXTURE_ATLAS_WIDTH     1024
#define TEXTURE_ATLAS_HEIGHT    1024

using namespace std;

class Canvas
{
    public:
        Canvas(int width, int height);
        virtual ~Canvas();

        void setTitle(const char *title);
        void destroy();
        void refreshContext();
        void draw();
        void addChild(CanvasObject *object);
        void removeChild(CanvasObject *object);
        void updateTexture(const uint32_t &source, unsigned int dx, unsigned int dy, unsigned int dw, unsigned int dh);

        Display* getDisplay() {return display;}
        Window* getWindow() {return &window;}

    private:
        void initOpenGL();
        void createTextureAtlas();

        vector<CanvasObject*>   objects;
        Display                *display;
        Window                  root, window;
        XVisualInfo            *vi_info;
        XSetWindowAttributes    win_attr;
        GLXContext              glxc;
        GC			            gc;

        GLuint                  texture_id;
};

#endif // CANVAS_H
