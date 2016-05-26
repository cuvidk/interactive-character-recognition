#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scribblearea.h"

MainWindow::MainWindow(ScribbleArea* area, QWidget *parent) :
    QMainWindow(parent), scribbleArea(area)
{    
    setupUiElements();
    resize(720, 400);
}

void MainWindow::setupUiElements()
{
    centerWidget = new QWidget;

    hbox = new QHBoxLayout;
    vbox_left = new QVBoxLayout;
    vbox_right = new QVBoxLayout;    

    predictButton = new QPushButton("Predict");
    clearButton = new QPushButton("Clear Screen / Stop Demo");
    mnistDemoButton = new QPushButton("MNIST Demo");

    predictedLabel = new QLabel;
    predictedLabel->setAlignment(Qt::AlignCenter);
    QFont font = predictedLabel->font();
    font.setPointSize(20);
    predictedLabel->setFont(font);

    actualLabel = new QLabel;
    actualLabel->setAlignment(Qt::AlignCenter);
    font = actualLabel->font();
    font.setPointSize(20);
    actualLabel->setFont(font);

    correctWrongLabel = new QLabel("c / w");

    vbox_left->addWidget(scribbleArea);
    vbox_left->addWidget(predictButton);
    vbox_left->addWidget(clearButton);
    vbox_left->addWidget(mnistDemoButton);

    vbox_right->addWidget(predictedLabel);
    vbox_right->addWidget(actualLabel);
    vbox_right->addWidget(correctWrongLabel);

    hbox->addLayout(vbox_left);
    hbox->addLayout(vbox_right);

    centerWidget->setLayout(hbox);

    setCentralWidget(centerWidget);
}

void MainWindow::updatePredictionLabel(int pred)
{
    predictedLabel->setText("Predicted label: " + QString::number(pred));
    actualLabel->setText("");
}

void MainWindow::updateActualLabel(int label)
{
    actualLabel->setText("Actual label: " + QString::number(label));
}

MainWindow::~MainWindow()
{
    delete correctWrongLabel;
    delete actualLabel;
    delete predictedLabel;
    delete vbox_right;

    delete clearButton;
    delete predictButton;
    delete vbox_left;

    delete hbox;
    delete centerWidget;
}
