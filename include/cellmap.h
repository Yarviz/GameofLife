#ifndef CELLMAP_H
#define CELLMAP_H

#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <cstring>
#include <random>

#include "canvasobject.h"
#include "canvas.h"

#define CELL_MAP_WIDTH    512       // Cell map maximum dimensions
#define CELL_MAP_HEIGHT   512
#define MAX_COLORS        8         // Cell color for shadows
#define COLOR_WHEEL_SIZE  176       // Color wheel size

#define LIVE              1         // Cell defines
#define DEAD              0

#define X_START           -0.9      // Cell map position and dimension on OpenGL context
#define X_END              0.9
#define Y_START            0.6
#define Y_END             -0.9
#define X_SIZE             1.8
#define Y_SIZE             1.5

#define CELL_COLOR         0xffffff // Cell default color = white(ABGR)

// Main class for game logic
class CellMap : public CanvasObject
{
    public:
        CellMap(Canvas *canvas_);       // Set root canvas
        virtual ~CellMap();

        void draw();                                                     // Draw cell map on canvas
        void updateTexture();                                            // Update cell map on texture atlas (current dimensions)
        void setSize(int _width, int _height);                           // Set cell map current dimensions
        void randomCellMap(int rnd);                                     // Set random cells to live
        void animateCells();                                             // Animate cells
        void mouseClick(const int &mx, const int &my, const int button); // Mouse click callback
        void setCellColor(uint8_t r, uint8_t g, uint8_t b);              // Set cell color (RGB)
        void setColorWheelColor(int i, bool stopped);                    // Set cell color from color wheel
        void toggleAnimate();                                            // Toggle shadows on/off
        bool getAnimate() {return animate;}                              // Get shadows state
        void clearCellMap(bool all);                                     // Clear cell map
        uint32_t getColor() {return colors[1];}                          // Get current cell color

    private:
        void checkCell(const int &x,const int &y, const int &n_cells);   // Check cell state relative to previous generation
        void countCenterCells(const int &x,const int &y);                // Check cell map center cells
        void countEdgeCells(const int &x,const int &y);                  // Check cell map edge cells
        bool lookMouseInside(const int &mx, const int &my);              // Check if mouse inside cell map
        void initColorWheel();
        void updateCellMapColors();

        Canvas  *canvas;

        typedef struct {        // Cell info
            uint8_t anim;       // Current shadow value (0 - MAX_COLORS)
            uint8_t cell[2];    // Cell state LIVE/DEAD (current and previous generation)
        }Cell;

        typedef struct {        // RBG struct for color wheel
            uint8_t r;
            uint8_t g;
            uint8_t b;
        }RGB_Col;

        Cell     cellmap[CELL_MAP_WIDTH][CELL_MAP_HEIGHT];          // Cell map for cell states
        uint32_t cellpic[CELL_MAP_WIDTH * CELL_MAP_HEIGHT];         // Bitmap for cell map (24 bit color value ABGR)
        uint32_t a_colors[MAX_COLORS];                              // Cell shadow colors
        uint32_t colors[2];                                         // Cell main colors
        RGB_Col  color_wheel[COLOR_WHEEL_SIZE];                     // Color wheel colors
        int      add_xy[8][2];                                      // Table for counting neighbor cells
        int      width;                                             // Cell map current dimensions
        int      height;
        int      cur_gen, last_gen;                                 // Current and last cell generations
        int      cmx, cmy;                                          // Mouse clicked cell on cell map
        bool     animate;                                           // Cell shadows on/off
        bool     update;                                            // Flag for updating texture atlas
};

#endif // CELLMAP_H
