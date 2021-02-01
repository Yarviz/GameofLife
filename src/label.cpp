#include "label.h"

Label::Label(Canvas *canvas_, int _width, int _height)
{
    canvas = canvas_;
    width = _width;
    height = _height;

    text_area.resize(width * height, 0);
}

Label::~Label()
{
    //
}

void Label::setText(string _text, uint32_t color)
{
    text = _text;

    memset(text_area.data(), 0, sizeof(uint32_t) * text_area.size());
    Fonts::text(text, text_area.data(), 0, 0, width, height, color);

    t_width = text.size() * 4;
    setXY(x, y, text_size);
}

void Label::uploadText()
{
    if (atlas_x == -1) return;

    setAtlasPos(atlas_x, atlas_y, t_width, height);

    canvas->updateTexture(text_area.data(), atlas_x, atlas_y, width, height);
}

void Label::setXY(int _x, int _y, int siz)
{
    x = _x;
    y = _y;
    text_size = siz;

    xf1 = -1.0 + (float)x / (float)canvas->getWidth() * 2;
    xf2 = -1.0 + (float)(x + t_width * siz) / (float)canvas->getWidth() * 2;
    yf1 = 1.0 - (float)y / (float)canvas->getHeight() * 2;
    yf2 = 1.0 - (float)(y + height * siz) / (float)canvas->getHeight() * 2;
}

void Label::draw()
{
    glTexCoord2f(tex_x, tex_y);
    glVertex2f(xf1, yf1);

    glTexCoord2f(tex_x2, tex_y);
    glVertex2f(xf2, yf1);

    glTexCoord2f(tex_x2, tex_y2);
    glVertex2f(xf2, yf2);

    glTexCoord2f(tex_x, tex_y2);
    glVertex2f(xf1, yf2);

    //cout << tex << endl;
}
