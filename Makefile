ARCH = i686
CXX = $(ARCH)-w64-mingw32-g++
LD = $(ARCH)-w64-mingw32-g++

CXXFLAGS += -pipe -std=c++14 -Ofast -fPIC
CXXFLAGS += -Wall -Wextra -Wno-unused-parameter -pedantic
LDFLAGS = -Wl,-O1 -mwindows -static-libstdc++ -static-libgcc -fPIE

LD_LIBS = -lgdi32

ifdef DEBUG
	CXXFLAGS += -gwarf
	LDFLAGS += -gdwarf
else
	LDFLAGS += -Wl,-s
endif

.PHONY: all clean
.EXTENSIONS: .exe

src = $(wildcard *.cc)
obj = $(src:.cc=.o)
dep = $(obj:.o=.d)

%.d: %.cc
	$(CXX) $(CXXFLAGS) -MM $< -MF $@

%.exe: %.o
	$(LD) $(LDFLAGS) -o $@ $< $(LD_LIBS)

all: ColorTest.exe
-include $(dep)

ColorTest.exe: $(obj)

clean:
	rm -f $(obj) $(dep) $(wildcard *.exe)
