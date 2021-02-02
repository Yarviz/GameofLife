#include "slide.h"

// Initialize static bitmap vector

vector<uint32_t>  Slide::image;
bool     Slide::inited = false;

Slide::Slide(Canvas *canvas_)
{
    canvas = canvas_;

    width = SLIDE_WIDTH;
    height = SLIDE_HEIGHT;
    grab = false;

    if (!inited) init(); // Check if bitmap already initialized
}

Slide::~Slide()
{
    //dtor
}

void Slide::init()
{
    // Resize image vector and set it to color FFAAAAAA (ABGR).
    // Then paint image edges on bitmap.

    image.resize(SLIDE_WIDTH * SLIDE_HEIGHT, 0xffaaaaaa);

    for(int xx = 0; xx < SLIDE_WIDTH; xx++)
    {
        image[xx] = 0xffdddddd;
        image[(SLIDE_HEIGHT - 1) * SLIDE_WIDTH + xx] = 0xff666666;
    }

    for(int yy = 0; yy < SLIDE_HEIGHT; yy++)
    {
        image[yy * SLIDE_WIDTH] = 0xffdddddd;
        image[yy * SLIDE_WIDTH + SLIDE_WIDTH - 1] = 0xff666666;
    }

    inited = true;
}

void Slide::setXY(int _x, int _y, int _siz)
{
    x = _x;
    y = _y;
    siz = _siz;

    // Set slide OpenGL position (-1.0 to 1.0) on canvas

    xf1 = GL_X(x);
    xf2 = GL_X(x + width * siz);
    yf1 = GL_Y(y);
    yf2 = GL_Y(y + height * siz);
}

void Slide::setSlidePos(int _x, int _x2, int min_i, int max_i)
{
    sx1 = _x;
    sx2 = _x2;
    min_item = min_i;

    item_ex = (float)(max_i - min_i) / (float)(sx2 - sx1);
}

void Slide::uploadImage()
{
    if (atlas_x == -1) return;  // Return if texture atlas position not initialized

    setAtlasPos(atlas_x, atlas_y, width, height);   // Set image rectangle on texture atlas

    canvas->updateTexture(image.data(), atlas_x, atlas_y, width, height);
}

bool Slide::lookMouseInside(const int &mx, const int &my)
{
    // Change mouse coordinates to OpenGL coordinates

    float mxf = GL_MX(mx);
    float myf = GL_MY(my);

    // If outside label bounds, return false

    if (mxf < xf1 || mxf > xf2 || myf > yf1 || myf < yf2) return false;

    return true;
}

void Slide::mouseClick(const int &mx, const int &my)
{
    if (!lookMouseInside(mx, my)) return;
    grab = true;    // Set label grabbed if mouse clicked inside bounds
}

void Slide::mouseRelease()
{
    grab = false;   // Release grab
}

int Slide::mouseMove(const int &mx)
{
    if (!grab) return -1;

    // Change mouse x-coordinates to OpenGL coordinates relative to current window dimensions

    x = (float)(mx - width) * ((float)canvas->getWidth() / (float)canvas->getWindowWidth());

    // Check if coordinates are outside slide min/max coordinates

    (x < sx1) ? x = sx1 : 0;
    (x > sx2) ? x = sx2 : 0;

    // Set slide coordinates to respond mouse x-coordinates

    xf1 = GL_X(x);
    xf2 = GL_X(x + width * siz);

    // Return variable value relative to slide coordinates

    return min_item + (int)((float)(x - sx1) * item_ex);
}

void Slide::draw()
{
    // Get texture positions from atlas and draw slide rectangle on canvas

    glTexCoord2f(tex_x, tex_y);
    glVertex2f(xf1, yf1);

    glTexCoord2f(tex_x2, tex_y);
    glVertex2f(xf2, yf1);

    glTexCoord2f(tex_x2, tex_y2);
    glVertex2f(xf2, yf2);

    glTexCoord2f(tex_x, tex_y2);
    glVertex2f(xf1, yf2);
}
