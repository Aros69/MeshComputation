cmake_minimum_required(VERSION 3.14)
project(meshcomputation)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(meshcomputation
        gldisplaywidget.cpp
        gldisplaywidget.h
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mesh.cpp
        mesh.h)
