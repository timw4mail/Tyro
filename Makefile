SOURCES = $(wildcard include/**/*.cpp include/*.cpp src/base/**/*.cpp)
OBJECTS = $(patsubst %.cpp,%.o, $(SOURCES))
BASE_LIB = build/base.a

JSON_FILES = $(patsubst config/%.json,%.json, $(wildcard config/*.json))

PROGRAM_SRC = $(wildcard src/widgets/*.cpp src/settings/*.cpp src/*.cpp)
PROGRAM = build/Tyro

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
	CXX = $(shell wx-config --cxx) -no-cpp-precomp -Xpreprocessor -Wno-missing-field-initializers
	LDLIBS += /usr/local/lib/libssh2.a
else
	CXX += -Wno-missing-field-initializers
	LDLIBS += -lssh2
endif

ifeq ($(OS),Windows_NT)
	CXXFLAGS += -static
	CXX += -I/include -DWIN32
	LDLIBS += -L/lib -lwsock32
endif

ifeq ($(CXX),clang++)
	CXX += -std=c++11
endif

CXX += -Iinclude -I. -I/usr/local/include

ifdef $(DEV)
all: CXXFLAGS = $(DEV_CXXFLAGS)
endif
all: build json_wrapper $(BASE_LIB) $(PROGRAM)
ifeq ($(OS),Darwin)
all: Tyro.app
endif

dev: DEV = true
dev: CXXFLAGS = $(DEV_CXXFLAGS)
dev: all
	
json_wrapper: json_wrapper_build
	$(foreach var, $(JSON_FILES), config/json2c config/$(var) $(patsubst %.json,config/%_json.h,$(var)) $(patsubst %.json,%_json,$(var));)

json_wrapper_build: 
	$(CC) config/json2c.c -o config/json2c

build:
	@mkdir -p build

$(BASE_LIB): build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@

$(PROGRAM): CXXFLAGS += $(WX_CXXFLAGS)
$(PROGRAM):
	$(CXX) $(CXXFLAGS) $(PROGRAM_SRC) $(BASE_LIB) $(WX_LDLIBS) $(LDLIBS) -o $(PROGRAM)
	
lib: $(OBJECTS) $(BASE_LIB)

run:
ifneq ($(OS),Darwin)
	./$(PROGRAM)
else
	./build/Tyro.app/Contents/MacOS/Tyro
endif


ifeq ($(OS),Darwin)
run-app:
	open -a $(PWD)/build/Tyro.app
endif

run-grind:
	valgrind $(PROGRAM)

# Make optimized and striped executable
release: DEV=
ifeq ($(OS),Darwin)
release: all
endif
ifeq ($(OS),Windows_NT)
release: exe
release:
	strip -SXx $(PROGRAM).exe
endif
ifeq ($(OS),Linux)
release: all
release:
	strip -SXx $(PROGRAM)
endif


# Windows resource linking
msw_resource:
	$(WX_RES) resources/platform/msw/resource.rc -O coff -o resource.res

exe: LDLIBS += resource.res
exe: json_wrapper_build json_wrapper $(BASE_LIB)
exe: msw_resource $(PROGRAM) 

# OS X application bundle	
Tyro.app:
ifndef DEV
	strip -SXx $(PROGRAM)
endif
	SetFile -t APPL $(BASE_LIB)
	-mkdir -p build/Tyro.app/Contents/MacOS
	-mkdir -p build/Tyro.app/Contents/Resources/English.lproj
	cp resources/platform/osx/Info.plist build/Tyro.app/Contents/
	echo -n 'APPL????' > build/Tyro.app/Contents/PkgInfo
	cp build/Tyro build/Tyro.app/Contents/MacOS/Tyro
	cp resources/platform/osx/tyro.icns build/Tyro.app/Contents/Resources/

$(TESTS): $(BASE_LIB)
	$(foreach var, $(TEST_SRC), $(CXX) $(CXXFLAGS) $(var) $(BASE_LIB) $(WX_LDLIBS) $(LDLIBS) -o $(patsubst %.cpp,%, $(var));)

.PHONY: tests	
tests: $(TESTS)
	sh ./tests/runtests.sh

clean:
	rm -f *.res
	rm -f config/json2c
	rm -f config/json2c.exe
	rm -f config/*_json.h
	rm -rf *.o
	rm -rf build
	rm -rf build $(OBJECTS) $(PROGRAM) $(BASE_LIB) $(TESTS)
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`
