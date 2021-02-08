#include "canvas.h"

Canvas::Canvas(int _width, int _height)
{
    width = _width;
    height = _height;

    display = XOpenDisplay(NULL);   // Open display from X-server

    if(display == NULL)
    {
        std::cout << "Cannot open display." << std::endl;
        exit(EXIT_FAILURE);
    }

    root = DefaultRootWindow(display);          // Set default root window and screen
	int screenId = DefaultScreen(display);

	// Choose visual with OpenGL context attributes

    GLint vi_attributes[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
    vi_info = glXChooseVisual(display, 0, vi_attributes);

    if(vi_info == NULL)
    {
        std::cout << "Cannot get visual." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Set accepted events for window and create color map.

    win_attr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask |PointerMotionMask;
    win_attr.colormap   = XCreateColormap(display, root, vi_info->visual, AllocNone);

    // Create window with attributes

    window = XCreateWindow(display, root, 0, 0, width, height, 0, vi_info->depth, InputOutput, vi_info->visual, CWEventMask  | CWColormap, &win_attr);

    XMapWindow(display, window);    // Map window on display

    initOpenGL();
}

Canvas::~Canvas()
{
    //dtor
}

void Canvas::initOpenGL()
{
    // Create OpenGL context from display

    glxc = glXCreateContext(display, vi_info, NULL, GL_TRUE);

    if (glxc == NULL)
    {
        std::cout << "Cannot get OpenGL context." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Set current context active and set OpenGL viewport

    glXMakeCurrent(display, window, glxc);
    refreshContext();

    // Disable depth test and alpha blending

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    createTextureAtlas();
}

void Canvas::refreshContext()
{
    // Update window attributes and OpenGL viewport

    XWindowAttributes	w_attr;
    XGetWindowAttributes(display, window, &w_attr);

    w_width = w_attr.width;
    w_height = w_attr.height;

    glViewport(0, 0, w_width, w_height);
}

void Canvas::destroy()
{
    // Destroy window and its context

    glDeleteTextures(1, &texture_id);
    glXMakeCurrent(display, None, NULL);
    glXDestroyContext(display, glxc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
}

void Canvas::setTitle(const char *title)
{
    // Set window title

    XStoreName(display, window, title);
}

void Canvas::addChild(CanvasObject *object, int width, int height)
{
    // Add child object pointer to canvas and reserve space from texture atlas

    objects.push_back(object);

    if (atlas_x + width > TEXTURE_ATLAS_WIDTH)
    {
        atlas_x = 512;
        atlas_y += 64;
    }

    object->setAtlasXY(atlas_x, atlas_y);
    atlas_x += width;

    if (atlas_x > TEXTURE_ATLAS_WIDTH)
    {
        atlas_x = 512;
        atlas_y += 64;
    }
}

void Canvas::removeChild(CanvasObject *object)
{
    // Remove child object from object pointer vector

    vector<CanvasObject*>::iterator it;

    for(it = objects.begin(); it != objects.end(); it++)
    {
        if (*it == object)
        {
            objects.erase(it);
            return;
        }
    }
}

void Canvas::createTextureAtlas()
{
    // Create texture atlas in video memory

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEXTURE_ATLAS_WIDTH, TEXTURE_ATLAS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    //glBindTexture(GL_TEXTURE_2D, 0);

    atlas_x = 0;
    atlas_y = 0;
}

void Canvas::updateTexture(const uint32_t *source, unsigned int dx, unsigned int dy, unsigned int dw, unsigned int dh)
{
    // Update texture atlas from source data on specific position and dimensions

    glTexSubImage2D(GL_TEXTURE_2D, 0, dx, dy, dw, dh, GL_RGBA, GL_UNSIGNED_BYTE, (void *)source);
}

void Canvas::draw()
{
    // Clear color buffer

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw child objects

    if (!objects.empty())
    {
        // First draw lines for slides and rectangle for cell map

        glBindTexture(GL_TEXTURE_2D, 0);
        glColor3f(0.0, 1.0, 1.0);

        glBegin(GL_LINES);

        glVertex2f(-0.91,  0.61);
        glVertex2f( 0.91,  0.61);

        glVertex2f( 0.91,  0.61);
        glVertex2f( 0.91, -0.91);

        glVertex2f( 0.91, -0.91);
        glVertex2f(-0.91, -0.91);

        glVertex2f(-0.91, -0.91);
        glVertex2f(-0.91,  0.61);

        glVertex2f(-0.91,  0.90);
        glVertex2f(-0.41,  0.90);

        glVertex2f(-0.25,  0.90);
        glVertex2f( 0.25,  0.90);

        glVertex2f( 0.91,  0.90);
        glVertex2f( 0.41,  0.90);

        glEnd();

        // Then draw each children image from texture atlas

        glBindTexture(GL_TEXTURE_2D, texture_id);
        glBegin(GL_QUADS);

        for(CanvasObject *obj: objects)
        {
            if (obj != nullptr) obj->draw();
        }

        glEnd();
    }

    // Swap buffers with display

    glXSwapBuffers(display, window);
    XFlush(display);
}
