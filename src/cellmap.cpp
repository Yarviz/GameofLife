#include "cellmap.h"

CellMap::CellMap(Canvas *canvas_)
{
    canvas = canvas_;

    memset(&cellmap, 0, sizeof(cellmap));
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
    uint32_t color = 0x000ff000;
    for(int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            cellpic[y * width + x] = color;
            color += 8;
        }
    }

    canvas->updateTexture(cellpic[0], 0, 0, width, height);
}

void CellMap::draw()
{
    glColor3f(1.0, 1.0, 1.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.5,  0.5, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f( 0.5,  0.5, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f( 0.5, -0.5, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, -0.5, 0.0);
}
