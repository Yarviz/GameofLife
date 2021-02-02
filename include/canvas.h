#ifndef CANVAS_H
#define CANVAS_H

#include <iostream>
#include <memory>
#include <vector>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>

#include "canvasobject.h"

//  GPU Texture atlas size
#define TEXTURE_ATLAS_WIDTH     2048
#define TEXTURE_ATLAS_HEIGHT    512

using namespace std;
class CanvasObject;

class Canvas
{
    public:
        Canvas(int _width, int _height);
        virtual ~Canvas();

        void setTitle(const char *title);       // Set window title
        void destroy();                         // Destroy window
        void refreshContext();                  // Refresh window and OpenGL context
        void draw();                            // Draw window and it's objects
        void repaint();
        void addChild(CanvasObject *object, int width, int height);     // Add object in window for drawing
        void removeChild(CanvasObject *object);                         // Remove object
        void updateTexture(const uint32_t *source, unsigned int dx, unsigned int dy, unsigned int dw, unsigned int dh); // Update GPU texture atlas

        int getWidth() {return width;}              // Get window native resolution (not changing after creation)
        int getHeight() {return height;}
        int getWindowWidth() {return w_width;}      // Get window current resolution
        int getWindowHeight() {return w_height;}

        Display* getDisplay() {return display;}     // X11 window getters
        Window* getWindow() {return &window;}
        Window* getRootWindow() {return &root;}

    private:
        void initOpenGL();
        void createTextureAtlas();

        vector<CanvasObject*>   objects;            // Vector for drawable objects
        Display                *display;            // X11 display
        Window                  root, window;       // X11 canvas and root windows
        XVisualInfo            *vi_info;            // X11 visual info for canvas window
        XSetWindowAttributes    win_attr;           // window attributes
        GLXContext              glxc;               // OpenGL context for X11 window
        GC			            gc;                 // Graphics Context for display

        GLuint                  texture_id;         // GPU texture ID
        int                     width, height;      // Window native resolution
        int                     w_width, w_height;  // Window current resolution
        int                     atlas_x, atlas_y;   // Current position of texture atlas
};

#endif // CANVAS_H
