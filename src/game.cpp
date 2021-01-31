#include "game.h"

Game::Game(int _width, int _height)
{
    width = _width;
    height = _height;

    init();
}

Game::~Game()
{
    delete(labelSize);
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

    labelSize = new Label(canvas.get(), 80, 5);
    labelSize->setText("SIZE: 64 X 64");
    labelSize->setXY(50, canvas->getHeight() - 150, 2);

    labelSpeed = new Label(canvas.get(), 80, 5);
    labelSpeed->setText("SPEED: 60 FPS");
    labelSpeed->setXY(50, canvas->getHeight() - 80, 2);

    canvas->addChild(cellmap, CELL_MAP_WIDTH, CELL_MAP_HEIGHT);
    canvas->addChild(labelSize, 80, 5);
    canvas->addChild(labelSpeed, 80, 5);

    labelSize->uploadText();
    labelSpeed->uploadText();

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
                        case XK_q:
                            speed < 60 ? ++speed : 0;
                            labelSpeed->setText("SPEED: " + to_string(61 - speed) + " FPS");
                            labelSpeed->uploadText();
                            break;
                        case XK_a:
                            speed > 1 ? --speed : 0;
                            labelSpeed->setText("SPEED: " + to_string(61 - speed) + " FPS");
                            labelSpeed->uploadText();
                            break;
                        case XK_w:
                            cell_siz < CELL_MAP_WIDTH ? cell_siz += 1 : 0;
                            labelSize->setText("SIZE: " + to_string(cell_siz) + " X " + to_string(cell_siz));
                            labelSize->uploadText();
                            cellmap->setSize(cell_siz, cell_siz);
                            break;
                        case XK_s:
                            cell_siz > 3 ? cell_siz -= 1 : 0;
                            labelSize->setText("SIZE: " + to_string(cell_siz) + " X " + to_string(cell_siz));
                            labelSize->uploadText();
                            cellmap->setSize(cell_siz, cell_siz);
                            break;
                    }
                    break;

                case MotionNotify:
                    mouse.x = xevent.xmotion.x;
                    mouse.y = xevent.xmotion.y;
                    break;

                case ButtonPress:
                    xevent.xbutton.button == 1 ? mouse.button[0] = 1 : mouse.button[1] = 1;
                    break;

                case ButtonRelease:
                    xevent.xbutton.button == 1 ? mouse.button[0] = 0 : mouse.button[1] = 0;
                    break;
            }
        }

        if (mouse.button[0]) cellmap->mouseClick(mouse.x, mouse.y, 0);
            else if (mouse.button[1]) cellmap->mouseClick(mouse.x, mouse.y, 1);

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
