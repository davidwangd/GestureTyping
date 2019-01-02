#include "TextDisplay.h"

using namespace cimg_library;

const int width = 533, height = 800;

const unsigned char black[] = {0, 0, 0}, white[] = {255, 255, 255};

TextDisplay::TextDisplay():visu(width, height, 1, 3, 0), background("bg.bmp"), disp(background, "TextOutput") {
    visu.draw_image(0, 0, 0, 0, background).display(disp);
    pghdr = new Page();
    curPage = pghdr;
    f = fopen("result.txt", "w");
    fclose(f);
}

void TextDisplay::setText(const char *text) {
    //printf("Trying to show: %s\n", text);

    f = fopen("result.txt", "a");
    fprintf(f, text);
    fprintf(f, " ");
    fclose(f);


    CImg<unsigned char> img;

    int len = strlen(curPage->cur);
    strcat(curPage->cur, text);
    strcat(curPage->cur, " ");
    img.draw_text(0, 0, curPage->cur, black, 0, 1, 20);

    if (visu.width() < img.width() + 20) {
        printf("NewLine\n");
        curPage->cur[len] = '\0';
        printf("%s\n", curPage->cur);

        if (visu.height() < 20 + curPage->lineh * (curPage->lines + 2)) {     ///cannot put one more line
            curPage->next = new Page();
            curPage->next->last = curPage;
            curPage = curPage->next;
        }
        else {
            curPage->lines++;
            curPage->cur = curPage->content[curPage->lines];
            printf("NewLine success\n");
        }
        strcat(curPage->cur, text);
        strcat(curPage->cur, " ");
    }        ///cannot place in this line
    visu.draw_image(0, 0, 0, 0, background);
    for (int i = 0; i <= curPage->lines; i++)
        visu.draw_text(10, 10 + i * (curPage->lineh), curPage->content[i], black, 0, 1, 20).display(disp);

}

TextDisplay::~TextDisplay() {
    while (pghdr) {
        Page *nxt = pghdr->next;
        delete pghdr;
        pghdr = nxt;
    }
}

Page::Page() {
    last = next = NULL;
    lines = 0;
    CImg<unsigned char> img;
    img.draw_text(0, 0, "testgraph", black, 0, 1, 20);
    lineh = img.height();
    for (int i = 0; i < 20; i++) memset(content[i], 0, 100);
    cur = content[lines];
}
