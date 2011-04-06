#-------------------------------------------------
#
# Project created by QtCreator 2011-01-24T12:40:32
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = QtChunkRenderer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    point.cpp \
    nbt.c \
    Map.cpp \
    FileEnumeration.cpp \
    endianness.c \
    Color.cpp \
    Chunk.cpp \
    gloptions.cpp \
    chunkselection.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    point.h \
    nbt.h \
    Map.h \
    FileEnumeration.h \
    endianness.h \
    Color.h \
    Chunk.h \
    gloptions.h \
    chunkselection.h

FORMS    += mainwindow.ui \
    chunkselection.ui

QMAKE_CXXFLAGS += -fopenmp

LIBS += -lgomp

INCLUDEPATH += zlib125-dll/include IL

unix:LIBS += -lzlib -lil

win32:LIBS += -L"C:\\Users\\Colton\\Desktop\\NBT_Library\\MapRenderer\\QtMapRenderer\\zlib125-dll\\lib" -lzdll -L"C:\\Users\\Colton\\Desktop\\NBT_Library\\MapRenderer\\DevIL" -lDevIL
