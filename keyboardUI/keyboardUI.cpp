//#define cimg_use_jpeg CIMG_USE_JPEG
#include "keyboardUI.h"

using namespace cimg_library;

const int height = 320, width = 1201;

const int limit = 2000;

const unsigned char red[] = { 255, 0, 0 }, green[] = { 0, 255, 0 }, blue[] = { 0, 0, 255 };

const unsigned char black[] = {0, 0, 0}, white[] = {255, 255, 255};

const char letters[3][11] = {"qwertyuiop", "asdfghjkl","zxcvbnm"};
const int len[3] = {10, 9, 7};
const int rad[3] = {10, 20, 10};

const int display_height = 60;

keyboard::keyboard(int trajectory_len):img("keyboard.bmp"), visu(width, display_height+height+60, 1, 3, 0), mouse("mouse.bmp"),
    mouseMask("mask.bmp"), disp(visu, "Keyboard"){
    visu.fill(255).draw_image(0, 0 + display_height, 0, 0, img).display(disp);
    if (trajectory_len < 10) trajectory_point_num = 10;
    else if (trajectory_len > 200) trajectory_point_num = 200;
    else trajectory_point_num = trajectory_len;
    head = tail = 0;
    gesture = 0;
    initPos();
}

void keyboard::initPos() {
    for (char ch = 'a'; ch <= 'z'; ch++) {
        bool flag = false;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < len[i]; j++) if (letters[i][j] == ch) {
                posx[ch] = i; posy[ch] = j; flag = true; break;
            }
            if (flag) break;
        }
    }
}

bool keyboard::bounded(int x, int y) {
    return ((x >= 0 && x < width) && (y >= 0 && y < height));
}

int keyboard::highlight(int y0, int x0) {
    int x, y;

    if (y0 == 0) x = x0 * 77 + 115;
    else if (y0 == 1) x = x0 * 77 + 154;
    else if (y0 == 2) x = x0 * 77 + 193;
    y = y0 * 62 + 63 + display_height;
    //printf("x0: %d y0: %d\n", x, y);

    antiColor(x, y, x+77, y+62);
    //visu.draw_rectangle(x, y, x+77, y+62, red);
	return 0;
}

int keyboard::antiColor(int x0, int y0, int x1, int y1) {
    unsigned char color[3];
    for (int i = x0; i < x1; i++)
        for (int j = y0; j < y1; j++)
            if (bounded(i, j)) {
                for (int k = 0; k < 3; k++)
                    color[k] = 255 - visu(i, j, 0, k);
                visu.draw_point(i, j, 0, color);
            }
	return 0;
}

int keyboard::getPos(int x, int y) {
    int yIndex = (y - 63) / 62, xIndex;
    if (y < 63 || y >= 249) {
        /*if (y >= 63 || x < 1077) return -1;
        yIndex = -1; xIndex = 0;*/
        return -1;
    }
    else {
        switch (yIndex) {
            case 0: if (x < 115 || x >= 885) return -1;
                    xIndex = (x - 115) / 77;
                    break;
            case 1: if (x < 154 || x >= 847) return -1;
                    xIndex = (x - 154) / 77;
                    break;
            case 2: if (x < 193 || x >= 732) return -1;
                    xIndex = (x - 194) / 77;
                    break;
            default: //printf("Why? %d\n", x);
                    break;
        }
    }

    //printf("Index: %d %d\n", xIndex, yIndex);
    getName(yIndex, xIndex);
	return 0;
}

int keyboard::getName(int y, int x) {
    if (y >= 0) {
        //putchar(letters[y][x]);
        //putchar('\n');
        char message[] = "Current key is:  ";
        message[16] = letters[y][x];
        displayKey(message);
        highlight(y, x);
    }
    else {
        //puts("Backspace");
        displayKey("Current key is: Backspace");
        //antiColor(1077, 0, 1200, 62);
    }
	return 0;
}

int keyboard::setGesture(int gesture) {
    if (gesture < 0 || gesture > 2) {
        printf("Error! Gesture number out of range.\n");
        return -1;
    }
    this->gesture = gesture;
    clear_trajectory();
    return 0;
}

int keyboard::clear_trajectory() {
    head = tail;
	return 0;
}

int keyboard::setPosXY(int x, int y) {
    //printf("xy: %d %d\n", x, y);
    //if (tail == trajectory_point_num - 1 || tail + 1 == head) head = (head + 1) % trajectory_point_num;
    if (!bounded(x, y)) {
        printf("Error! Position index out of range.\n");
        return -1;
    }

    if (getnext(tail) == head) head = getnext(head);
    px[tail] = x; py[tail] = y + display_height;
    pt[tail] = curTime = clock();
    //printf("time: %d\n", pt[tail]);    ///consider adding time flags here?
    tail = getnext(tail);

    draw(x, y);

    return 0;
}

int keyboard::draw(int x, int y) {
    visu.fill(255).draw_image(0, 0 + display_height, 0, 0, img);
    if (gesture == 0) displayState("Current mode is: Typing");
    else if (gesture == 1) displayState("Current mode is: Selecting");
    else if (gesture == 2) displayState("Current mode is: Waiting");
    getPos(x, y);
    drawMouse(x, y, gesture);
    //visu.draw_circle(x, y, rad[gesture], blue);
    if (gesture == 0) draw_trajectory();

    updateWordlist();
    visu.display(disp);
	return 0;
}

int keyboard::drawMouse(int x, int y, int gesture) {
    y += display_height;
    if (gesture != 1) visu.draw_circle(x, y, rad[gesture], blue);
    else {
        //visu.draw_image(x-20, y-25, 0, 0, mouse);
        unsigned char color[3];
        for (int i = 0; i < 40; i++)
        for (int j = 0; j < 50; j++)
            if (mouseMask(i, j, 0, 0) != 255) {
                color[0] = mouse(i, j, 0, 0);
                color[1] = mouse(i, j, 0, 1);
                color[2] = mouse(i, j, 0, 2);
                visu.draw_point(i + x - 20, j + y - 25, 0, color);
            }
    }
	return 0;
}

int keyboard::draw_trajectory() {
    //printf("head: %d tail: %d\n", head, tail);
    if (gesture == 0 && getnext(head) != tail) {
        for (int i = head, j = getnext(i); j != tail; i = j, j = getnext(j)) {
            if (curTime - pt[i] >= limit) {
                head = getnext(head);
                continue;
            }
            visu.draw_line(px[i], py[i], px[j], py[j], blue);
            visu.draw_line(px[i]+1, py[i], px[j]+1, py[j], blue);
            visu.draw_line(px[i]-1, py[i], px[j]-1, py[j], blue);
            visu.draw_line(px[i], py[i]+1, px[j], py[j]+1, blue);
            visu.draw_line(px[i], py[i]-1, px[j], py[j]-1, blue);
        }
    }
	return 0;
}

int keyboard::getnext(int i) {
    //printf("traj: %d\n", trajectory_point_num);
    if (i == trajectory_point_num - 1) return 0;
    else return (i+1);
}

int keyboard::displayKey(const char *str) {
    visu.draw_text(230, 335 + display_height, str, black, white, 1, 24);
	return 0;
}


int keyboard::displayState(const char *str) {
    visu.draw_text(730, 335 + display_height, str, black, white, 1, 24);
	return 0;
}

int keyboard::feedWordlist(std::vector<std::string> wordlist) {
    std::cout << wordlist.size() << wordlist[0] << std::endl;
    return 0;
}

int keyboard::updateWordlist() {
    std::vector<std::string> words;
    words.push_back("Hello world!");
    words.push_back("PPAP!");
    feedWordlist(words);
    return 0;
}

int keyboard::displayWordlist() {
    return 0;
}
