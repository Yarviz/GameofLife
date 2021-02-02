#include "cellmap.h"

CellMap::CellMap(Canvas *canvas_)
{
    canvas = canvas_;

    memset(&cellmap, 0, sizeof(cellmap));   // Initialize cell map to all zeros

    // Set x/y adders to calculate cell neighbor positions

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
    update = false;

    clearCellMap(true);
}

CellMap::~CellMap()
{
    //dtor
}

void CellMap::setCellColor(uint8_t r, uint8_t g, uint8_t b)
{
    // Set cell main and shadow colors. Color is stored in 32-bit unsigned integer
    // ABGR-order (8-bit each channel) because when updating texture data is readed
    // from LSB first (0x000000ff).

    a_colors[0] = 0xff000000;
    a_colors[MAX_COLORS - 1] = 0xff000000 | (b << 16) | (g << 8) | r;

    float xp = 0.3;

    // Set shadow colors. Every color channel is little bit dimmer than on previous color.

    for (int i = MAX_COLORS - 2; i > 0; i--)
    {
        a_colors[i] = 0xff000000;
        a_colors[i] |= (uint32_t)((float)b * xp) << 16;
        a_colors[i] |= (uint32_t)((float)g * xp) << 8;
        a_colors[i] |= (uint32_t)((float)r * xp);

        xp -= 0.3 / (MAX_COLORS);
    }

    // Set main colors (used when shadows are off)

    colors[0] = a_colors[0];
    colors[1] = a_colors[MAX_COLORS - 1];

    update = true;
}

void CellMap::setColorWheelColor(int i, bool stopped)
{
    // Get cell color from color wheel array

    if (i > COLOR_WHEEL_SIZE) return;

    setCellColor(color_wheel[i].r, color_wheel[i].g, color_wheel[i].b);
    if (stopped) updateCellMapColors();
}

void CellMap::initColorWheel()
{
    // Initialize color wheel to run trough all colors. There are seven steps:
    // red -> yellow -> green -> blue-green -> blue -> purple -> red-purple -> white

    // Set each step calculated from color wheel size

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
    // Set cell map current size

    width = _width;
    height = _height;

    updateCellMapColors();
}

void CellMap::toggleAnimate()
{
    // Set cell shadows on/off

    animate ^= 1;

    updateCellMapColors();
}

void CellMap::updateCellMapColors()
{
    // Update current cell colors on cell map image

    for(int y = 0; y < width; y++)
    {
        for (int x = 0; x < height; x++)
        {
            animate ? cellpic[y * width + x] = a_colors[cellmap[x][y].anim] :
                cellpic[y * width + x] = colors[cellmap[x][y].cell[last_gen]];
        }
    }

    update = true;
}

void CellMap::clearCellMap(bool all)
{
    // Clear cell map. If all = true set all cells dead, else only clear shadows

    for(int y = 0; y < CELL_MAP_HEIGHT; y++)
    {
        for (int x = 0; x < CELL_MAP_WIDTH; x++)
        {
            if (all)
            {
                cellmap[x][y].cell[cur_gen] = DEAD;
                cellmap[x][y].cell[last_gen] = DEAD;
            }
            if (cellmap[x][y].cell[cur_gen] == DEAD)
            {
                cellmap[x][y].anim = 0;
                cellpic[y * width + x] = colors[0];
            }
        }
    }

    update = true;
}

void CellMap::randomCellMap(int rnd)
{
    // Set random cells to live based on rnd-value

    for(int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (rand() % rnd == 0)
            {
                cellmap[x][y].cell[last_gen] = LIVE;
                cellmap[x][y].anim = MAX_COLORS - 1;
                cellpic[y * width + x] = colors[1];
            }
        }
    }

    update = true;
}

bool CellMap::lookMouseInside(const int &mx, const int &my)
{
    // Change mouse coordinates to OpenGL coordinates

    float mxf = GL_MX(mx);
    float myf = GL_MY(my);

    // If outside cell map bounds, return false

    if (mxf < X_START || mxf > X_END || myf > Y_START || myf < Y_END) return false;

    // Transfer OpenGL coordinates to match selected cell

    cmx = (mxf + -X_START) / (X_SIZE / width);
    cmy = -(myf - Y_START) / (Y_SIZE / height);

    return true;
}

void CellMap::mouseClick(const int &mx, const int &my, const int button)
{
    if (!lookMouseInside(mx, my)) return;

    if (button == 0)    // Left mouse button set cell to live
    {
        cellmap[cmx][cmy].cell[last_gen] = LIVE;
        cellmap[cmx][cmy].anim = MAX_COLORS - 1;
        cellpic[cmy * width + cmx] = colors[1];
    }
    else if (button == 1)   // Right mouse button set cell to dead
    {
        cellmap[cmx][cmy].cell[last_gen] = DEAD;
        cellmap[cmx][cmy].anim = MAX_COLORS - 2;
        animate ? cellpic[cmy * width + cmx] = a_colors[MAX_COLORS - 2] : cellpic[cmy * width + cmx] = colors[0];
    }

    update = true;
}

void CellMap::checkCell(const int &x, const int &y, const int &n_cells)
{
    // Set current cell state to LIVE or DEAD based on Conway's Game of Life - rules.
    // n_cells is number of live neighbors from previous generation.

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
    // Count cells living neighbors from center of map.
    // No need to check every time if position is outside bounds.

    int cells = 0;
    for(int i = 0; i < 8; i++)
        cells += cellmap[x + add_xy[i][0]][y + add_xy[i][1]].cell[last_gen];

    checkCell(x, y, cells); // Set current cell LIVE or DEAD based on the rules

    // If cell is dead and cell shadow value > 0 decrease it

    if (cellmap[x][y].cell[cur_gen] == DEAD) (cellmap[x][y].anim) ? (--cellmap[x][y].anim) : 0;

    // If animations on, set cell color to respond shadow color, else
    // set color to respond cells current state (LIVE or DEAD)

    if (animate)
    {
        cellpic[y * width + x] = a_colors[cellmap[x][y].anim];
    }
    else cellpic[y * width + x] = colors[cellmap[x][y].cell[cur_gen]];
}

void CellMap::countEdgeCells(const int &x, const int &y)
{
    // Count cells living neighbors from edges of map. Every time need
    // to check if position is outside bounds. If so, then wrap position
    // around to other edge of cell map.

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

    checkCell(x, y, cells); // Set current cell LIVE or DEAD based on the rules

    // If cell is dead and cell shadow value > 0 decrease it

    if (cellmap[x][y].cell[cur_gen] == DEAD) (cellmap[x][y].anim) ? (--cellmap[x][y].anim) : 0;

    // If animations on, set cell color to respond shadow color, else
    // set color to respond cells current state (LIVE or DEAD)

    if (animate)
    {
        cellpic[y * width + x] = a_colors[cellmap[x][y].anim];
    }
    else cellpic[y * width + x] = colors[cellmap[x][y].cell[cur_gen]];
}

void CellMap::animateCells()
{
    int x, y, xx, yy, i;
    int cells;

    // First animate cells from center

    for(y = 1; y < height - 1; y++)
        for(x = 1; x < width - 1; x++) countCenterCells(x, y);

    // Then edge cells

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

    // Swap generations

    cur_gen ^= 1;
    last_gen ^= 1;

    update = true;
}

void CellMap::updateTexture()
{
    if (!update) return;    // Return if no need to update texture atlas

    // Update texture atlas from current bounds and reset update flag.

    canvas->updateTexture(&cellpic[0], 0, 0, width, height);
    update = false;
}

void CellMap::draw()
{
    // Get current cell map size on texture atlas

    float x_size =  (float)width / (float)TEXTURE_ATLAS_WIDTH;
    float y_size =  (float)height / (float)TEXTURE_ATLAS_HEIGHT;

    // Draw cell map image on canvas

    glTexCoord2f(atlas_x, atlas_y);
    glVertex2f(X_START, Y_START);

    glTexCoord2f(atlas_x + x_size, atlas_y);
    glVertex2f(X_END, Y_START);

    glTexCoord2f(atlas_x + x_size, atlas_y + y_size);
    glVertex2f(X_END, Y_END);

    glTexCoord2f(atlas_x, atlas_y + y_size);
    glVertex2f(X_START, Y_END);
}
