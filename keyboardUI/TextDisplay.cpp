#include "TextDisplay.h"

using namespace cimg_library;

const int width = 533, height = 800;

const unsigned char black[] = {0, 0, 0}, white[] = {255, 255, 255};

TextDisplay::TextDisplay():visu(width, height, 1, 3, 0), background("bg.bmp"), disp(background, "TextOutput") {
    visu.draw_image(0, 0, 0, 0, background).display(disp);
    pghdr = new Page();
    lastPage = curPage = pghdr;
    f = fopen("result.txt", "w");
    fclose(f);
}

void TextDisplay::setText(const char *text) {
    //printf("Trying to show: %s\n", text);
	if (lastPage != curPage) return;

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
            lastPage = curPage;
        }
        else {
            curPage->lines++;
            curPage->cur = curPage->content[curPage->lines];
            printf("NewLine success\n");
        }
        strcat(curPage->cur, text);
        strcat(curPage->cur, " ");
    }        ///cannot place in this line

    display();
	pre_op = SET_TEXT;
	return;
}

void TextDisplay::display() {
    visu.draw_image(0, 0, 0, 0, background);
    for (int i = 0; i <= curPage->lines; i++)
        visu.draw_text(10, 10 + i * (curPage->lineh), curPage->content[i], black, 0, 1, 20);

    visu.display(disp);
}

void TextDisplay::delText() {
    if (lastPage != curPage) return;
    int len = strlen(curPage->cur);
    if (len == 0) {
        if (curPage->lines > 0) {
            curPage->lines--;
            curPage->cur = curPage->content[curPage->lines];
            len = strlen(curPage->cur);
        }
        else {
            if (curPage->last == NULL) return;
            curPage = curPage->last;
            lastPage = curPage;
            delete curPage->next;
            len = strlen(curPage->cur);
        }
    }

    len -= 2;
    while (len > 0 && curPage->cur[len] != ' ') len--;
    if (len) curPage->cur[len+1] = '\0';
    else curPage->cur[0] = '\0';

    display();
	pre_op = DEL_TEXT;
}

TextDisplay::~TextDisplay() {
    while (pghdr) {
        Page *nxt = pghdr->next;
        delete pghdr;
        pghdr = nxt;
    }
}

void TextDisplay::pageUp() {
    if (curPage->last != NULL) {
        curPage = curPage->last;
        display();
    }
}

void TextDisplay::pageDown() {
    if (curPage != lastPage) {
        curPage = curPage->next;
        display();
    }
}

void TextDisplay::setLetter(char c) {
	if (c == 0) return;
	CImg<unsigned char> img;
	if (c == '\t') {
		setLetter(' ');
		setLetter(' ');
	}
	if (c == '\n') {
		if (visu.height() < 20 + curPage->lineh * (curPage->lines + 2)) {     ///cannot put one more line
			curPage->next = new Page();
			curPage->next->last = curPage;
			curPage = curPage->next;
			lastPage = curPage;
		}
		else {
			curPage->lines++;
			curPage->cur = curPage->content[curPage->lines];
			printf("NewLine success\n");
		}
		return;
	}

	int len = strlen(curPage->cur);
	// 如果之前是输入的单词，那么我们需要一个空格的间隔
	if (pre_op == SET_TEXT) curPage->cur[len++] = ' ';
	curPage->cur[len++] = c;
	curPage->cur[len] = '\0';

	img.draw_text(0, 0, curPage->cur, black, 0, 1, 20);

	if (visu.width() < img.width() + 20) {
		printf("NewLine\n");
		
		// 考虑连续输入的单词内容，加入一个空格的分割。
		int pos = -1;
		for (int p = len;p >= 0; p--) {
			if (curPage->cur[p] == ' ') {
				curPage->cur[p] = '\0';
				pos = p;
				break;
			}
		}

		printf("%s\n", curPage->cur);

		if (visu.height() < 20 + curPage->lineh * (curPage->lines + 2)) {     ///cannot put one more line
			curPage->next = new Page();
			curPage->next->last = curPage;
			curPage = curPage->next;
			lastPage = curPage;
		}
		else {
			curPage->lines++;
			curPage->cur = curPage->content[curPage->lines];
			printf("NewLine success\n");
		}
		if (pos == -1) {
			curPage->cur[0] = c;
			curPage->cur[1] = '\0';
		}
		else {
			for (int i = pos + 1; curPage->content[curPage->lines - 1][i]; i++) {
				curPage->cur[i - pos - 1] = curPage->content[curPage->lines - 1][i];
				curPage->cur[i - pos] = '\0';
			}
		}
	}        ///cannot place in this line

	display();
	pre_op = SET_LETTER;
	return;
}

void TextDisplay::delLetter() {
	if (lastPage != curPage) return;
	int len = strlen(curPage->cur);
	if (len == 0) {
		if (curPage->lines > 0) {
			curPage->lines--;
			curPage->cur = curPage->content[curPage->lines];
			len = strlen(curPage->cur);
		}
		else {
			if (curPage->last == NULL) return;
			curPage = curPage->last;
			lastPage = curPage;
			delete curPage->next;
			len = strlen(curPage->cur);
		}
	}
	curPage->cur[len-1] = '\0';
	if (len == 1) {
		curPage->lines--;
		curPage->cur = curPage->content[curPage->lines];	
	}
	display();
	pre_op = DEL_TEXT;
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