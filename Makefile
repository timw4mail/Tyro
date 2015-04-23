CXX += -I include -I.

SOURCES = $(wildcard include/**/*.cpp include/*.cpp src/network/*.cpp src/settings/*.cpp)
OBJECTS = $(patsubst %.cpp,%.o, $(SOURCES))
TYRO_LIB = build/Tyro.a

JSON_FILES = $(patsubst config/%.json,%.json, $(wildcard config/*.json))

PROGRAM_SRC = $(wildcard src/*.cpp src/widgets/*.cpp)
PROGRAM = build/Tyro
PROGRAM_OBJECTS = $(patsubst %.cpp,%.o, $(PROGRAM_SRC)) 

WX_LDLIBS = $(shell wx-config --libs base core aui stc adv)
WX_CXXFLAGS =  $(shell wx-config --cxxflags)
WX_RES = $(shell wx-config --rescomp)

DEV_CXXFLAGS = -g -Wall -Wextra -DDEBUG -DSTATIC_BUILD
CXXFLAGS = -Os -DNDEBUG -DSTATIC_BUILD

TEST_SRC = $(wildcard tests/*.cpp)
TESTS = $(patsubst %.cpp,%,$(TEST_SRC))

OS ?= $(shell uname -s)

LDLIBS =

ifeq ($(OS),Darwin)
	CXX += -std=c++98 -mmacosx-version-min=10.5
endif
ifeq ($(OS),Linux)
	CXX += -std=c++11
endif
ifeq ($(OS),Windows_NT)
	CXX += -I/include -DWIN32
	LDLIBS += -L/lib -lwsock32
endif

LDLIBS += -lssh2

all: build json_wrapper $(TYRO_LIB) $(PROGRAM)

dev: CXXFLAGS= $(DEV_CXXFLAGS)
dev: all
	
json_wrapper: json_wrapper_build
	$(foreach var, $(JSON_FILES), config/json2c config/$(var) $(patsubst %.json,config/%_json.h,$(var)) $(patsubst %.json,%_json,$(var));)

json_wrapper_build: 
	$(CC) config/json2c.c -o config/json2c

build:
	@mkdir -p build

$(TYRO_LIB): $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@


$(PROGRAM): CXXFLAGS += $(WX_CXXFLAGS) $(TYRO_LIB)
$(PROGRAM):
	$(CXX) $(CXXFLAGS) $(PROGRAM_SRC) $(TYRO_LIB) $(WX_LDLIBS) -o $(PROGRAM)
	

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
	SetFile -t APPL $(TYRO_LIB)
	-mkdir Tyro.app
	-mkdir Tyro.app/Contents
	-mkdir Tyro.app/Contents/MacOS
	-mkdir Tyro.app/Contents/Resources
	-mkdir Tyro.app/Contents/Resources/English.lproj
	cp resources/platform/osx/Info.plist Tyro.app/Contents/
	echo -n 'APPL????' > Tyro.app/Contents/PkgInfo
	cp build/Tyro Tyro.app/Contents/MacOS/Tyro
	cp resources/platform/osx/tyro.icns Tyro.app/Contents/Resources/

$(TESTS): $(TYRO_LIB)
	$(foreach var, $(TEST_SRC), $(CXX) $(CXXFLAGS) $(var) $(TYRO_LIB) $(LDLIBS) -o $(patsubst %.cpp,%, $(var));)

.PHONY: tests	
tests: $(TESTS)
	sh ./tests/runtests.sh

clean:
	rm -f *.res
	rm -f config/json2c
	rm -f config/json2c.exe
	rm -f config/*_json.h
	rm -rf *.o
	rm -rf Tyro.app
	rm -rf build $(OBJECTS) $(PROGRAM) $(TYRO_LIB) $(TESTS)
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`
