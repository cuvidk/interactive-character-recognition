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
        ui/mainwindow.cpp \
    ui/scribblearea.cpp \
    nn/exception/fileformatexception.cpp \
    nn/neural_network/neuralnetwork.cpp \
    nn/neural_network_io/nnio.cpp \
    mnist-loader/mnistloader.cpp \
    digit-classification/mnistdemo.cpp \
    ocrappmanager.cpp \
    digit-classification/characterclassifier.cpp

HEADERS  += ui/mainwindow.h \
    ui/scribblearea.h \
    nn/exception/fileformatexception.h \
    nn/exception/fileopenexception.h \
    nn/exception/invalidinputexception.h \
    nn/neural_network/neuralnetwork.h \
    nn/neural_network_io/nnio.h \
    nn/utils/nnfiletype.h \
    mnist-loader/mnistloader.h \
    digit-classification/mnistdemo.h \
    ocrappmanager.h \
    digit-classification/characterclassifier.h \
    util/sleeper.h

FORMS    += ui/mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../usr/lib/release/ -larmadillo
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../usr/lib/debug/ -larmadillo
else:unix: LIBS += -L$$PWD/../../../../../../usr/lib/ -larmadillo

INCLUDEPATH += $$PWD/../../../../../../usr/include
DEPENDPATH += $$PWD/../../../../../../usr/include
