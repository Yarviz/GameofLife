#include "fonts.h"

bool        Fonts::inited = false;
uint16_t    Fonts::font_data[FONTS];
int         Fonts::font_num[128];

Fonts::Fonts()
{
    //
}

Fonts::~Fonts()
{
    //dtor
}


bool Fonts::loadFonts()
{
    if (Fonts::inited == true) return true;

    std::stringstream font_code;
    ifstream font_file;

    font_file.exceptions (ifstream::failbit | ifstream::badbit);

    try
    {
        font_file.open("fonts.txt");
        font_code << font_file.rdbuf();
        font_file.close();
    }
    catch (ifstream::failure e)
    {
        cout << "Error: Font File not succesfully read" << endl;
        return false;
    }

    uint16_t data = 0x0000;
    int counter = 0;
    int bits = 0;

    for (char& c : font_code.str())
    {
        if (c == '0' || c == '1')
        {
            if (c == '0') data >>= 1;
                else data = (data >> 1) | 0x8000;

            if (++bits == 15)
            {
                data >>= 1;
                font_data[counter] = data;
                data = 0x0000;
                bits = 0;
                if (++counter == FONTS)
                {
                    initFonts();
                    return true;
                }
            }
        }
    }

    return false;
}

void Fonts::initFonts()
{
    string fonts = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:";

    memset(&font_num, 0, sizeof(font_num));

    for (int i = 0; i < fonts.size(); i++)
    {
        font_num[(int)fonts[i]] = i;
    }

    inited = true;
}

void Fonts::text(const string &txt, uint32_t *dest, int x, int y, int width, int height, uint32_t color, bool middle)
{
    int xx, yy;
    int ch;
    uint16_t data;

    if (middle)
    {
        x = x + width / 2 - (txt.size() * 4 / 2);
        y = y + height / 2 - 3;

        if (x < 0 || y < 0) return;
    }

    for (const char &c: txt)
    {
        if (x + 3 > width) return;

        data = font_data[font_num[(int)c]];

        for (yy = 0; yy < 5; yy++)
        {
            for (xx = 0; xx < 3; xx++)
            {
                data & 0x0001 ? dest[(y + yy) * width + x + xx] = color : 0;
                data >>= 1;
            }
        }

        x += 4;
    }
}
