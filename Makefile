SOURCES = $(wildcard include/**/*.cpp include/*.cpp src/base/**/*.cpp)
OBJECTS = $(patsubst %.cpp,%.o, $(SOURCES))
TYRO_LIB = build/Tyro.a

JSON_FILES = $(patsubst config/%.json,%.json, $(wildcard config/*.json))

PROGRAM_SRC = $(wildcard src/*.cpp src/widgets/*.cpp src/settings/*.cpp)
PROGRAM = build/Tyro
PROGRAM_OBJECTS = $(patsubst %.cpp,%.o, $(PROGRAM_SRC)) 

WX_RES = $(shell wx-config --rescomp)
WX_CXXFLAGS =  $(shell wx-config --cxxflags)

DEV_CXXFLAGS = -g -Wall -Wextra -DDEBUG
CXXFLAGS = -Os -DNDEBUG

TEST_SRC = $(wildcard tests/*.cpp)
TESTS = $(patsubst %.cpp,%,$(TEST_SRC))

LDLIBS =

OS ?= $(shell uname -s)

# Get static version of libs to link to on platforms that support it
ifneq ($(OS),Linux)
	WX_LDLIBS = $(shell wx-config --static --libs base core aui stc adv)
else
	WX_LDLIBS = $(shell wx-config --libs base core aui stc adv)
endif

# Platform compiler flags
ifeq ($(OS),Darwin)
	CXX = clang++ -std=c++98 -mmacosx-version-min=10.5
	LDLIBS += /usr/local/lib/libssh2.a
endif
ifeq ($(OS),Linux)
	CXX += -std=c++11
	LDLIBS += -lssh2
endif
ifeq ($(OS),Windows_NT)
	CXXFLAGS = -DNDEBUG -static
	CXX += -I/include -DWIN32
	LDLIBS += -L/lib -lwsock32 -lssh2
endif

CXX += -I include -I.

all: build json_wrapper $(TYRO_LIB) $(PROGRAM)

dev: CXXFLAGS = $(DEV_CXXFLAGS)
dev: all
	
json_wrapper: json_wrapper_build
	$(foreach var, $(JSON_FILES), config/json2c config/$(var) $(patsubst %.json,config/%_json.h,$(var)) $(patsubst %.json,%_json,$(var));)

json_wrapper_build: 
	$(CC) config/json2c.c -o config/json2c

build:
	@mkdir -p build

$(TYRO_LIB): build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@


$(PROGRAM): CXXFLAGS += $(WX_CXXFLAGS)
$(PROGRAM):
	$(CXX) $(CXXFLAGS) $(PROGRAM_SRC) $(TYRO_LIB) $(WX_LDLIBS) $(LDLIBS) -o $(PROGRAM)
	
lib: $(OBJECTS) $(TYRO_LIB)

run:
	./build/Tyro


run-grind:
	valgrind $(PROGRAM)

# Make optimized and striped executable
release:
ifeq ($(OS),Darwin)
	make all
	strip -SXx $(PROGRAM)
	make Tyro.app
endif
ifeq ($(OS),Windows_NT)
	make exe
	strip -SXx $(PROGRAM).exe
endif
ifeq ($(OS),Linux)
	make all
	strip -SXx $(PROGRAM)
endif


# Windows resource linking
msw_resource:
	$(WX_RES) resources/platform/msw/resource.rc -O coff -o resource.res

exe: LDLIBS += resource.res
exe: json_wrapper_build json_wrapper $(TYRO_LIB)
exe: msw_resource $(PROGRAM) 

# OS X application bundle
Tyro.app: CXXFLAGS += -static	
Tyro.app: all
	SetFile -t APPL $(TYRO_LIB)
	-mkdir -p build/Tyro.app
	-mkdir -p build/Tyro.app/Contents
	-mkdir -p build/Tyro.app/Contents/MacOS
	-mkdir -p build/Tyro.app/Contents/Resources
	-mkdir -p build/Tyro.app/Contents/Resources/English.lproj
	cp resources/platform/osx/Info.plist build/Tyro.app/Contents/
	echo -n 'APPL????' > build/Tyro.app/Contents/PkgInfo
	cp build/Tyro build/Tyro.app/Contents/MacOS/Tyro
	cp resources/platform/osx/tyro.icns build/Tyro.app/Contents/Resources/

$(TESTS): $(TYRO_LIB)
	$(foreach var, $(TEST_SRC), $(CXX) $(CXXFLAGS) $(var) $(TYRO_LIB) $(WX_LDLIBS) $(LDLIBS) -o $(patsubst %.cpp,%, $(var));)

.PHONY: tests	
tests: $(TESTS)
	sh ./tests/runtests.sh

clean:
	rm -f *.res
	rm -f config/json2c
	rm -f config/json2c.exe
	rm -f config/*_json.h
	rm -rf *.o
	rm -rf build/Tyro.app
	rm -rf build $(OBJECTS) $(PROGRAM) $(TYRO_LIB) $(TESTS)
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`
