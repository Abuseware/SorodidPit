ARCH = i686
CXX = $(ARCH)-w64-mingw32-g++
LD = $(ARCH)-w64-mingw32-g++
WINDRES = $(ARCH)-w64-mingw32-windres

CXXFLAGS += -pipe -std=c++14 -Ofast -march=core2 -fPIC -fPIE
CXXFLAGS += -Wall -Wextra -Wno-unused-parameter -pedantic
LDFLAGS = -Wl,-O1 -mwindows -static-libstdc++ -static-libgcc -fPIC -fPIE

LD_LIBS = resources.o -lgdi32

ifdef DEBUG
	CXXFLAGS += -gdwarf
	LDFLAGS += -gdwarf
else
	LDFLAGS += -Wl,-s
endif

.PHONY: all clean
.EXTENSIONS: .exe

src = $(wildcard *.cc)
obj = $(src:.cc=.o) resources.o
dep = $(obj:.o=.d)

%.d: %.cc
	$(CXX) $(CXXFLAGS) -MM $< -MF $@

%.exe: %.o
	$(LD) $(LDFLAGS) -o $@ $< $(LD_LIBS)

all: ColorTest.exe

-include $(dep)

resources.o: resources.rc
	$(WINDRES) $< $@

ColorTest.exe: $(obj)

clean:
	rm -f $(obj) $(dep) $(wildcard *.exe)
