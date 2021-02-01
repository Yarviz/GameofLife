#include "cellmap.h"

CellMap::CellMap(Canvas *canvas_)
{
    canvas = canvas_;

    memset(&cellmap, 0, sizeof(cellmap));

    int _add_xy[8][2] = {
        {-1, -1}, { 0, -1}, { 1, -1}, {-1,  0},
        { 1,  0}, {-1,  1}, { 0,  1}, { 1,  1}
    };

    memcpy(&add_xy, &_add_xy, sizeof(_add_xy));

    setCellColor(255, 255, 255);
    initColorWheel();

    srand(time(NULL));
    cur_gen = 1;
    last_gen = 0;
    animate = 1;

    initCellMap();
}

CellMap::~CellMap()
{
    //dtor
}

void CellMap::setCellColor(uint8_t r, uint8_t g, uint8_t b)
{
    a_colors[0] = 0xff000000;
    a_colors[MAX_COLORS - 1] = 0xff000000 | (b << 16) | (g << 8) | r;

    float xp = 0.3;

    for (int i = MAX_COLORS - 2; i > 0; i--)
    {
        a_colors[i] = 0xff000000;
        a_colors[i] |= (uint32_t)((float)b * xp) << 16;
        a_colors[i] |= (uint32_t)((float)g * xp) << 8;
        a_colors[i] |= (uint32_t)((float)r * xp);

        xp -= 0.3 / (MAX_COLORS);
    }

    colors[0] = a_colors[0];
    colors[1] = a_colors[7];
}

void CellMap::setColorWheelColor(int i)
{
    if (i > COLOR_WHEEL_SIZE) return;

    setCellColor(color_wheel[i].r, color_wheel[i].g, color_wheel[i].b);
}

void CellMap::initColorWheel()
{
    float exp = (256.0 * 7 + 5) / (float)COLOR_WHEEL_SIZE;

    float rgb_add[7][3] = {{ 0, exp,   0}, {-exp,  0,  0}, {  0,  0, exp},
                           { 0,-exp,   0}, { exp,  0,  0}, {  0,  0,-exp},
                           { 0, exp, exp}};

    float rbg_col[3] = {255, 0, 0};
    int i, i2, cnt = 0;
    bool next = false;

    for (i = 0; i < COLOR_WHEEL_SIZE; i++)
    {
        color_wheel[i].r = (int)rbg_col[0];
        color_wheel[i].g = (int)rbg_col[1];
        color_wheel[i].b = (int)rbg_col[2];

        for (i2 = 0; i2 < 3; i2++)
        {
            rbg_col[i2] += rgb_add[cnt][i2];

            if (rbg_col[i2] < 0)
            {
                rbg_col[i2] = 0;
                next = true;
            }
            else if (rbg_col[i2] > 255)
            {
                rbg_col[i2] = 255;
                next = true;
            }
        }

        if (next)
        {
            next = false;
            ++cnt;
        }
    }
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

    canvas->updateTexture(&cellpic[0], 0, 0, width, height);
}

bool CellMap::lookMouseInside(const int &x, const int &y)
{
    float xf = GL_MX(x);
    float yf = GL_MY(y);

    if (xf < X_START || xf > X_END || yf > Y_START || yf < Y_END) return false;

    cmx = (xf + -X_START) / (X_SIZE / width);
    cmy = -(yf - Y_START) / (Y_SIZE / height);

    return true;
}

void CellMap::mouseClick(const int &x, const int &y, const int button)
{
    if (!lookMouseInside(x, y)) return;

    if (button == 0 && cellmap[cmx][cmy].cell[cur_gen] == DEAD)
    {
        cellmap[cmx][cmy].cell[last_gen] = LIVE;
        cellmap[cmx][cmy].anim = MAX_COLORS - 1;
        cellpic[cmy * width + cmx] = colors[1];
    }
    else if (button == 1 && cellmap[cmx][cmy].cell[cur_gen] == LIVE)
    {
        cellmap[cmx][cmy].cell[last_gen] = DEAD;
        cellmap[cmx][cmy].anim = MAX_COLORS - 2;
        cellpic[cmy * width + cmx] = a_colors[MAX_COLORS - 2];
    }

    canvas->updateTexture(&cellpic[0], 0, 0, width, height);
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

    if (cellmap[x][y].cell[cur_gen] == DEAD) (cellmap[x][y].anim) ? (--cellmap[x][y].anim) : 0;
    if (animate)
    {
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
    last_gen ^= 1;

    canvas->updateTexture(&cellpic[0], 0, 0, width, height);
}

void CellMap::draw()
{
    float x_size =  (float)width / (float)TEXTURE_ATLAS_WIDTH;
    float y_size =  (float)height / (float)TEXTURE_ATLAS_HEIGHT;

    glTexCoord2f(atlas_x, atlas_y);
    glVertex2f(X_START, Y_START);

    glTexCoord2f(atlas_x + x_size, atlas_y);
    glVertex2f(X_END, Y_START);

    glTexCoord2f(atlas_x + x_size, atlas_y + y_size);
    glVertex2f(X_END, Y_END);

    glTexCoord2f(atlas_x, atlas_y + y_size);
    glVertex2f(X_START, Y_END);
}
