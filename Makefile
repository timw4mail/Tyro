PROGRAM = build/Tyro

WX_RES = $(shell wx-config --rescomp)
WX_CXXFLAGS =  $(shell wx-config --cxxflags)

TEST_RUNNER = build/test_runner

OS ?= $(shell uname -s)


all:
	./cmake.sh Tyro


dev:
	mkdir -p build
	cmake -DCMAKE_BUILD_TYPE=Debug -B./build -H.
	make -C ./build

run:
ifneq ($(OS),Darwin)
	./$(PROGRAM)
else
	./build/Tyro.app/Contents/MacOS/Tyro
endif


ifeq ($(OS),Darwin)
run-app: all
	open -a $(PWD)/build/Tyro.app
endif

run-grind: all
	valgrind $(PROGRAM)

# Make optimized and striped executable
ifeq ($(OS),Darwin)
release: all
endif
ifeq ($(OS),Windows_NT)
release: exe
	strip -SXx $(PROGRAM).exe
endif
ifeq ($(OS),Linux)
release: all
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


$(TEST_RUNNER):
	./cmake.sh test_runner

run-tests: tests
	
tests: $(TEST_RUNNER)
	./build/test_runner
	
tests-verbose: $(TEST_RUNNER)
	./build/test_runner -s

clean:
	rm -f *.res
	rm -f config/json2c
	rm -f config/json2c.exe
	rm -f config/*_json.h
	rm -rf build/*
	mkdir -p build
