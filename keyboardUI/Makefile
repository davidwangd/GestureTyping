flags = -O2  -std=gnu++0x
.PHONY: all
all: keyboardUI.exe
keyboardUI.exe: keyboardUI.cpp main.cpp keyboardUI.h
	g++ -o keyboardUI.exe keyboardUI.cpp main.cpp -O2 -lgdi32 -std=gnu++0x
.PHONY: clean
clean:
	del *.exe *.o
