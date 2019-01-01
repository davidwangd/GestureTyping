OS := $(shell uname)
ARCH := $(shell uname -m)

ifeq ($(OS), Linux)
  ifeq ($(ARCH), x86_64)
    LEAP_LIBRARY := ../lib/x64/libLeap.so -Wl,-rpath,../lib/x64
  else
    LEAP_LIBRARY := ../lib/x86/libLeap.so -Wl,-rpath,../lib/x86
  endif
else
  # OS X
  LEAP_LIBRARY := ../lib/libLeap.dylib
endif

Sample: Sample.cpp getWords.cpp keyboardUI.cpp Listener.cpp TextDisplay.cpp TextDisplay.h CImg.h getWords.hpp keyboardUI.h Leap.h LeapMath.h Listener.h Point.h
	$(CXX) -Wall -g  -O2 -lm -lpthread -I /opt/X11/include -L /opt/X11/lib -lm -lpthread -lX11 -I../include Sample.cpp getWords.cpp keyboardUI.cpp Listener.cpp TextDisplay.cpp -o Sample $(LEAP_LIBRARY)
ifeq ($(OS), Darwin)
	install_name_tool -change @loader_path/libLeap.dylib ../lib/libLeap.dylib Sample
endif

clean:
	rm -rf Sample Sample.dSYM
