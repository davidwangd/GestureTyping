#include "TextDisplay.h"
#include <windows.h>

TextDisplay disp;
int main() {
    disp.setText("Hello world!");
    while (true) {
        Sleep(1000);
        disp.setText("Hello world!");
        //disp.setText("This is a vvvvvvvvvvveeeeeeeeeeeeeerrrrrrrrrrrrrrrryyyyyyyyyyyyy lllllllllooooooonnnnnnnnngggggg message");
    }
    return 0;
}
