cmake_minimum_required(VERSION 3.0)

project(ezwow)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O2 -fPIC -fpermissive -m32 -L/usr/lib/x86_64-linux-gnu")

set(CMAKE_CXX_FLAGS_LIST "${CMAKE_CXX_FLAGS_LIST} -m32")
set(CMAKE_EXE_LINK_FLAGS_LIST "${CMAKE_EXE_LINK_FLAGS_LIST} -m32")
set(CMAKE_SHARED_LINK_FLAGS_LIST "${CMAKE_SHARED_LINK_FLAGS_LIST} '-m32'")

set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS OFF)
set(CMAKE_SYSTEM_LIBRARY_PATH /lib32 /usr/lib32 /usr/lib/i386-linux-gnu /usr/local/lib32)
set(CMAKE_IGNORE_PATH /lib /usr/lib /usr/lib/x86_64-linux-gnu /usr/lib64 /usr/local/lib)
set(CMAKE_ASM_FLAGS "--32")

add_library(ezwow SHARED
    src/main.cpp 
    src/third_party/imgui/imgui_draw.cpp
    src/third_party/imgui/imgui.cpp
    src/third_party/imgui/imgui_widgets.cpp
    src/imgui_impl_opengl2.cpp
)

