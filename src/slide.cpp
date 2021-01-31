#include "slide.h"

vector<uint32_t>  Slide::image;
bool     Slide::inited = false;

Slide::Slide(Canvas *canvas_)
{
    canvas = canvas_;

    width = SLIDE_WIDTH;
    height = SLIDE_HEIGHT;
    grab = false;

    if (!inited) init();
}

Slide::~Slide()
{
    //dtor
}

void Slide::init()
{
    image.resize(SLIDE_WIDTH * SLIDE_HEIGHT, 0xffaaaaaa);

    for(int xx = 0; xx < SLIDE_WIDTH; xx++)
    {
        image[xx] = 0xffdddddd;
        image[(SLIDE_HEIGHT - 1) * SLIDE_WIDTH + xx] = 0xffdddddd;
    }

    for(int yy = 0; yy < SLIDE_HEIGHT; yy++)
    {
        image[yy * SLIDE_WIDTH] = 0xffdddddd;
        image[yy * SLIDE_WIDTH + SLIDE_WIDTH - 1] = 0xffdddddd;
    }

    inited = true;
}

void Slide::setXY(int _x, int _y, int _siz)
{
    x = _x;
    y = _y;
    siz = _siz;

    xf1 = -1.0 + (float)x / (float)canvas->getWidth() * 2;
    xf2 = -1.0 + (float)(x + width * siz) / (float)canvas->getWidth() * 2;
    yf1 = 1.0 - (float)y / (float)canvas->getHeight() * 2;
    yf2 = 1.0 - (float)(y + height * siz) / (float)canvas->getHeight() * 2;
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
    if (atlas_x == -1) return;

    setAtlasPos(width, height);

    canvas->updateTexture(image.data(), atlas_x, atlas_y, width, height);
}

bool Slide::lookMouseInside(const int &mx, const int &my)
{
    float mxf = -1.0 + (float)mx / (float)canvas->getWindowWidth() * 2;
    float myf =  1.0 - (float)my / (float)canvas->getWindowHeight() * 2;

    if (mxf < xf1 || mxf > xf2 || myf > yf1 || myf < yf2) return false;

    return true;
}

void Slide::mouseClick(const int &mx, const int &my)
{
    if (!lookMouseInside(mx, my)) return;
    grab = true;
}

void Slide::mouseRelease()
{
    grab = false;
}

int Slide::mouseMove(const int &mx)
{
    if (!grab) return -1;

    x = (float)(mx - width) * ((float)canvas->getWidth() / (float)canvas->getWindowWidth());

    (x < sx1) ? x = sx1 : 0;
    (x > sx2) ? x = sx2 : 0;

    xf1 = -1.0 + (float)x / (float)canvas->getWidth() * 2;
    xf2 = -1.0 + (float)(x + width * siz) / (float)canvas->getWidth() * 2;

    return min_item + (int)((float)(x - sx1) * item_ex);
}

void Slide::draw()
{
    glTexCoord2f(tex_x, tex_y);
    glVertex2f(xf1, yf1);

    glTexCoord2f(tex_x2, tex_y);
    glVertex2f(xf2, yf1);

    glTexCoord2f(tex_x2, tex_y2);
    glVertex2f(xf2, yf2);

    glTexCoord2f(tex_x, tex_y2);
    glVertex2f(xf1, yf2);
}
