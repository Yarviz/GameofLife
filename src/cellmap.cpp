#include "cellmap.h"

CellMap::CellMap(Canvas *canvas_)
{
    canvas = canvas_;

    memset(&cellmap, 0, sizeof(cellmap));

    int _add_xy[8][2] = {
        {-1, -1}, { 0, -1}, { 1, -1}, {-1,  0},
        { 1,  0}, {-1,  1}, { 0,  1}, { 1,  1}
    };

    uint32_t col[] = {
        0xff000000,
        0xff111111,
        0xff222222,
        0xff333333,
        0xff444444,
        0xff555555,
        0xff666666,
        0xffffffff
    };

    memcpy(&a_colors, &col, sizeof(col));
    memcpy(&add_xy, &_add_xy, sizeof(_add_xy));

    colors[0] = a_colors[0];
    colors[1] = a_colors[7];

    srand(time(NULL));
    cur_gen = 1;
    last_gen = 0;
    animate = true;

    initCellMap();
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
    for(int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (rand() % 8 == 0)
            {
                cellmap[x][y].cell[last_gen] = LIVE;
                cellmap[x][y].anim = MAX_COLORS;
                cellpic[y * width + x] = colors[7];
            }
            else cellpic[y * width + x] = colors[0];
        }
    }

    canvas->updateTexture(cellpic[0], 0, 0, width, height);
}

void CellMap::checkCell(const int &x, const int &y, const int &n_cells)
{
    if(cellmap[x][y].cell[last_gen] == LIVE)
    {
        if (n_cells < 2 || n_cells > 3) cellmap[x][y].cell[cur_gen] = DEAD;
            else cellmap[x][y].cell[cur_gen] = LIVE;
    }
    else if (n_cells == 3)
    {
        cellmap[x][y].cell[cur_gen] = LIVE;
        cellmap[x][y].anim = MAX_COLORS - 1;
    }
    else cellmap[x][y].cell[cur_gen] = DEAD;
}

void CellMap::countCenterCells(const int &x, const int &y)
{
    int cells = 0;
    for(int i = 0; i < 8; i++)
        cells += cellmap[x + add_xy[i][0]][y + add_xy[i][1]].cell[last_gen];

    checkCell(x, y, cells);

    if (animate)
    {
        if (cellmap[x][y].cell[cur_gen] == DEAD) (cellmap[x][y].anim) ? (--cellmap[x][y].anim) : 0;
        cellpic[y * width + x] = a_colors[cellmap[x][y].anim];
    }
    else cellpic[y * width + x] = colors[cellmap[x][y].cell[cur_gen]];
}

void CellMap::countEdgeCells(const int &x, const int &y)
{
    int cells = 0;
    int xx, yy;

    for(int i = 0; i < 8; i++)
    {
        xx = x + add_xy[i][0];
        yy = y + add_xy[i][1];

        if (xx == width) xx = 0; else if (xx < 0) xx = width - 1;
        if (yy == height) yy = 0; else if (yy < 0) yy = height - 1;

        cells += cellmap[xx][yy].cell[last_gen];
    }

    checkCell(x, y, cells);

    if (animate)
    {
        if (cellmap[x][y].cell[cur_gen] == DEAD) (cellmap[x][y].anim) ? (--cellmap[x][y].anim) : 0;
        cellpic[y * width + x] = a_colors[cellmap[x][y].anim];
    }
    else cellpic[y * width + x] = colors[cellmap[x][y].cell[cur_gen]];
}

void CellMap::animateCells()
{
    int x, y, xx, yy, i;
    int cells;
    last_gen = cur_gen ^ 1;

    for(y = 1; y < height - 1; y++)
        for(x = 1; x < width - 1; x++) countCenterCells(x, y);

    for (x = 0; x < width; x++)
    {
        countEdgeCells(x, 0);
        countEdgeCells(x, height - 1);
    }

    for (y = 0; y < height; y++)
    {
        countEdgeCells(0, y);
        countEdgeCells(width - 1, y);
    }

    cur_gen ^= 1;
    canvas->updateTexture(cellpic[0], 0, 0, width, height);
}

void CellMap::draw()
{
    float x_size =  (float)width / (float)TEXTURE_ATLAS_WIDTH;
    float y_size =  (float)height / (float)TEXTURE_ATLAS_HEIGHT;

    glTexCoord2f(0.0, 0.0);
    glVertex2f(-0.9,  0.9);

    glTexCoord2f(x_size, 0.0);
    glVertex2f( 0.9,  0.9);

    glTexCoord2f(x_size, y_size);
    glVertex2f( 0.9, -0.6);

    glTexCoord2f(0.0, y_size);
    glVertex2f(-0.9, -0.6);
}
