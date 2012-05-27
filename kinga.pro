TEMPLATE = app
CONFIG += qt thread
TARGET = kinga
DEPENDPATH += . include src
INCLUDEPATH += . include

# Input
HEADERS += include/kingapresets.hpp
HEADERS += include/kingafileconverter.hpp
HEADERS += include/kingafilemanager.hpp
HEADERS += include/kingamainwindow.hpp
SOURCES += src/kingafileconverter.cpp
SOURCES += src/kingafilemanager.cpp
SOURCES += src/kingamainwindow.cpp
SOURCES += src/kinga.cpp

unix {
    CONFIG += x11
    MAGICK++_VERSION = $$system(Magick++-config --version)
    isEmpty(MAGICK++_VERSION):error(Missing Magick++)
    QMAKE_CFLAGS += $$system(Magick++-config --cppflags --cxxflags)
    QMAKE_CXXFLAGS += $$system(Magick++-config --cppflags --cxxflags)
    QMAKE_LFLAGS += $$system(Magick++-config --ldflags)
    QMAKE_LIBS += $$system(Magick++-config --libs)
}

