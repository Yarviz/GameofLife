#ifndef CANVAS_H
#define CANVAS_H

#include <iostream>
#include <memory>
#include <vector>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>

#include "canvasobject.h"

#define TEXTURE_ATLAS_WIDTH     2048
#define TEXTURE_ATLAS_HEIGHT    512

using namespace std;
class CanvasObject;

class Canvas
{
    public:
        Canvas(int _width, int _height);
        virtual ~Canvas();

        void setTitle(const char *title);
        void destroy();
        void refreshContext();
        void draw();
        void repaint();
        void addChild(CanvasObject *object, int width, int height);
        void removeChild(CanvasObject *object);
        void updateTexture(const uint32_t *source, unsigned int dx, unsigned int dy, unsigned int dw, unsigned int dh);

        int getWidth() {return width;}
        int getHeight() {return height;}
        int getWindowWidth() {return w_width;}
        int getWindowHeight() {return w_height;}

        Display* getDisplay() {return display;}
        Window* getWindow() {return &window;}
        Window* getRootWindow() {return &root;}

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
        int                     width, height;
        int                     w_width, w_height;
        int                     atlas_x, atlas_y;
};

#endif // CANVAS_H
