#include "ocrappmanager.h"
#include "ui/mainwindow.h"
#include "ui/scribblearea.h"
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
    QObject::connect(mainWindow->clearButton, SIGNAL(released()),
                     mainWindow, SLOT(enableWhenDemoDone()));
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
    QObject::connect(mainWindow->mnistDemoButton, SIGNAL(released()),
                     mainWindow, SLOT(disableWhenDemoRunning()));
    QObject::connect(mainWindow->learnButton, SIGNAL(released()),
                     mainWindow, SLOT(learnWarningBox()));
    QObject::connect(mainWindow, SIGNAL(startLearning()),
                     mainWindow, SLOT(toggleUi()));
    QObject::connect(mainWindow, SIGNAL(startLearning()),
                     scribbleArea, SLOT(emitImageToLearn()));
    QObject::connect(scribbleArea, SIGNAL(imageToLearnSignal(QImage)),
                     mainWindow, SLOT(emitLearnData(QImage)));
    QObject::connect(mainWindow, SIGNAL(learnDataSignal(QImage,int)),
                     characterClassifier, SLOT(learnCharacter(QImage,int)));
    QObject::connect(characterClassifier, SIGNAL(doneLearning()),
                     mainWindow, SLOT(setDoneLearning()));
}

OcrAppManager::~OcrAppManager()
{
    delete scribbleArea;
    delete mainWindow;
    delete characterClassifier;
}
