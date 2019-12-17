#-------------------------------------------------
#
# Project created by QtCreator 2018-08-28T10:55:17
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mesh_Computational_Geometry
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gldisplaywidget.cpp \
    mesh.cpp \
    utils.cpp \
    mesh_creation.hxx \
    mesh_debug.hxx \
    mesh_iterators.hxx \
    mesh_modification.hxx \
    mesh_predicate.hxx \
    mesh_statistics.hxx

HEADERS  += mainwindow.h \
    gldisplaywidget.h \
    mesh.h \
    segmentMemory.h \
    iterators.h \
    utils.h \
    testUtil.h


FORMS    += mainwindow.ui

#---- Comment the following line on MacOS
LIBS = -lGLU
CONFIG += c++11

