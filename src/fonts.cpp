#include "fonts.h"

// Initialize static variables

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
    if (Fonts::inited == true) return true; // Return if fonts already initialized

    std::stringstream font_code;
    std::ifstream font_file;

    font_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    // Read fonts from file and catch possible errors

    try
    {
        font_file.open("fonts.txt");
        font_code << font_file.rdbuf();
        font_file.close();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "Error: Font File not succesfully read" << std::endl;
        return false;
    }

    uint16_t data = 0x0000;
    int counter = 0;
    int bits = 0;

    // Turn readed data to individual fonts. Each font is stored in
    // 16-bit unsigned short where 1-bit means pixel to draw. Each
    // font takes 3*5 bits = 15 bits.

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
                    initFonts();    // When every defined font is readed, initialize fonts and return
                    return true;
                }
            }
        }
    }

    return false;   // Return false if fonts are missing
}

void Fonts::initFonts()
{
    std::string fonts = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:";

    memset(&font_num, 0, sizeof(font_num));

    // Initialize font table with ASCII characters

    for (int i = 0; i < fonts.size(); i++)
    {
        font_num[(int)fonts[i]] = i;
    }

    inited = true;
}

void Fonts::text(const std::string &txt, uint32_t *dest, int x, int y, int width, int height, uint32_t color, bool middle)
{
    int xx, yy;
    int ch;
    uint16_t data;

    // Count text positions if text is setted to middle of bitmap

    if (middle)
    {
        x = x + width / 2 - (txt.size() * 4 / 2);
        y = y + height / 2 - 3;

        if (x < 0 || y < 0) return; // Return if outside bounds
    }

    for (const char &c: txt)
    {
        if (x + 3 > width) return;  // Return if outside bounds

        data = font_data[font_num[(int)c]]; // Get font data from font table based on current characters value

        for (yy = 0; yy < 5; yy++)
        {
            for (xx = 0; xx < 3; xx++)
            {
                // Get current font first bit and set pixel if bit = 1.
                // Then shift data bits to right to get next bit.

                data & 0x0001 ? dest[(y + yy) * width + x + xx] = color : 0;
                data >>= 1;
            }
        }

        x += 4;
    }
}
