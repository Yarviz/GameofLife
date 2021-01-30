#include "game.h"

Game::Game(int _width, int _height)
{
    width = _width;
    height = _height;

    init();
}

Game::~Game()
{
    delete(cellmap);
}

void Game::init()
{
    canvas.reset(new Canvas(width, height));
    canvas->setTitle("Conways Game of Life");

    cell_siz = 64;

    cellmap = new CellMap(canvas.get());
    cellmap->setSize(cell_siz, cell_siz);
    cellmap->initCellMap();

    speed = 2;
    memset(&mouse, 0, sizeof(mouse));
}

void Game::run()
{
    Atom wmDeleteMessage = XInternAtom(canvas->getDisplay(), "WM_DELETE_WINDOW", False);
    XSetWMProtocols(canvas->getDisplay(), *canvas->getWindow(), &wmDeleteMessage, 1);

    XEvent xevent;
    bool running = true;
    int speed_cnt = 0;

    long mask = ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask;

    canvas->addChild(cellmap);

    chrono::duration<long, ratio<1,60>> fps_tick(1);
    auto frame_timer = chrono::high_resolution_clock::now();

    while(running)
    {
        frame_timer = chrono::high_resolution_clock::now();

        while (XCheckWindowEvent(canvas->getDisplay(), *canvas->getWindow(), mask, &xevent))
        {
            switch(xevent.type)
            {
                case Expose:
                    canvas->refreshContext();
                    break;

                case ClientMessage:
                    if (xevent.xclient.data.l[0] == wmDeleteMessage)
                    running = false;
                    break;

                case KeyPress:
                    switch(XLookupKeysym(&xevent.xkey, 0))
                    {
                        case XK_Escape: running = false; break;
                        case XK_q: speed < 60 ? ++speed : 0; break;
                        case XK_a: speed > 1 ? --speed : 0; break;
                        case XK_w:
                            cell_siz < CELL_MAP_WIDTH ? cell_siz += 1 : 0;
                            cellmap->setSize(cell_siz, cell_siz);
                            break;
                        case XK_s:
                            cell_siz > 3 ? cell_siz -= 1 : 0;
                            cellmap->setSize(cell_siz, cell_siz);
                            break;
                    }
                    break;

                case MotionNotify:
                    mouse.x = xevent.xmotion.x;
                    mouse.y = xevent.xmotion.y;
                    cout << mouse.x << " " << mouse.y << endl;
                    break;

                case ButtonPress:
                    xevent.xbutton.button == 1 ? mouse.button[0] = 1 : mouse.button[1] = 1;
                    break;

                case ButtonRelease:
                    xevent.xbutton.button == 1 ? mouse.button[0] = 0 : mouse.button[1] = 0;
                    break;
            }
        }

        if (++speed_cnt >= speed)
        {
            cellmap->animateCells();
            speed_cnt = 0;
        }

        this_thread::sleep_until(frame_timer + fps_tick);

        canvas->draw();
    }

    canvas->destroy();
}
