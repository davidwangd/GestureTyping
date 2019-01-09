#ifndef KEYBOARD_UI_H_
#define KEYBOARD_UI_H_

#include "CImg.h"
#include<ctime>
#include<vector>
#include<string>
#include<iostream>
#include<pthread.h>

#include "TextDisplay.h"

#define MAX_WORD_NUM 5
#define MAX_WORD_LEN 30

class Button{
public:
    cimg_library::CImg<unsigned char> option;
    int x, y;
    int w, h;
    int fw, fh;
    char *wd;
    void setWord(char *word);
    void setPos(int x, int y);
};

class keyboard{
private:
    int trajectory_point_num, head, tail;
    int gesture;
    int px[405], py[405];
    clock_t pt[405];
    clock_t curTime;
    int posx[400], posy[400];

    enum inputMode{
        NormalWord, SingleKey
    }mode;

    char** words;
    int wordnum;
    bool selectingWords;

    bool isShift, isCapsLock;

    TextDisplay output;

    Button buttons[MAX_WORD_NUM];

    pthread_mutex_t trajLock, gestureLock;

    cimg_library::CImg<unsigned char> img;
    cimg_library::CImg<unsigned char> visu;
    cimg_library::CImg<unsigned char> mouse;
    cimg_library::CImg<unsigned char> mouseMask;
    cimg_library::CImgDisplay disp;

    //void initPos();

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

    int drawMiddleResults();

    int createButtons();

    void paint(Button &button);

    void getButton(int x, int y);

    void sendword(char *word);

    void getKeyPos(int &x, int &y);

    void getKey(int x, int y);

    void hoverKey(int x, int y);

    void highlightKey(int x, int y);

    void pressKey(int x, int y);

public:

    keyboard(int trajectory_len = 20);

    int setGesture(int gesture);

    int setPosXY(int x, int y);

    int setwords(char **wordlist, int wordnum, bool isFinal);

    ~keyboard();
};


#endif // KEYBOARD_UI_H_

