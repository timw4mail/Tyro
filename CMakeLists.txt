################################################################################
# Setup
################################################################################
cmake_minimum_required (VERSION 3.0.2)

project(Tyro)
include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/include)

# C++11, please
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(FATAL_ERROR "Compiler ${CMAKE_CXX_COMPILER} has no C++11 support.")
endif()

# wxwidgets stuff
find_package(wxWidgets COMPONENTS core base aui stc adv REQUIRED)
include("${wxWidgets_USE_FILE}")
set(wxWidgets_CONFIG_OPTIONS --static)

#libssh2
set(CMAKE_MODULE_PATH ${Tyro_SOURCE_DIR}/cmake)
find_package(LibSSH2 REQUIRED)
if (LIBSSH2_FOUND)
   set (INCLUDE_DIRS ${INCLUDE_DIRS} ${LIBSSH2_INCLUDE_DIR})
else (LIBSSH2_FOUND)
   message ( FATAL_ERROR "Could not find LibSSH2" )
endif (LIBSSH2_FOUND)


include_directories(${INCLUDE_DIRS})

# set some platform-specific flags
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(CMAKE_CXX_FLAGS "--sysroot ${CMAKE_OSX_SYSROOT} ${CMAKE_CXX_FLAGS} -stdlib=libc++")
	add_definitions(-D_WCHAR_H_CPLUSPLUS_98_CONFORMANCE_ -D__WXMAC__)
endif()

################################################################################
# Build
################################################################################

# json library
add_library(JsonLib STATIC
	include/jsoncpp.cpp)

# Build json conversion script
add_executable(json2c config/json2c.c)
add_custom_command(
	TARGET JsonLib PRE_BUILD
	COMMAND json2c ../config/languages.json ../config/languages_json.h languages_json
	DEPENDS json2c
)
add_custom_command(
	TARGET JsonLib PRE_BUILD
	COMMAND json2c ../config/themes.json ../config/themes_json.h themes_json
	DEPENDS json2c
)

# base library
add_library(BaseLib STATIC  
	src/base/SFTP.cpp
	src/settings/Config.cpp)

# widget library
file(GLOB widget_SRC
	"src/settings/*.cpp"
	"src/widgets/*.cpp"
)
add_library(WidgetLib STATIC ${widget_SRC})
#add_dependencies(WidgetLib BaseLib JsonLib)
target_link_libraries(WidgetLib JsonLib)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(MACOSX_BUNDLE_ICON_FILE, ${PROJECT_SOURCE_DIR}/resources/platform/osx/tyro.icns)
	set(MACOSX_BUNDLE_INFO_PLIST, ${PROJECT_SOURCE_DIR}/resources/platform/osx/Info.plist)
	add_executable(Tyro MACOSX_BUNDLE
		src/TyroApp.cpp)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	add_executable(Tyro WIN32
		resources/platform/msw/resource.rc
		src/TyroApp.cpp)
else()
	add_executable(Tyro
		src/TyroApp.cpp)
endif()

#link it all
target_link_libraries(Tyro JsonLib BaseLib WidgetLib "${wxWidgets_LIBRARIES}" "${Libssh2_LIBRARIES}")

################################################################################
# Tests
################################################################################
enable_testing(true)

file(GLOB test_SRC
	"tests/main.cpp"
	"tests/*Test.cpp"
)
add_executable(test_runner ${test_SRC})

target_link_libraries(test_runner BaseLib WidgetLib "${wxWidgets_LIBRARIES}" "${Libssh2_LIBRARIES}")
