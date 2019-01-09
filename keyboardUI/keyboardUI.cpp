//#define cimg_use_jpeg CIMG_USE_JPEG
#include "keyboardUI.h"

using namespace cimg_library;

const int height = 320, width = 1201;

const int limit = 1000000000;

const unsigned char red[] = { 255, 0, 0 }, green[] = { 0, 255, 0 }, blue[] = { 0, 0, 255 };

const unsigned char black[] = {0, 0, 0}, white[] = {255, 255, 255}, bgc[] = {51, 51, 51};

const char letters[3][11] = {"qwertyuiop", "asdfghjkl","zxcvbnm"};

const char keyname[5][15][10] = {{"Esc", "`", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "Backspace"},
                        {"Tab", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]", "\\", "Del"},
                        {"CapsLock", "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "'", "Enter"},
                        {"Shift", "z", "x", "c", "v", "b", "n", "m", ",", ".", "/", "Up", "Shift"},
                        {"Fn", "Ctrl", "Win", "Alt", "Space", "Alt", "Ctrl", "Left", "Down", "Right", "??????"}};

const int len[3] = {10, 9, 7};
const int rad[3] = {10, 20, 10};

const int display_height = 60;

const int lrMargin = 60;

keyboard::keyboard(int trajectory_len):img("keyboard.bmp"), visu(width + lrMargin * 2, display_height+height+60, 1, 3, 0), mouse("mouse.bmp"),
    mouseMask("mask.bmp"), disp(visu, "Keyboard"){
    visu.fill(51).draw_image(0 + lrMargin, 0 + display_height, 0, 0, img).display(disp);
    if (trajectory_len < 10) trajectory_point_num = 10;
    else if (trajectory_len > 400) trajectory_point_num = 400;
    else trajectory_point_num = trajectory_len;
    head = tail = 0;
    gesture = 0;
    selectingWords = false;

    //mode = NormalWord;
    mode = SingleKey;
    printf("%d\n", mode);

    words = new char*[MAX_WORD_NUM];
    for (int i = 0; i < MAX_WORD_NUM; i++) words[i] = new char[MAX_WORD_LEN];

    isShift = isCapsLock = false;
    //initPos();
}

keyboard::~keyboard() {
    printf("Destroying keyboard object");
    for (int i = 0; i < MAX_WORD_NUM; i++) delete [] words[i];
    delete words;
}

/*void keyboard::initPos() {
    for (char ch = 'a'; ch <= 'z'; ch++) {
        bool flag = false;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < len[i]; j++) if (letters[i][j] == ch) {
                posx[ch] = i; posy[ch] = j; flag = true; break;
            }
            if (flag) break;
        }
    }
}*/

bool keyboard::bounded(int x, int y) {
    return ((x >= 0 && x < width) && (y >= 0 && y < height));
}

int keyboard::highlight(int y0, int x0) {
    int x, y;

    if (y0 == 0) x = x0 * 77 + 115;
    else if (y0 == 1) x = x0 * 77 + 154;
    else if (y0 == 2) x = x0 * 77 + 193;
    else x = 0;
    x += lrMargin;
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
            if (i >= 0 && i < width + lrMargin * 2 && j >= 0 && j < display_height+height+60) {
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
    pthread_mutex_lock(&gestureLock);
    pthread_mutex_lock(&trajLock);
    if (gesture < 0) {
        selectingWords = false;
        lastGesture = -1;
        return 0;
    }
    if (gesture > 2) {
        printf("Error! Gesture number out of range.\n");
        return -1;
    }

    clear_trajectory();

    lastGesture = this->gesture;
    this->gesture = gesture;

    if (gesture == 2) {
        if (mode == NormalWord && selectingWords) {
            printf("Selecting words!\n");
            int thatTime = head;
            int x0 = px[thatTime], y0 = py[thatTime];
            printf("%d %d\n", x0, y0);
            for (int i = 0; i < wordnum; i++) {
                printf("%d %d %d %d\n", buttons[i].x, buttons[i].y, buttons[i].w, buttons[i].h);
                if (labs(x0 - buttons[i].x) <= buttons[i].w && labs(y0 - buttons[i].y) <= buttons[i].h) {
                    sendword(buttons[i].wd);
                    break;
                }
            }
            selectingWords = false;
        }
        else if (mode == SingleKey && lastGesture != -1) {
            int thatTime = head;
            int x0 = px[thatTime] - lrMargin, y0 = py[thatTime] - display_height;
            printf("%d %d\n", x0, y0);
            getKeyPos(x0, y0);
            pressKey(x0, y0);
        }
    }

    pthread_mutex_unlock(&trajLock);
    pthread_mutex_unlock(&gestureLock);
    return 0;
}

int keyboard::clear_trajectory() {
    head = tail;
	return 0;
}

int keyboard::setPosXY(int x, int y) {
    //printf("xy: %d %d\n", x, y);
    //if (tail == trajectory_point_num - 1 || tail + 1 == head) head = (head + 1) % trajectory_point_num;
    pthread_mutex_lock(&gestureLock);
    pthread_mutex_lock(&trajLock);
    if (!bounded(x, y)) {
        printf("Error! Position index out of range.\n");
        return -1;
    }

    if (getnext(tail) == head) head = getnext(head);
    px[tail] = x + lrMargin; py[tail] = y + display_height;
    pt[tail] = curTime = clock();
    //printf("time: %d\n", pt[tail]);    ///consider adding time flags here?
    tail = getnext(tail);
    draw(x, y);

    pthread_mutex_unlock(&trajLock);
    pthread_mutex_unlock(&gestureLock);

    return 0;
}

int keyboard::draw(int x, int y) {
    visu.fill(51).draw_image(0 + lrMargin, 0 + display_height, 0, 0, img);
    if (gesture == 0) displayState("Current mode is: Typing");
    else if (gesture == 1) displayState("Current mode is: Selecting");
    else if (gesture == 2) displayState("Current mode is: Waiting");
    if (mode == NormalWord) {
        if (selectingWords) {
            for (int i = 0; i < wordnum; i++) paint(buttons[i]);
            getButton(x, y);
        }
        else getPos(x, y);
    }
    else if (mode == SingleKey) {
        getKey(x, y);
    }
    drawMouse(x, y, gesture);
    //visu.draw_circle(x, y, rad[gesture], blue);
    if (gesture == 0) draw_trajectory();

    visu.display(disp);
	return 0;
}

void keyboard::getKeyPos(int &x, int &y) {
    if (y < 63) y = 0;
    else y = (y - 1) / 62;
    if (y > 4) y = 4;
    ///0 - 4

    switch (y) {
    case 0:
        x = x / 77;
        if (x >= 15) x = 14;
        ///0 - 14
        break;
    case 1:
        if (x < 115) x = 0;
        else x = (x - 38) / 77;
        if (x >= 15) x = 14;
        ///0 - 14
        break;
    case 2:
        if (x < 154) x = 0;
        else x = (x - 77) / 77;
        if (x >= 12) x = 12;
        ///0 - 12
        break;
    case 3:
        if (x < 193) x = 0;
        else x = (x - 116) / 77;
        if (x >= 12) x = 12;
        ///0 - 12
        break;
    case 4:
        if (x < 308) x = x / 77;            ///0 - 3
        else if (x >= 732) x = (x - 347) / 77;
        else x = 4;

        if (x >= 10) x = 10;

        ///0 - 10
        break;
    default:
        printf("Error!\n"); break;
    }
}

void keyboard::getKey(int x, int y) {
    getKeyPos(x, y);
    hoverKey(x, y);
}

void keyboard::highlightKey(int x, int y) {        ///all written of magic numbers
    switch (y) {
    case 0:
        if (x != 14) antiColor(x * 77 + lrMargin, 0 + display_height, (x + 1) * 77 + lrMargin - 1, 62 + display_height);
        else antiColor(14 * 77 + lrMargin, 0 + display_height, width + lrMargin, 62 + display_height);
        break;
    case 1:
        if (x == 14) antiColor(14 * 77 + 38 + lrMargin, 63 + display_height, width + lrMargin, 124 + display_height);
        else if (x == 0) antiColor(lrMargin, 63 + display_height, 114 + lrMargin, 124 + display_height);
        else antiColor(lrMargin + x * 77 + 38, 63 + display_height, lrMargin + (x + 1) * 77 + 37, 124 + display_height);
        break;
    case 2:
        if (x == 12) antiColor(12 * 77 + 77 + lrMargin, 125 + display_height, width + lrMargin, 186 + display_height);
        else if (x == 0) antiColor(lrMargin, 125 + display_height, 153 + lrMargin, 186 + display_height);
        else antiColor(lrMargin + x * 77 + 77, 125 + display_height, lrMargin + (x + 1) * 77 + 76, 186 + display_height);
        break;
    case 3:
        if (x == 12) antiColor(12 * 77 + 116 + lrMargin, 187 + display_height, width + lrMargin, 248 + display_height);
        else if (x == 0) antiColor(lrMargin, 187 + display_height, 192 + lrMargin, 248 + display_height);
        else antiColor(lrMargin + x * 77 + 116, 187 + display_height, lrMargin + (x + 1) * 77 + 115, 248 + display_height);
        break;
    case 4:
        if (x == 10) antiColor(10 * 77 + 347 + lrMargin, 249 + display_height, width + lrMargin, 310 + display_height);
        else if (x == 4) antiColor(308 + lrMargin, 249 + display_height, 731 + lrMargin, 310 + display_height);
        else if (x < 4) antiColor(lrMargin + x * 77, 249 + display_height, lrMargin + (x + 1) * 77 - 1, 310 + display_height);
        else antiColor(lrMargin + x * 77 + 347, 249 + display_height, lrMargin + (x + 1) * 77 + 346, 310 + display_height);
        break;
    default:
        printf("Error!\n");
        break;
    }
}

void keyboard::hoverKey(int x, int y) {
    //printf(keyname[y][x]);
    char displayStr[30] = "Current Key is: ";
    displayKey(strcat(displayStr, keyname[y][x]));
    highlightKey(x, y);
}

void keyboard::pressKey(int x, int y) {
    printf("Keypressed: %s\n", keyname[y][x]);
    sendword(keyname[y][x]);

}

int keyboard::drawMouse(int x, int y, int gesture) {
    y += display_height;
    x += lrMargin;
    if (gesture == 0) visu.draw_circle(x, y, rad[gesture], red);
    else if (gesture == 2) visu.draw_circle(x, y, rad[gesture], blue);
    else {
        //visu.draw_image(x-20, y-25, 0, 0, mouse);
        unsigned char color[3];
        for (int i = 0; i < 40; i++)
        for (int j = 0; j < 50; j++)
            if (mouseMask(i, j, 0, 0) != 255) {
                color[0] = mouse(i, j, 0, 0);
                color[1] = mouse(i, j, 0, 1);
                color[2] = mouse(i, j, 0, 2);
                visu.draw_point(i + x - 15, j + y - 3, 0, color);
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
            visu.draw_line(px[i], py[i], px[j], py[j], red);
            visu.draw_line(px[i]+1, py[i], px[j]+1, py[j], red);
            visu.draw_line(px[i]-1, py[i], px[j]-1, py[j], red);
            visu.draw_line(px[i], py[i]+1, px[j], py[j]+1, red);
            visu.draw_line(px[i], py[i]-1, px[j], py[j]-1, red);
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
    visu.draw_text(230, 335 + display_height, str, white, bgc, 1, 24);
	return 0;
}


int keyboard::displayState(const char *str) {
    visu.draw_text(730, 335 + display_height, str, white, bgc, 1, 24);
	return 0;
}

int keyboard::setwords(char **wordlist, int wordnum, bool isFinal) {
    pthread_mutex_lock(&trajLock);
    if (wordnum > 5) wordnum = 5;
    if (wordnum < 0) wordnum = 0;

    for (int i = 0; i < wordnum; i++) strcpy(words[i], wordlist[i]);
    if (isFinal) {
        selectingWords = true;
        this->wordnum = wordnum;
        createButtons();
    }
    else {
        drawMiddleResults();
    }
    pthread_mutex_unlock(&trajLock);
}

int keyboard::drawMiddleResults() {
    printf("Drawing middle results!\n");
    for (int i = 0; i < wordnum; i++) printf("%s\n", words[i]);
}

int keyboard::createButtons() {
    //printf("Creating Buttons!\n");
    //for (int i = 0; i < wordnum; i++) printf("%s\n", words[i]);

    for (int i = 0; i < wordnum; i++) buttons[i].setWord(words[i]);

    int x0 = px[tail], y0 = py[tail];

    buttons[0].setPos(x0, y0);

    for (int i = 1; i < wordnum; i++) {
        switch (i) {
        case 1:
            buttons[i].setPos(x0 - buttons[0].w - 40 - buttons[1].w, y0); break;
        case 2:
            buttons[i].setPos(x0, y0 - buttons[0].h - 40 - buttons[2].h); break;
        case 3:
            buttons[i].setPos(x0 + buttons[0].w + 40 + buttons[3].w, y0); break;
        case 4:
            buttons[i].setPos(x0, y0 + buttons[0].h + 40 + buttons[4].h); break;
        default: break;
        }
    }
}

void Button::setWord(char *word) {
    int margin = 10;
    wd = word;
    CImg<unsigned char> wordi;
    wordi.draw_text(0, 0, word, black, white, 1, 50);
    option.resize(wordi.width() + margin * 2, wordi.height() + margin * 2);
    fw = option.width();
    fh = option.height();
    w = fw >> 1;
    h = fh >> 1;
    option.draw_rectangle(0, 0, fw, fh, red).draw_text(margin, margin, word, black, red, 1, 50);
}

void Button::setPos(int x, int y) {
    this->x = x; this->y = y;
}

void keyboard::paint(Button &button) {
    visu.draw_image(button.x - button.w, button.y - button.h, 0, 0, button.option);
}

void keyboard::getButton(int x, int y) {
    x += lrMargin; y += display_height;
    for (int i = 0; i < wordnum; i++) {
        if (labs(x - buttons[i].x) <= buttons[i].w && labs(y - buttons[i].y) <= buttons[i].h) {
            antiColor(buttons[i].x - buttons[i].w, buttons[i].y - buttons[i].h,
                       buttons[i].x - buttons[i].w + buttons[i].fw, buttons[i].y - buttons[i].h + buttons[i].fh);
            break;
        }
    }
}

void keyboard::sendword(const char *word) {
    printf("[word]: %s\n", word);
    output.setText(word);
}
