#include "game.h"

Game::Game()
{
    init();
}

Game::~Game()
{
    delete(cellmap);
}

void Game::init()
{
    canvas.reset(new Canvas(WIN_WIDTH, WIN_HEIGHT));
    canvas->setTitle("Conways Game of Life");

    cellmap = new CellMap(canvas.get());
    cellmap->setSize(16, 16);
    cellmap->initCellMap();
}

void Game::run()
{
    Atom wmDeleteMessage = XInternAtom(canvas->getDisplay(), "WM_DELETE_WINDOW", False);
    XSetWMProtocols(canvas->getDisplay(), *canvas->getWindow(), &wmDeleteMessage, 1);

    XEvent xevent;
    bool running = true;

    canvas->addChild(cellmap);

    while(running)
    {
        XNextEvent(canvas->getDisplay(), &xevent);

        switch(xevent.type)
        {
            case Expose:
                canvas->refreshContext();
                canvas->draw();
                break;

            case ClientMessage:
                if (xevent.xclient.data.l[0] == wmDeleteMessage)
                running = false;
                break;

            case KeyPress:
                running = false;
                break;
        }
    }

    canvas->destroy();
}
