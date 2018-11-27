#include "keyboardUI.h"
//#include<unistd.h>
#include<stdlib.h>
#include<time.h>
#include "windows.h"

keyboard board;
int main() {
    srand(time(NULL));
    int gesture = 0;
    for (int i = 1; i <= 300; i++) {
        board.setPosXY(rand() % 1200, rand() % 315);
        //usleep(20000000);
        Sleep(100);
        if (i % 100 == 0) {
            gesture = 1 - gesture;
            board.setGesture(gesture);
        }
    }
    return 0;
}
