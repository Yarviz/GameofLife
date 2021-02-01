#include "game.h"

Game::Game(int _width, int _height)
{
    width = _width;
    height = _height;

    Fonts::loadFonts();
    init();
}

Game::~Game()
{
    delete(labelSize);
    delete(labelSpeed);
    delete(labelColor);
    delete(slideSize);
    delete(slideSpeed);
    delete(slideColor);
    delete(buttonShadows);
    delete(buttonClear);
    delete(buttonRandom);
    delete(buttonStop);
    delete(buttonStep);
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
    labelSize->setText("SIZE: 64 X 64", 0xff00ffff);
    labelSize->setXY(50, 60, 2);

    labelSpeed = new Label(canvas.get(), 80, 5);
    labelSpeed->setText("SPEED: 60 FPS", 0xff00ffff);
    labelSpeed->setXY(320, 60, 2);

    labelColor = new Label(canvas.get(), 80, 5);
    labelColor->setText("COLOR: FFFFFF", cellmap->getColor());
    labelColor->setXY(580, 60, 2);

    canvas->addChild(cellmap, CELL_MAP_WIDTH, CELL_MAP_HEIGHT);
    canvas->addChild(labelSize, 80, 5);
    canvas->addChild(labelSpeed, 80, 5);
    canvas->addChild(labelColor, 80, 5);

    labelSize->uploadText();
    labelSpeed->uploadText();
    labelColor->uploadText();

    slideSize = new Slide(canvas.get());
    slideSize->setXY(56, 32, 2);
    slideSize->setSlidePos(36, 212, 3, CELL_MAP_HEIGHT);

    slideSpeed = new Slide(canvas.get());
    slideSpeed->setXY(476, 32, 2);
    slideSpeed->setSlidePos(300, 476, 1, 60);

    slideColor = new Slide(canvas.get());
    slideColor->setXY(740, 32, 2);
    slideColor->setSlidePos(564, 740, 0, 175);

    canvas->addChild(slideSize, SLIDE_WIDTH, SLIDE_HEIGHT);
    canvas->addChild(slideSpeed, SLIDE_WIDTH, SLIDE_HEIGHT);
    canvas->addChild(slideColor, SLIDE_WIDTH, SLIDE_HEIGHT);

    slideSize->uploadImage();
    slideSpeed->uploadImage();
    slideColor->uploadImage();

    buttonShadows= new Button(canvas.get(), BUTTON_WIDTH, BUTTON_HEIGHT, "SHADOWS: ON", 0xffffff);
    buttonShadows->setXY(36, 90, 2);

    buttonClear = new Button(canvas.get(), BUTTON_WIDTH, BUTTON_HEIGHT, "CLEAR", 0xffffff);
    buttonClear->setXY(186, 90, 2);

    buttonRandom = new Button(canvas.get(), BUTTON_WIDTH, BUTTON_HEIGHT, "RANDOM", 0xffffff);
    buttonRandom->setXY(336, 90, 2);

    buttonStop = new Button(canvas.get(), BUTTON_WIDTH, BUTTON_HEIGHT, "STOP", 0xffffff);
    buttonStop->setXY(486, 90, 2);

    buttonStep = new Button(canvas.get(), BUTTON_WIDTH, BUTTON_HEIGHT, "STEP", 0xffffff);
    buttonStep->setXY(634, 90, 2);

    canvas->addChild(buttonShadows, BUTTON_WIDTH, BUTTON_HEIGHT * 2);
    canvas->addChild(buttonClear, BUTTON_WIDTH, BUTTON_HEIGHT * 2);
    canvas->addChild(buttonRandom, BUTTON_WIDTH, BUTTON_HEIGHT * 2);
    canvas->addChild(buttonStop, BUTTON_WIDTH, BUTTON_HEIGHT * 2);
    canvas->addChild(buttonStep, BUTTON_WIDTH, BUTTON_HEIGHT * 2);

    buttonShadows->uploadImage();
    buttonClear->uploadImage();
    buttonRandom->uploadImage();
    buttonStop->uploadImage();
    buttonStep->uploadImage();

    buttonStep->setState(false);

    speed = 60;
    memset(&mouse, 0, sizeof(mouse));
}

string Game::toHex(uint32_t value, int len)
{
    static const char* digit = "0123456789ABCDEF";
    string str(len, '0');
    for (size_t i = 0, j = (len - 1) * 4 ; i < len; ++i, j -= 4)
                str[i] = digit[(value >> j) & 0x0f];
    return str;
}

void Game::run()
{
    Atom wmDeleteMessage = XInternAtom(canvas->getDisplay(), "WM_DELETE_WINDOW", False);
    XSetWMProtocols(canvas->getDisplay(), *canvas->getWindow(), &wmDeleteMessage, 1);

    XEvent xevent;
    bool running = true;
    float speed_cnt = 1.0;
    float speed_dec = (float)speed / 60.0;
    int item;

    long mask = ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask;

    chrono::duration<long, ratio<1,60>> fps_tick(1);
    auto frame_timer = chrono::high_resolution_clock::now();

    while(running)
    {
        frame_timer = chrono::high_resolution_clock::now();
        item = -1;

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
                            labelSpeed->setText("SPEED: " + to_string(speed) + " FPS", 0xff00ffff);
                            labelSpeed->uploadText();
                            break;
                        case XK_a:
                            speed > 1 ? --speed : 0;
                            labelSpeed->setText("SPEED: " + to_string(speed) + " FPS", 0xff00ffff);
                            labelSpeed->uploadText();
                            break;
                        case XK_w:
                            cell_siz < CELL_MAP_WIDTH ? cell_siz += 1 : 0;
                            labelSize->setText("SIZE: " + to_string(cell_siz) + " X " + to_string(cell_siz), 0xff00ffff);
                            labelSize->uploadText();
                            cellmap->setSize(cell_siz, cell_siz);
                            break;
                        case XK_s:
                            cell_siz > 3 ? cell_siz -= 1 : 0;
                            labelSize->setText("SIZE: " + to_string(cell_siz) + " X " + to_string(cell_siz), 0xff00ffff);
                            labelSize->uploadText();
                            cellmap->setSize(cell_siz, cell_siz);
                            break;
                    }
                    break;

                case MotionNotify:
                    mouse.x = xevent.xmotion.x;
                    mouse.y = xevent.xmotion.y;

                    buttonShadows->mouseMove(mouse.x, mouse.y);
                    buttonClear->mouseMove(mouse.x, mouse.y);
                    buttonRandom->mouseMove(mouse.x, mouse.y);
                    buttonStop->mouseMove(mouse.x, mouse.y);
                    buttonStep->mouseMove(mouse.x, mouse.y);

                    item = slideSize->mouseMove(mouse.x);
                    if (item > -1)
                    {
                        cell_siz = item;

                        labelSize->setText("SIZE: " + to_string(item) + " X " + to_string(item), 0xff00ffff);
                        labelSize->uploadText();
                        cellmap->setSize(item, item);
                        break;
                    }

                    item = slideSpeed->mouseMove(mouse.x);
                    if (item > -1)
                    {
                        speed = item;

                        labelSpeed->setText("SPEED: " + to_string(speed) + " FPS", 0xff00ffff);
                        labelSpeed->uploadText();
                        break;
                    }

                    item = slideColor->mouseMove(mouse.x);
                    if (item > -1)
                    {
                        cellmap->setColorWheelColor(item);

                        labelColor->setText("COLOR: " + toHex(cellmap->getColor() & 0xffffff, 6), cellmap->getColor());
                        labelColor->uploadText();
                        break;
                    }
                    break;

                case ButtonPress:
                    xevent.xbutton.button == 1 ? mouse.button[0] = 1 : mouse.button[1] = 1;

                    slideSize->mouseClick(mouse.x, mouse.y);
                    slideSpeed->mouseClick(mouse.x, mouse.y);
                    slideColor->mouseClick(mouse.x, mouse.y);

                    buttonShadows->mouseClick(mouse.x, mouse.y);
                    buttonClear->mouseClick(mouse.x, mouse.y);
                    buttonRandom->mouseClick(mouse.x, mouse.y);
                    buttonStop->mouseClick(mouse.x, mouse.y);
                    buttonStep->mouseClick(mouse.x, mouse.y);
                    break;

                case ButtonRelease:
                    xevent.xbutton.button == 1 ? mouse.button[0] = 0 : mouse.button[1] = 0;

                    slideSize->mouseRelease();
                    slideSpeed->mouseRelease();
                    slideColor->mouseRelease();

                    if (buttonShadows->mouseRelease()) cellmap->setAnimate();
                    buttonClear->mouseRelease();
                    buttonRandom->mouseRelease();
                    buttonStop->mouseRelease();
                    buttonStep->mouseRelease();
                    break;
            }
        }

        if (item == -1)
        {
            if (mouse.button[0]) cellmap->mouseClick(mouse.x, mouse.y, 0);
                else if (mouse.button[1]) cellmap->mouseClick(mouse.x, mouse.y, 1);
        }

        speed_cnt -= speed_dec;
        if (speed_cnt <= 0)
        {
            cellmap->animateCells();
            speed_cnt = 1.0;
            speed_dec = (float)speed / 60.0;
        }

        this_thread::sleep_until(frame_timer + fps_tick);

        canvas->draw();
    }

    canvas->destroy();
}
