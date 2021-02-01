#include "button.h"

Button::Button(Canvas *canvas_, int _width, int _height, string _text, uint32_t _text_col)
{
    canvas = canvas_;
    width = _width;
    height = _height;
    text = _text;
    state = true;

    image.resize(width * height * 2);

    setTextColor(_text_col);
    init();
}

Button::~Button()
{
    //dtor
}

void Button::init()
{
    int xx, yy;

    for (yy = 0; yy < height; yy++)
    {
        for (xx = 0; xx < width; xx++)
        {
            image[yy * width + xx] = 0xffaaaaaa;
            image[(height + yy) * width + xx] = 0xff555555;
        }
    }

    for(xx = 0; xx < width; xx++)
    {
        image[xx] = 0xffdddddd;
        image[(height - 1) * width + xx] = 0xff666666;

        image[(width * height) + xx] = 0xff888888;
        image[(width * (2 * height - 1)) + xx] = 0xff1111111;
    }

    for(yy = 0; yy < height; yy++)
    {
        image[yy * width] = 0xffdddddd;
        image[yy * width + width - 1] = 0xff666666;

        image[(height + yy) * width] = 0xff888888;
        image[(height + yy) * width + width - 1] = 0xff111111;
    }

    Fonts::text(text, image.data(), 0, 0, width, height, text_col[0], true);
    Fonts::text(text, image.data(), 0, height, width, height, text_col[1], true);
}

void Button::setTextColor(uint32_t col)
{
    text_col[0] = 0xff000000 | col;

    text_col[1] = 0xff000000;
    text_col[1] |= (int)((float)(col & 0x0000ff) * 0.7);
    text_col[1] |= (int)((float)(col & 0x00ff00) * 0.7);
    text_col[1] |= (int)((float)(col & 0xff0000) * 0.7);
}

void Button::setText(const string &txt)
{
    text = txt;

    init();
    uploadImage();
}

void Button::setState(bool _state)
{
    state = _state;

    if (atlas_x == -1) return;

    setAtlasPos(atlas_x, atlas_y + (height * !state), width, height);
}

void Button::setXY(int _x, int _y, int _siz)
{
    x = _x;
    y = _y;
    siz = _siz;

    xf1 = GL_X(x);
    xf2 = GL_X(x + width * siz);
    yf1 = GL_Y(y);
    yf2 = GL_Y(y + height * siz);
}

bool Button::lookMouseInside(const int &mx, const int &my)
{
    float mxf = GL_MX(mx);
    float myf = GL_MY(my);

    if (mxf < xf1 || mxf > xf2 || myf > yf1 || myf < yf2) return false;

    return true;
}

void Button::mouseClick(const int &mx, const int &my)
{
    if (!lookMouseInside(mx, my) || !state) return;

    setAtlasPos(atlas_x, atlas_y + height, width, height);
    grab = true;
}

bool Button::mouseRelease()
{
    if (!state) return false;

    setAtlasPos(atlas_x, atlas_y, width, height);
    if (grab)
    {
        grab = false;
        return true;
    }

    grab = false;
    return false;
}

int Button::mouseMove(const int &mx, const int &my)
{
    if (!grab) return -1;

    if (!lookMouseInside(mx, my)) setAtlasPos(atlas_x, atlas_y, width, height);
        else setAtlasPos(atlas_x, atlas_y + height, width, height);
}

void Button::uploadImage()
{
    if (atlas_x == -1) return;

    setAtlasPos(atlas_x, atlas_y, width, height);

    canvas->updateTexture(image.data(), atlas_x, atlas_y, width, height * 2);
}

void Button::draw()
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
