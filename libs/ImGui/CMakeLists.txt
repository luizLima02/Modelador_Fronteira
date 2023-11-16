cmake_minimum_required(VERSION 3.20)
project(imgui_lib)

set(IMGUI_SRC  
    imgui_impl_opengl3.h
    imgui_impl_opengl3.cpp
    #imgui_impl_opengl3_loader.h
    imgui_impl_glfw.h
    imgui_impl_glfw.cpp
    imgui.h
    imgui.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui_demo.cpp
)

add_library(${PROJECT_NAME} ${IMGUI_SRC})