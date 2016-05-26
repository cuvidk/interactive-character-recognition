#-------------------------------------------------
#
# Project created by QtCreator 2016-05-25T13:59:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = interactive-character-recognition
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    scribblearea.cpp \
    nn/exception/fileformatexception.cpp \
    nn/neural_network/neuralnetwork.cpp \
    nn/neural_network_io/nnio.cpp \
    mnist-loader/mnistloader.cpp \
    mnistdemo.cpp \
    ocrappmanager.cpp \
    characterclassifier.cpp

HEADERS  += mainwindow.h \
    scribblearea.h \
    nn/exception/fileformatexception.h \
    nn/exception/fileopenexception.h \
    nn/exception/invalidinputexception.h \
    nn/neural_network/neuralnetwork.h \
    nn/neural_network_io/nnio.h \
    nn/utils/nnfiletype.h \
    mnist-loader/mnistloader.h \
    mnistdemo.h \
    ocrappmanager.h \
    characterclassifier.h \
    sleeper.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../usr/lib/release/ -larmadillo
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../usr/lib/debug/ -larmadillo
else:unix: LIBS += -L$$PWD/../../../../../../usr/lib/ -larmadillo

INCLUDEPATH += $$PWD/../../../../../../usr/include
DEPENDPATH += $$PWD/../../../../../../usr/include
