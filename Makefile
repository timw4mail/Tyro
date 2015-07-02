SOURCES = $(wildcard include/**/*.cpp include/*.cpp src/base/*.cpp)
OBJECTS = $(patsubst %.cpp,%.o, $(SOURCES))
BASE_LIB = build/base.a

JSON_FILES = $(patsubst config/%.json,%.json, $(wildcard config/*.json))

PROGRAM_SRC = $(wildcard src/*.cpp)
PROGRAM = build/Tyro

CONFIG_SRC = $(wildcard src/settings/*.cpp)
CONFIG_OBJ = $(patsubst %.cpp,%.o, $(CONFIG_SRC))
CONFIG_LIB = build/config.a

WIDGET_SRC = $(wildcard src/settings/*.cpp src/widgets/*.cpp src/base/widget/*.cpp)
WIDGET_OBJ = $(patsubst %.cpp,%.o, $(WIDGET_SRC))
WIDGET_LIB = build/widget.a

WX_RES = $(shell wx-config --rescomp)
WX_CXXFLAGS =  $(shell wx-config --cxxflags)

INC_FLAGS = -Iinclude -I. -I/usr/local/include
DEV_CXXFLAGS =  -O0 -g -Wall -Wextra -pipe -DDEBUG $(INC_FLAGS)
CXXFLAGS += -Os -pipe -DNDEBUG $(INC_FLAGS)

TEST_SRC = $(wildcard tests/*Test.cpp)
TESTS = $(patsubst %.cpp,%.o,$(TEST_SRC))
TEST_RUNNER = tests/runner

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
	CXX = $(shell wx-config --cxx) -D__WXMAC__ -D_WCHAR_H_CPLUSPLUS_98_CONFORMANCE_ -std=gnu++11
	LDLIBS += $(shell pkg-config libssh2 --libs)
else
	LDLIBS += -lssh2
endif

ifeq ($(OS),Windows_NT)
	CXXFLAGS += -static
	CXX += -std=gnu++11 -Wno-unknown-pragmas -Wno-missing-field-initializers -I/include -DWIN32
	LDLIBS += -L/lib -lws2_32 -lssh2
endif

ifeq ($(OS),Linux)
	CXX += -std=c++11 -Wno-unknown-pragmas -Wno-missing-field-initializers
endif

ifdef $(DEV)
all: CXXFLAGS = $(DEV_CXXFLAGS)
endif
all: build json_wrapper $(BASE_LIB) $(WIDGET_LIB) $(PROGRAM)
ifeq ($(OS),Darwin)
all: Tyro.app
endif

dev: DEV = true
dev: CXXFLAGS += $(DEV_CXXFLAGS)
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


$(CONFIG_LIB): build
	$(foreach var, $(CONFIG_SRC), $(CXX) $(CXXFLAGS) $(WX_CXXFLAGS) -c -o $(patsubst %.cpp,%.o,$(var)) $(var);) 
	ar rcs $@ $(CONFIG_OBJ)
	ranlib $@


$(WIDGET_LIB): build $(CONFIG_LIB)
	$(foreach var, $(WIDGET_SRC), $(CXX) $(CXXFLAGS) $(WX_CXXFLAGS) -c -o $(patsubst %.cpp,%.o,$(var)) $(var);)
	ar rcs $@ $(WIDGET_OBJ)
	ranlib $@


$(PROGRAM): $(WIDGET_LIB)
	$(CXX) $(CXXFLAGS) $(WX_CXXFLAGS) $(PROGRAM_SRC) $(WIDGET_LIB) $(CONFIG_LIB) $(BASE_LIB) $(WX_LDLIBS) $(LDLIBS) -o $(PROGRAM)
	

lib: $(OBJECTS) $(BASE_LIB) $(CONFIG_LIB) $(WIDGET_LIB)

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


$(TESTS):
	$(foreach var, $(TEST_SRC), $(CXX) $(CXXFLAGS) $(WX_CXXFLAGS) -c -o $(patsubst %.cpp,%.o,$(var)) $(var);)

.PHONY: tests
tests: $(TESTS) json_wrapper $(BASE_LIB) $(WIDGET_LIB)
	$(CXX)  $(CXXFLAGS) $(WX_CXXFLAGS) tests/main.cpp $(TESTS) $(WIDGET_LIB) $(BASE_LIB) $(LDLIBS) $(WX_LDLIBS) -o tests/runner


run-tests: tests
	./tests/runner

clean:
	rm -f *.res
	rm -f config/json2c
	rm -f config/json2c.exe
	rm -f config/*_json.h
	rm -rf *.o
	rm -rf build
	rm -rf $(OBJECTS) $(TESTS)
	rm -f tests/runner
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`
	mkdir -p build
