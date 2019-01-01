#include "keyboardUI.h"

using namespace cimg_library;

CImg<unsigned char> image("keyboard.bmp");
keyboard board;

char *wordlist[5] = {"1", "22", "333", "4444", "55555"};

char *wordlist2[4] = {"666", "777777777777", "888888888", "9999"};

int main() {
    CImgDisplay main_disp(image, "Click a point");
    int gesture;
    int cnt = 0;
    while (!main_disp.is_closed()) {
        main_disp.wait();
        if (main_disp.button() && main_disp.mouse_x() >= 0 && main_disp.mouse_y()>=0) {
            cnt++;
            int x = main_disp.mouse_x(), y = main_disp.mouse_y();
            board.setPosXY(x, y);
            if (cnt == 100) board.setGesture(1);
            else if (cnt == 200) {
                board.setwords(wordlist, 5, true);
            }
            else if (cnt == 300) board.setGesture(2);
            else if (cnt == 400) {
                board.setwords(wordlist2, 4, true);
                cnt = 0;
            }
        }
    }
    return 0;
}
