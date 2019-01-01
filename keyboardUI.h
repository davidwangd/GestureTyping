#ifndef KEYBOARD_UI_H_
#define KEYBOARD_UI_H_

#include "CImg.h"
#include<ctime>

class keyboard{
private:
    int trajectory_point_num, head, tail;
    int gesture;
    int px[205], py[205];
    clock_t pt[205];
    clock_t curTime;
    int posx[200], posy[200];

    cimg_library::CImg<unsigned char> img;
    cimg_library::CImg<unsigned char> visu;
    cimg_library::CImg<unsigned char> mouse;
    cimg_library::CImg<unsigned char> mouseMask;
    cimg_library::CImgDisplay disp;

    void initPos();

    bool bounded(int x, int y);

    int highlight(int y0, int x0);

    int antiColor(int x0, int y0, int x1, int y1);

    int drawMouse(int x, int y, int gesture);

    int getPos(int x, int y);

    int getName(int y, int x);

    int displayKey(const char* str);

    int displayState(const char* str);

    int draw(int x, int y);

    int draw_trajectory();

    int clear_trajectory();

    int getnext(int i);

public:

    keyboard(int trajectory_len = 50);

    int setGesture(int gesture);

    int setPosXY(int x, int y);
};


#endif // KEYBOARD_UI_H_

