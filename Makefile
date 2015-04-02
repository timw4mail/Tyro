CXX = $(shell wx-config --cxx) -Wno-c++11-compat-deprecated-writable-strings
TARGET = build/Tyro
LDLIBS = $(shell wx-config --libs all)
WX_CXXFLAGS = -I./src -I./include -static $(shell wx-config --cxxflags)
DEV_CXXFLAGS = -g -Wall -Wextra $(WX_CXXFLAGS)
CXXFLAGS = -Os -s $(WX_CXXFLAGS)

SOURCES = $(wildcard src/**/*.cpp src/*.cpp include/**/*.cpp include/*.cpp)
OBJECTS = $(patsubst %.cpp,%.o, $(SOURCES))

all: build $(SOURCES) $(TARGET)

dev: CXXFLAGS= $(DEV_CXXFLAGS)
dev: all

build:
	@mkdir -p build

$(TARGET): $(OBJECTS)
	$(CXX) $(LDLIBS) $(OBJECTS) -o $@

run:
	./build/Tyro

Tyro.app: all platform/osx/Info.plist
	SetFile -t APPL $(TARGET)
	-mkdir Tyro.app
	-mkdir Tyro.app/Contents
	-mkdir Tyro.app/Contents/MacOS
	-mkdir Tyro.app/Contents/Resources
	-mkdir Tyro.app/Contents/Resources/English.lproj
	cp platform/osx/Info.plist Tyro.app/Contents/
	echo -n 'APPL????' > Tyro.app/Contents/PkgInfo
	cp build/Tyro Tyro.app/Contents/MacOS/Tyro
	cp platform/osx/tyro.icns Tyro.app/Contents/Resources/

clean:
	rm -f *.o
	rm -rf Tyro.app
	rm -rf build $(OBJECTS) $(PROGRAM)
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`
