#ifndef TEXT_DISPLAY
#define TEXT_DISPLAY

#include "CImg.h"

class TextDisplay{
public:
    cimg_library::CImg<unsigned char> visu;
    cimg_library::CImg<unsigned char> background;
    cimg_library::CImgDisplay disp;

    TextDisplay();

    void setText(const char *text);
};
#endif // TEXT_DISPLAY
