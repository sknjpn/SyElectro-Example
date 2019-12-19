cmake_minimum_required (VERSION 3.0)

find_package(PkgConfig)

project(OpenSiv3D_Linux CXX)
enable_language(C)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -Wno-unknown-pragmas -fPIC -msse4.1")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -pg -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -march=x86-64")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g3 -Og -pg")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG -march=x86-64")
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_C_COMPILER "gcc92")
set(CMAKE_CXX_COMPILER "g++92")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	add_compile_options ("-fcolor-diagnostics")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	add_compile_options ("-fdiagnostics-color=always")
endif()

#set(CMAKE_BUILD_TYPE Release)

pkg_check_modules(OPENCV4 REQUIRED opencv4)

link_directories(
	"/home/sknjpn/boost_1_70_0/boost/lib"
	"/home/sknjpn/sdk/angelscript/lib"
)
include_directories(
	"/usr/include"
	"/usr/include/boost"
	"/usr/local/include"
	
	"../../Siv3D/include"
	"../../Siv3D/include/ThirdParty"
)

set(SOURCE_FILES
"./Command_MakeCommand.cpp"
"./Command_MoveObject.cpp"
"./Command_RemoveObject.cpp"
"./Command_MoveObject.cpp"
"./Command_RemoveWire.cpp"
"./Command_SetObject.cpp"
"./Command_SetWire.cpp"
"./Command_Switch.cpp"
"./Command.cpp"
"./Main.cpp"
"./Node.cpp"
"./Object_Battery.cpp"
"./Object_Light.cpp"
"./Object_MakeObject.cpp"
"./Object_Relay.cpp"
"./Object_Switch.cpp"
"./Object.cpp"
"./Wire.cpp"
"./World.cpp"
)

add_executable(Siv3D_App ${SOURCE_FILES})

target_link_libraries(Siv3D_App
	${OPENCV4_LIBRARIES}
	-lOpenGL
	-lGLEW
	-lX11
	-langelscript
	-lpthread
	-ldl
	-ludev
	-lfreetype
	-lharfbuzz
	-lglib-2.0
	-lgobject-2.0
	-lgio-2.0
	-lpng
	-lturbojpeg
	-lgif
	-lwebp
	-lopenal
	-logg
	-lvorbis
	-lvorbisenc
	-lvorbisfile
	-lboost_filesystem
	-lavformat
	-lavcodec
	-lavutil
	-lswresample

	${PROJECT_SOURCE_DIR}/../Build/libSiv3D.a
)

