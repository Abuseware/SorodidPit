ARCH = i686
CXX = $(ARCH)-w64-mingw32-g++
CXXFLAGS += -std=c++14 -Wl,-s -Ofast -pipe
CXXFLAGS += -Wall -Wextra -Wno-unused-parameter -pedantic
CXXFLAGS += -mwindows -static-libstdc++ -static-libgcc

all: ColorTest.exe

ColorTest.exe: main.cpp
	$(CXX) $(CXXFLAGS) -o ColorTest.exe main.cpp -lgdi32

clean:
	rm ColorTest.exe
