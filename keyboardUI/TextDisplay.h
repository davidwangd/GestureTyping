#ifndef TEXT_DISPLAY
#define TEXT_DISPLAY

#include "CImg.h"

class Page {
public:
    Page();
    int lines, lineh;
    char *cur;
    char content[20][100];
    Page *last, *next;
    void setText(const char *text);
};


class TextDisplay{
public:
    cimg_library::CImg<unsigned char> visu;
    cimg_library::CImg<unsigned char> background;
    cimg_library::CImgDisplay disp;

    Page *pghdr, *curPage, *lastPage;

    TextDisplay();

    ~TextDisplay();

    void setText(const char *text);

    void delText();

    void setLetter(char ch);

    void delLetter();

    void display();

    void pageUp();

    void pageDown();

    FILE *f;

	enum {
		SET_TEXT,
		DEL_TEXT,
		SET_LETTER,
		DEL_LETTER
	} pre_op;
};
#endif // TEXT_DISPLAY
