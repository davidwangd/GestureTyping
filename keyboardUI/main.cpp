#include "keyboardUI.h"

using namespace cimg_library;

CImg<unsigned char> image("keyboard.bmp");
keyboard board;

int main() {
    CImgDisplay main_disp(image, "Click a point");
    int gesture;
    while (!main_disp.is_closed()) {
        main_disp.wait();
        if (main_disp.button() && main_disp.mouse_x() >= 0 && main_disp.mouse_y()>=0) {
            int x = main_disp.mouse_x(), y = main_disp.mouse_y();
            board.setPosXY(x, y);
            if (x > 1000) {
                gesture = y / 100;
                if (gesture < 3) board.setGesture(gesture);
            }
        }
    }
    return 0;
}
