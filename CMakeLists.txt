################################################################################
# Setup
################################################################################
cmake_minimum_required (VERSION 2.8)

set(CMAKE_CXX_STANDARD 11)

project(Tyro)

include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/include)

# C11, please
include(CheckCCompilerFlag)
CHECK_C_COMPILER_FLAG("-std=c11" COMPILER_SUPPORTS_C11)
if (COMPILER_SUPPORTS_C11)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
else()
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
endif()

# C++11, please
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	# Need this for Visual Studio compilers
else()
	message(FATAL_ERROR "Compiler ${CMAKE_CXX_COMPILER} has no C++11 support.")
endif()


if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
	# Silence some useless errors
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-potentially-evaluated-expression")
	# Set the correct standard lib
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
endif()

# wxwidgets stuff
#set(wxWidgets_CONFIG_OPTIONS --static)
find_package(wxWidgets COMPONENTS core base aui stc adv REQUIRED)
include("${wxWidgets_USE_FILE}")

#libssh2
#set(CMAKE_MODULE_PATH ${Tyro_SOURCE_DIR}/cmake)
#find_package(LibSSH2 REQUIRED)
#if (LIBSSH2_FOUND)
#	set (INCLUDE_DIRS ${INCLUDE_DIRS} ${LIBSSH2_INCLUDE_DIR})
#else (LIBSSH2_FOUND)
#	message ( FATAL_ERROR "Could not find LibSSH2" )
#endif (LIBSSH2_FOUND)

include_directories(${INCLUDE_DIRS})

# set some platform-specific flags
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(MACOSX_DEPLOYMENT_TARGET 10.9)
	#set(CMAKE_OSX_SYSROOT "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk")
	set(CMAKE_CXX_FLAGS "--sysroot ${CMAKE_OSX_SYSROOT} ${CMAKE_CXX_FLAGS} -stdlib=libc++")
	add_definitions(-D_WCHAR_H_CPLUSPLUS_98_CONFORMANCE_ -D__WXMAC__)
else()
	add_definitions(-D_WCHAR_H_CPLUSPLUS_98_CONFORMANCE_)
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
#	src/base/SFTP.cpp
	src/settings/Config.cpp)

# widget library
file(GLOB widget_SRC
	"src/settings/*.cpp"
	"src/widgets/*.cpp"
)
add_library(WidgetLib STATIC ${widget_SRC})
target_link_libraries(WidgetLib JsonLib)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(MACOSX_icon_file ${CMAKE_CURRENT_SOURCE_DIR}/resources/platform/osx/tyro.icns)
	set(MACOSX_BUNDLE_ICON_FILE tyro.icns)
	set(MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/resources/platform/osx/Info.plist)
	set_source_files_properties(${MACOSX_icon_file} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
	add_executable(Tyro MACOSX_BUNDLE
		src/TyroApp.cpp
		${MACOSX_icon_file}
	)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	add_executable(Tyro WIN32
		resources/platform/msw/resource.rc
		src/TyroApp.cpp)
else()
	add_executable(Tyro
		src/TyroApp.cpp)
endif()

#link it all
target_link_libraries(Tyro JsonLib BaseLib WidgetLib ${wxWidgets_LIBRARIES})# ${Libssh2_LIBRARIES})

################################################################################
# Tests
################################################################################
enable_testing(true)

file(GLOB test_SRC
	"tests/main.cpp"
	"tests/*Test.cpp"
)
add_executable(test_runner ${test_SRC})

target_link_libraries(test_runner ${wxWidgets_LIBRARIES} ${Libssh2_LIBRARIES} JsonLib BaseLib WidgetLib)
