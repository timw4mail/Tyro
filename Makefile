CXX = $(shell wx-config --cxx)

SOURCES = $(wildcard include/**/*.cpp src/settings/*.cpp include/*.cpp)
OBJECTS = $(patsubst %.cpp,%.o, $(SOURCES))
TARGET = build/Tyro.a

PROGRAM_SRC = $(wildcard src/*.cpp src/widgets/*.cpp)
PROGRAM = build/Tyro
PROGRAM_OBJECTS = $(patsubst %.cpp,%.o, $(PROGRAM_SRC))

BASE_FLAGS = -DSCI_LEXER

LDLIBS = $(TARGET) $(shell wx-config --libs base core aui stc adv)
WX_CXXFLAGS =  $(shell wx-config --cxxflags) $(BASE_FLAGS)
DEV_CXXFLAGS = -g -Wall -Wextra
CXXFLAGS = -Os 
TEST_SRC= $(wildcard tests/*.cpp)
TESTS = $(patsubst %.cpp,%,$(TEST_SRC))

all: build $(TARGET) $(PROGRAM)

dev: CXXFLAGS= $(DEV_CXXFLAGS)
dev: all

build:
	@mkdir -p build

$(TARGET): CXXFLAGS += -fPIC
$(TARGET): $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@


$(PROGRAM): CXXFLAGS += $(WX_CXXFLAGS) $(TARGET)
$(PROGRAM):
	$(CXX) $(WX_CXXFLAGS) $(PROGRAM_SRC) $(LDLIBS) -o $(PROGRAM)
	

run:
	./build/Tyro

Tyro.app: all resources/platform/osx/Info.plist
	SetFile -t APPL $(TARGET)
	-mkdir Tyro.app
	-mkdir Tyro.app/Contents
	-mkdir Tyro.app/Contents/MacOS
	-mkdir Tyro.app/Contents/Resources
	-mkdir Tyro.app/Contents/Resources/English.lproj
	cp resources/platform/osx/Info.plist Tyro.app/Contents/
	echo -n 'APPL????' > Tyro.app/Contents/PkgInfo
	cp build/Tyro Tyro.app/Contents/MacOS/Tyro
	cp resources/platform/osx/tyro.icns Tyro.app/Contents/Resources/

.PHONY: tests	
tests: LDLIBS = $(TARGET) -lssh2
tests: $(TESTS)
	sh ./tests/runtests.sh

clean:
	rm -f *.o
	rm -rf Tyro.app
	rm -rf build $(OBJECTS) $(PROGRAM) $(TARGET) $(TESTS)
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`
