#include "label.h"

Label::Label(Canvas *canvas_, int _width, int _height)
{
    canvas = canvas_;
    width = _width;
    height = _height;

    text_area.resize(width * height, 0);    // Initialize bitmap vector
}

Label::~Label()
{
    //
}

void Label::setText(string _text, uint32_t color)
{
    text = _text;

    // Clear bitmap vector data and draw text on bitmap

    memset(text_area.data(), 0, sizeof(uint32_t) * text_area.size());
    Fonts::text(text, text_area.data(), 0, 0, width, height, color);

    t_width = text.size() * 4;  // Save text width
    
    setXY(x, y, text_size);   // Update text rectangle
}

void Label::uploadText()
{
    if (atlas_x == -1) return;  // Return if texture atlas position not initialized

    setAtlasPos(atlas_x, atlas_y, t_width, height); // Set current text image rectangle on texture atlas

    canvas->updateTexture(text_area.data(), atlas_x, atlas_y, width, height);
}

void Label::setXY(int _x, int _y, int siz)
{
    x = _x;
    y = _y;
    text_size = siz;

    // Set label OpenGL position (-1.0 to 1.0) on canvas

    xf1 = GL_X(x);
    xf2 = GL_X(x + t_width * siz);
    yf1 = GL_Y(y);
    yf2 = GL_Y(y + width * siz);
}

void Label::draw()
{
    // Get texture positions from atlas and draw label rectangle on canvas

    glTexCoord2f(tex_x, tex_y);
    glVertex2f(xf1, yf1);

    glTexCoord2f(tex_x2, tex_y);
    glVertex2f(xf2, yf1);

    glTexCoord2f(tex_x2, tex_y2);
    glVertex2f(xf2, yf2);

    glTexCoord2f(tex_x, tex_y2);
    glVertex2f(xf1, yf2);
}
