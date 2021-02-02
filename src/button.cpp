#include "button.h"

Button::Button(Canvas *canvas_, int _width, int _height, string _text, uint32_t _text_col)
{
    canvas = canvas_;
    width = _width;
    height = _height;
    text = _text;
    state = true;

    image.resize(width * height * 2);   // Initialize image vector (multiply height by 2 for normal/pressed images)

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

    // Set images background colors to FFAAAAAA and FF555555 (ABGR).
    // Then paint image edges on bitmaps and draw texts for normal
    // and pressed buttons. This function is called every time when
    // button text or color is changed.

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

    // Draw button texts on center of both images

    Fonts::text(text, image.data(), 0, 0, width, height, text_col[0], true);
    Fonts::text(text, image.data(), 0, height, width, height, text_col[1], true);
}

void Button::setTextColor(uint32_t col)
{
    // Set button text colors. Color is stored in 32-bit unsigned integer
    // on order -> alpha, blue, green, red(8 bits each): Color channel order
    // is reversed because data is stored in little-endian order and
    // TexSubImage2D-function reads data starting from red channel bits.

    text_col[0] = 0xff000000 | col;

    // Set pressed button color little dimmer

    text_col[1] = 0xff000000;
    text_col[1] |= (int)((float)(col & 0x0000ff) * 0.7);
    text_col[1] |= (int)((float)(col & 0x00ff00) * 0.7);
    text_col[1] |= (int)((float)(col & 0xff0000) * 0.7);
}

void Button::setText(const string &txt)
{
    text = txt;

    init();
    uploadImage();  // Repaint image after text change
}

void Button::setState(bool _state)
{
    state = _state; // Set button active/inactive

    if (atlas_x == -1) return;  // Return if texture atlas position not initialized

    //  Set atlas position to match current button image

    setAtlasPos(atlas_x, atlas_y + (height * !state), width, height);
}

void Button::setXY(int _x, int _y, int _siz)
{
    x = _x;
    y = _y;
    siz = _siz;

    // Set button OpenGL position (-1.0 to 1.0) on canvas

    xf1 = GL_X(x);
    xf2 = GL_X(x + width * siz);
    yf1 = GL_Y(y);
    yf2 = GL_Y(y + height * siz);
}

bool Button::lookMouseInside(const int &mx, const int &my)
{
    // Change mouse coordinates to OpenGL coordinates

    float mxf = GL_MX(mx);
    float myf = GL_MY(my);

    // If outside button bounds, return false

    if (mxf < xf1 || mxf > xf2 || myf > yf1 || myf < yf2) return false;

    return true;
}

void Button::mouseClick(const int &mx, const int &my)
{
    if (!lookMouseInside(mx, my) || !state) return;

    // If button pressed set atlas position to match pressed button image

    setAtlasPos(atlas_x, atlas_y + height, width, height);
    grab = true;
}

bool Button::mouseRelease()
{
    if (!state) return false;

    // Set atlas position to match normal button image

    setAtlasPos(atlas_x, atlas_y, width, height);

    if (grab)
    {
        grab = false;   // If button is currently grabbed, return true (button clicked)
        return true;
    }

    grab = false;
    return false;
}

int Button::mouseMove(const int &mx, const int &my)
{
    if (!grab) return -1;

    // If button is grabbed, set button image to match pressed button if mouse position
    // is inside button bounds.

    if (!lookMouseInside(mx, my)) setAtlasPos(atlas_x, atlas_y, width, height);
        else setAtlasPos(atlas_x, atlas_y + height, width, height);
}

void Button::uploadImage()
{
    if (atlas_x == -1) return;  // Return if texture atlas position not initialized

    setAtlasPos(atlas_x, atlas_y, width, height);   // Set image texture atlas position

    // Update both button images on texture atlas (normal/pressed)

    canvas->updateTexture(image.data(), atlas_x, atlas_y, width, height * 2);
}

void Button::draw()
{
    // Get texture positions from atlas and draw button rectangle on canvas

    glTexCoord2f(tex_x, tex_y);
    glVertex2f(xf1, yf1);

    glTexCoord2f(tex_x2, tex_y);
    glVertex2f(xf2, yf1);

    glTexCoord2f(tex_x2, tex_y2);
    glVertex2f(xf2, yf2);

    glTexCoord2f(tex_x, tex_y2);
    glVertex2f(xf1, yf2);
}
