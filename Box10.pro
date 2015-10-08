#-------------------------------------------------
#
# Project created by QtCreator 2015-10-03T15:19:19
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Box10
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    box.cpp \
    shader.cpp

HEADERS  += mainwindow.h \
    box.h \
    shader.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc

OTHER_FILES += \
    FragmentShader.sh \
    VertexShader.sh

LIBS += -lGLEW
