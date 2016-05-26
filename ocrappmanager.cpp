#include "ocrappmanager.h"
#include "mainwindow.h"
#include "scribblearea.h"
#include "characterclassifier.h"

OcrAppManager::OcrAppManager()
{
    characterClassifier = new CharacterClassifier;
    scribbleArea = new ScribbleArea;
    mainWindow = new MainWindow(scribbleArea);

    manageConnections();

    mainWindow->show();
}

void OcrAppManager::manageConnections()
{
    QObject::connect(mainWindow->clearButton, SIGNAL(released()),
                     characterClassifier, SLOT(stopDemo()));
    QObject::connect(mainWindow->clearButton, SIGNAL(released()),
                     scribbleArea, SLOT(clearScreen()));
    QObject::connect(mainWindow->predictButton, SIGNAL(released()),
                     scribbleArea, SLOT(emitImageSignal()));
    QObject::connect(scribbleArea, SIGNAL(imageSignal(QImage)),
                     characterClassifier, SLOT(predictImage(QImage)));
    QObject::connect(characterClassifier, SIGNAL(nnPredictionLabel(int)),
                     mainWindow, SLOT(updatePredictionLabel(int)));
    QObject::connect(characterClassifier, SIGNAL(nnActualLabel(int)),
                     mainWindow, SLOT(updateActualLabel(int)));
    QObject::connect(characterClassifier, SIGNAL(newMnistImage(QImage)),
                     scribbleArea, SLOT(setImage(QImage)));
    QObject::connect(mainWindow->mnistDemoButton, SIGNAL(released()),
                     characterClassifier, SLOT(startDemo()));
    QObject::connect(mainWindow->mnistDemoButton, SIGNAL(released()),
                     scribbleArea, SLOT(setAreaUnrenderable()));
}

OcrAppManager::~OcrAppManager()
{
    delete scribbleArea;
    delete mainWindow;
    delete characterClassifier;
}
