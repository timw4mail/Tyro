CXX = $(shell wx-config --cxx) -I/include

SOURCES = $(wildcard include/**/*.cpp src/network/*.cpp src/settings/*.cpp include/*.cpp)
OBJECTS = $(patsubst %.cpp,%.o, $(SOURCES))
TARGET = build/Tyro.a

JSON_FILES = $(patsubst config/%.json,%.json, $(wildcard config/*.json))

PROGRAM_SRC = $(wildcard src/*.cpp src/widgets/*.cpp)
PROGRAM = build/Tyro
PROGRAM_OBJECTS = $(patsubst %.cpp,%.o, $(PROGRAM_SRC))

BASE_FLAGS = -DSCI_LEXER 

LDLIBS = $(TARGET) $(shell wx-config --libs base core aui stc adv) -L/lib -lssh2
WX_CXXFLAGS =  $(shell wx-config --cxxflags) $(BASE_FLAGS)
DEV_CXXFLAGS = -g -Wall -Wextra
CXXFLAGS = -Os 
TEST_SRC= $(wildcard tests/*.cpp)
TESTS = $(patsubst %.cpp,%,$(TEST_SRC))

all: build json_wrapper $(TARGET) $(PROGRAM)

dev: CXXFLAGS= $(DEV_CXXFLAGS)
dev: all
	
json_wrapper: json_wrapper_build
	$(foreach var, $(JSON_FILES), config/json2c config/$(var) $(patsubst %.json,config/%_json.h,$(var)) $(patsubst %.json,%_json,$(var));)

json_wrapper_build: 
	$(CC) config/json2c.c -o config/json2c

build:
	@mkdir -p build

#$(TARGET): CXXFLAGS += 
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
	rm -f config/json2c
	rm -f config/*_json.h
	rm -f *.o
	rm -rf Tyro.app
	rm -rf build $(OBJECTS) $(PROGRAM) $(TARGET) $(TESTS)
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`
