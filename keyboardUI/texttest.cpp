#include "TextDisplay.h"
#include <windows.h>
#include <conio.h>

TextDisplay disp;
const int ESC = 27;
const int ENTER = 13;
const int DEL = 8;

int main() {
    disp.setText("Hello world!");
    char cmd[100];

    int single = 0;
    int len = 0;

    while (true){
    	char c = getch();
    	printf("%d\n", c);
		
		if (c == ESC) single ^= 1;
		if (c == ENTER) disp.setLetter('\n');

		if (single == 1){
			if (c == DEL) disp.delLetter();
			else disp.setLetter(c);
		}else{
			if (c == DEL) disp.delText();
			else if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')){
				cmd[len++] = c;
			}else{
				cmd[len] = 0;
				disp.setText(cmd);
				len = 0;
			}
		}
    }

    return 0;
}
