#Try using clang, if it's installed
ifneq (`command -v clang`,)
	CC = clang
	CXX = clang++
endif

CXX += -Iinclude

SOURCES = $(wildcard include/**/*.cpp src/network/*.cpp src/settings/*.cpp include/*.cpp)
OBJECTS = $(patsubst %.cpp,%.o, $(SOURCES))
TARGET = build/Tyro.a

JSON_FILES = $(patsubst config/%.json,%.json, $(wildcard config/*.json))

PROGRAM_SRC = $(wildcard src/*.cpp src/widgets/*.cpp)
PROGRAM = build/Tyro
PROGRAM_OBJECTS = $(patsubst %.cpp,%.o, $(PROGRAM_SRC)) 

WX_LDLIBS = $(shell wx-config --libs base core aui stc adv) -lssh2
WX_CXXFLAGS =  $(shell wx-config --cxxflags)
WX_RES = $(shell wx-config --rescomp)

LDLIBS = $(TARGET) 

DEV_CXXFLAGS = -g -Wall -Wextra -DDEBUG
CXXFLAGS = -Os -DNDEBUG

TEST_SRC = $(wildcard tests/*.cpp)
TESTS = $(patsubst %.cpp,%,$(TEST_SRC))

OS ?= $(shell uname -s)

ifeq ($(OS),Darwin)
	CXX += -std=c++98 -mmacosx-version-min=10.5
else
	CXX += -std=c++11
endif

ifeq ($(OS),Windows_NT)
	LDLIBS += -L/lib
endif

all: build json_wrapper $(TARGET) $(PROGRAM)

dev: CXXFLAGS= $(DEV_CXXFLAGS)
dev: all
	
json_wrapper: json_wrapper_build
	$(foreach var, $(JSON_FILES), config/json2c config/$(var) $(patsubst %.json,config/%_json.h,$(var)) $(patsubst %.json,%_json,$(var));)

json_wrapper_build: 
	$(CC) config/json2c.c -o config/json2c

build:
	@mkdir -p build

$(TARGET): $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@


$(PROGRAM): CXXFLAGS += $(WX_CXXFLAGS) $(TARGET)
$(PROGRAM):
	$(CXX) $(WX_CXXFLAGS) $(PROGRAM_SRC) $(LDLIBS) $(WX_LDLIBS) -o $(PROGRAM)
	

run:
	./build/Tyro


run-grind:
	valgrind ./build/Tryo

# Make optimized and striped executable
release: all
	strip -SXx $(PROGRAM)
ifeq ($(OS),Darwin)
	make Tyro.app
endif
ifeq ($(OS),Windows_NT)
	make exe
endif


# Windows resource linking
msw_resource:
	$(WX_RES) resources/platform/msw/resource.rc -O coff -o resource.res

exe: LDLIBS += resource.res
exe: msw_resource all

# OS X application bundle	
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
	rm -f *.res
	rm -f config/json2c
	rm -f config/json2c.exe
	rm -f config/*_json.h
	rm -rf *.o
	rm -rf Tyro.app
	rm -rf build $(OBJECTS) $(PROGRAM) $(TARGET) $(TESTS)
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`
