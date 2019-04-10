cmake_minimum_required(VERSION 3.13)
set(HAVE_FLAG_SEARCH_PATHS_FIRST 0)

project(colortest CXX RC)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-pipe -march=core2 -mtune=generic -fPIC -fPIE")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")
set(CMAKE_EXE_LINKER_FLAGS "-mwindows -static-libstdc++ -static-libgcc -fPIC -fPIE")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-Wl,-O1")
set(MINGW True)
set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")

add_executable(colortest
        ColorTest.cc
        resources.rc
        )

target_link_libraries(colortest gdi32)