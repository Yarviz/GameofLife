#ifndef FONT_H
#define FONT_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>

#define FONTS   38

using namespace std;

class Fonts
{
    public:
        Fonts();
        virtual ~Fonts();

        bool loadFonts();
        void text(const string &txt, uint32_t *dest, int x, int y, int width, int height);

    private:
        void initFonts();

        static bool        inited;
        static uint16_t    font_data[FONTS];
        static int         font_num[128];
};

#endif // FONT_H
