#ifndef LABEL_H
#define LABEL_H

#include <stdio.h>
#include <string>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <cstring>

#include "canvasobject.h"
#include "canvas.h"

using namespace std;

class Label : public CanvasObject
{
    public:
        Label(Canvas *canvas_);
        virtual ~Label();

        void setText(string _text) {text = _text;}

    private:
        Canvas  *canvas;
        string   text;
};

#endif // LABEL_H
