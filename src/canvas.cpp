#include "canvas.h"

Canvas::Canvas(int _width, int _height)
{
    width = _width;
    height = _height;

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

    win_attr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask |PointerMotionMask;
    win_attr.colormap   = XCreateColormap(display, root, vi_info->visual, AllocNone);

    window = XCreateWindow(display, root, 0, 0, width, height, 0, vi_info->depth, InputOutput, vi_info->visual, CWEventMask  | CWColormap, &win_attr);

    XMapWindow(display, window);
    gc = XDefaultGC(display, 0);

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

    w_width = w_attr.width;
    w_height = w_attr.height;

    glViewport(0, 0, w_width, w_height);
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

void Canvas::addChild(CanvasObject *object, int width, int height)
{
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
    //glBindTexture(GL_TEXTURE_2D, 0);

    atlas_x = 0;
    atlas_y = 0;
}

void Canvas::updateTexture(const uint32_t *source, unsigned int dx, unsigned int dy, unsigned int dw, unsigned int dh)
{
    glTexSubImage2D(GL_TEXTURE_2D, 0, dx, dy, dw, dh, GL_RGBA, GL_UNSIGNED_BYTE, (void *)source);
}

Pixmap Canvas::createPixmap(int width, int height)
{
    return XCreatePixmap(display, root, width, height, vi_info->depth);
}

void Canvas::repaint()
{
    XMapWindow(display, window);
}

void Canvas::draw()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    if (!objects.empty())
    {
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
