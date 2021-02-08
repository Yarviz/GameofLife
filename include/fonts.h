#ifndef FONTS_H
#define FONTS_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>

#define FONTS   38

// Static class for drawing fonts on bitmap
class Fonts
{
    public:
        Fonts();
        virtual ~Fonts();

        static bool loadFonts();    // Load fonts from file

        static void text(const std::string &txt, uint32_t *dest, int x, int y,
                         int width, int height, uint32_t color, bool middle = false);   // Drawing text on destination bitmap

    private:
        static void initFonts();    // Initialize fonts

        static bool        inited;              // Flag if fonts already initialized
        static uint16_t    font_data[FONTS];    // Fonts data packed in 16 bits (3 * 5 bits)
        static int         font_num[128];       // Array table for printable fonts (taking from character (char) value)
};

#endif // FONTS_H
