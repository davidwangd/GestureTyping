#ifndef KEYBOARD_UI_H_
#define KEYBOARD_UI_H_

#include "CImg.h"
#include<ctime>

class keyboard{
public:
    int trajectory_point_num, head, tail;
    int gesture;
    int px[205], py[205];
    clock_t pt[205];
    clock_t curTime;
    int posx[200], posy[200];

    cimg_library::CImg<unsigned char> img;
    cimg_library::CImg<unsigned char> visu;
    cimg_library::CImgDisplay disp;

    keyboard(int trajectory_len = 50);

    void initPos();

    bool bounded(int x, int y);

    int highlight(int y0, int x0);

    int antiColor(int x0, int y0, int x1, int y1);

    int getPos(int x, int y);

    int getName(int y, int x);

    int displayKey(const char* str);

    int displayState(const char* str);

    int setGesture(int gesture);

    int setPosXY(int x, int y);

    int draw(int x, int y);

    int draw_trajectory();

    int clear_trajectory();

    int getnext(int i);
};


#endif // KEYBOARD_UI_H_

