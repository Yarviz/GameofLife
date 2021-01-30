#include "canvasobject.h"

CanvasObject::CanvasObject()
{
    //ctor
}

CanvasObject::~CanvasObject()
{
    //dtor
}

void CanvasObject::setAtlasXY(int x, int y)
{
    atlas_x =  (float)x / (float)TEXTURE_ATLAS_WIDTH;
    atlas_y =  (float)y / (float)TEXTURE_ATLAS_HEIGHT;
}
