#include "TextDisplay.h"

using namespace cimg_library;

const int width = 533, height = 800;

const unsigned char black[] = {0, 0, 0}, white[] = {255, 255, 255};

TextDisplay::TextDisplay():visu(width, height, 1, 3, 0), background("bg.bmp"), disp(background, "TextOutput") {
    visu.draw_image(0, 0, 0, 0, background).display(disp);
}

void TextDisplay::setText(const char *text) {
    //printf("Trying to show: %s\n", text);
    visu.draw_image(0, 0, 0, 0, background).draw_text(10, 10, text, black, 0, 1, 20).display(disp);

}
