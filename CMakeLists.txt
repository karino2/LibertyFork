# This is only for Android NDK. Use Makefile for PC.
cmake_minimum_required(VERSION 3.5)

project("liberty")

add_library(liberty
SHARED
genmove.c
attacker.c
defender.c
shapes.c
matchpat.c
hacks.c
gnugo.c
opening.c
sethand.c
jni_bridge.c
)

# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# liberty.h duplicate symbol problem workaround.
target_compile_options(liberty PRIVATE -fcommon)

target_link_libraries(liberty
                      log)