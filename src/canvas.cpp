#include "canvas.h"

Canvas::Canvas(int width, int height)
{
    display = XOpenDisplay(NULL);

    if(display == NULL)
    {
        cout << "Cannot open display." << endl;
        exit(EXIT_FAILURE);
    }

    root = DefaultRootWindow(display);
	int screenId = DefaultScreen(display);

    GLint vi_attributes[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
    vi_info = glXChooseVisual(display, 0, vi_attributes);

    if(vi_info == NULL)
    {
        cout << "Cannot get visual." << endl;
        exit(EXIT_FAILURE);
    }

    win_attr.event_mask = ExposureMask | KeyPressMask;
    win_attr.colormap   = XCreateColormap(display, root, vi_info->visual, AllocNone);

    window = XCreateWindow(display, root, 0, 0, width, height, 0, vi_info->depth, InputOutput, vi_info->visual, CWEventMask  | CWColormap, &win_attr);

    XMapWindow(display, window);

    initOpenGL();
}

Canvas::~Canvas()
{
    //dtor
}

void Canvas::initOpenGL()
{
    glxc = glXCreateContext(display, vi_info, NULL, GL_TRUE);

    if (glxc == NULL)
    {
        cout << "Cannot get OpenGL context." << endl;
        exit(EXIT_FAILURE);
    }

    glXMakeCurrent(display, window, glxc);
    refreshContext();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    createTextureAtlas();
}

void Canvas::refreshContext()
{
    XWindowAttributes	w_attr;

    XGetWindowAttributes(display, window, &w_attr);
    glViewport(0, 0, w_attr.width, w_attr.height);
}

void Canvas::destroy()
{
    glDeleteTextures(1, &texture_id);
    glXMakeCurrent(display, None, NULL);
    glXDestroyContext(display, glxc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
}

void Canvas::setTitle(const char *title)
{
    XStoreName(display, window, title);
}

void Canvas::addChild(CanvasObject *object)
{
    objects.push_back(object);
}

void Canvas::removeChild(CanvasObject *object)
{
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
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEXTURE_ATLAS_WIDTH, TEXTURE_ATLAS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Canvas::updateTexture(const uint32_t &source, unsigned int dx, unsigned int dy, unsigned int dw, unsigned int dh)
{
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, dx, dy, dw, dh, GL_RGBA, GL_UNSIGNED_BYTE, (void *)&source);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Canvas::draw()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    if (!objects.empty())
    {
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glBegin(GL_QUADS);

        for(CanvasObject *obj: objects)
        {
            if (obj != nullptr) obj->draw();
        }

        glEnd();
    }

    glXSwapBuffers(display, window);
    XFlush(display);
}
