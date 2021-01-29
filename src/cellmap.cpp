#include "cellmap.h"

CellMap::CellMap(Canvas *canvas_)
{
    canvas = canvas_;

    memset(&cellmap, 0, sizeof(cellmap));

    uint32_t col[] = {
        0xff000000,
        0xff222222,
        0xff444444,
        0xff666666,
        0xff888888,
        0xffaaaaaa,
        0xffcccccc,
        0xffffffff
    };

    /*uint32_t col[] = {
        0xff000000,
        0xff0000ff,
        0xff0000ff,
        0xff0000ff,
        0xff0000ff,
        0xff0000ff,
        0xff0000ff,
        0xffffffff
    };*/

    memcpy(&colors, &col, sizeof(col));
    srand(time(NULL));
}

CellMap::~CellMap()
{
    //dtor
}

void CellMap::setSize(int _width, int _height)
{
    width = _width;
    height = _height;
}

void CellMap::initCellMap()
{
    //uint32_t color = 0x00000000;
    for(int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            cellpic[y * width + x] = colors[rand() % 8];
            //color += 8;
        }
    }

    canvas->updateTexture(cellpic[0], 0, 0, width, height);
}

void CellMap::draw()
{
    float x_size =  (float)width / (float)TEXTURE_ATLAS_WIDTH;
    float y_size =  (float)height / (float)TEXTURE_ATLAS_HEIGHT;

    glTexCoord2f(0.0, 0.0); glVertex3f(-0.5,  0.5, 0.0);
    glTexCoord2f(x_size, 0.0); glVertex3f( 0.5,  0.5, 0.0);
    glTexCoord2f(x_size, y_size); glVertex3f( 0.5, -0.5, 0.0);
    glTexCoord2f(0.0, y_size); glVertex3f(-0.5, -0.5, 0.0);
}
