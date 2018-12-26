flags = -O2  -std=gnu++0x
.PHONY: all
all: keyboardUI.exe
keyboardUI.exe: keyboardUI.cpp main.cpp keyboardUI.h
	g++ -o keyboardUI.exe keyboardUI.cpp main.cpp -O2 -lgdi32 -std=gnu++0x
.PHONY: test
test: test.exe
test.exe: keyboardUI.cpp test.cpp keyboardUI.h
	g++ -o test.exe keyboardUI.cpp test.cpp -O2 -lgdi32 -std=gnu++0x
.PHONY: texttest
texttest: texttest.exe
texttest.exe: texttest.cpp TextDisplay.cpp TextDisplay.h
	g++ -o texttest.exe texttest.cpp TextDisplay.cpp -O2 -lgdi32 -std=gnu++0x
.PHONY: clean
clean:
	del *.exe *.o
