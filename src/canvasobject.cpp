#include "canvasobject.h"

CanvasObject::CanvasObject()
{
    atlas_x = -1;
    atlas_y = -1;
}

CanvasObject::~CanvasObject()
{
    //dtor
}

void CanvasObject::setAtlasXY(int x, int y)
{
    atlas_x =  x;
    atlas_y =  y;
}

void CanvasObject::setAtlasPos(int x, int y, int width, int height)
{
    atlas_width =  width;
    atlas_height =  height;

    // Change texture atlas positions to fit range 0.0 - 1.0

    tex_x =  (float)x/ (float)TEXTURE_ATLAS_WIDTH;
    tex_y =  (float)y / (float)TEXTURE_ATLAS_HEIGHT;
    tex_x2 =  (float)(x + atlas_width) / (float)TEXTURE_ATLAS_WIDTH;
    tex_y2 =  (float)(y + atlas_height) / (float)TEXTURE_ATLAS_HEIGHT;
}
