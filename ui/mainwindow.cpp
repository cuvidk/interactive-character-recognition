#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scribblearea.h"
#include <QApplication>

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
    actual_hbox = new QHBoxLayout;
    vbox_left = new QVBoxLayout;
    vbox_right = new QVBoxLayout;    

    predictButton = new QPushButton("Predict");
    clearButton = new QPushButton("Clear Screen / Stop Demo");
    mnistDemoButton = new QPushButton("MNIST Demo");
    learnButton = new QPushButton("Learn this!");

    predictedLabel = new QLabel;
    predictedLabel->setAlignment(Qt::AlignCenter);
    QFont font = predictedLabel->font();
    font.setPointSize(20);
    predictedLabel->setFont(font);

    actualLabel = new QLabel("Actual label: ");
    actualLabel->setAlignment(Qt::AlignCenter);
    font = actualLabel->font();
    font.setPointSize(20);
    actualLabel->setFont(font);

    actualLabelBox = new QSpinBox;
    actualLabelBox->setMaximum(9);
    actualLabelBox->setMinimum(0);
    font = actualLabelBox->font();
    font.setPointSize(20);
    actualLabelBox->setFont(font);

    doneLabel = new QLabel;
    doneLabel->setAlignment(Qt::AlignCenter);
    font = doneLabel->font();
    font.setPointSize(20);
    doneLabel->setFont(font);

    actual_hbox->addWidget(actualLabel);
    actual_hbox->addWidget(actualLabelBox);
    actual_hbox->addWidget(learnButton);

    vbox_left->addWidget(scribbleArea);
    vbox_left->addWidget(predictButton);
    vbox_left->addWidget(clearButton);
    vbox_left->addWidget(mnistDemoButton);

    vbox_right->addWidget(predictedLabel);
    vbox_right->addLayout(actual_hbox);
    vbox_right->addWidget(doneLabel);

    hbox->addLayout(vbox_left);
    hbox->addLayout(vbox_right);

    centerWidget->setLayout(hbox);

    setCentralWidget(centerWidget);
}

void MainWindow::updatePredictionLabel(int pred)
{
    actualLabelBox->setValue(0);
    doneLabel->setText("");
    predictedLabel->setText("Predicted label: " + QString::number(pred));
}

void MainWindow::updateActualLabel(int label)
{
    actualLabelBox->setValue(label);
}

void MainWindow::disableWhenDemoRunning()
{    
    predictButton->setDisabled(true);
    learnButton->setDisabled(true);
    actualLabelBox->setDisabled(true);
}

void MainWindow::enableWhenDemoDone()
{
    predictButton->setDisabled(false);
    learnButton->setDisabled(false);
    actualLabelBox->setDisabled(false);
    predictedLabel->setText("");
    actualLabelBox->setValue(0);
}

void MainWindow::toggleUi()
{
    if (predictButton->isEnabled())
        disableWhenDemoRunning();
    else
        enableWhenDemoDone();

    mnistDemoButton->setDisabled(mnistDemoButton->isEnabled());
    clearButton->setDisabled(clearButton->isEnabled());
    qApp->processEvents();
}

void MainWindow::emitLearnData(QImage image)
{
    emit learnDataSignal(image, actualLabelBox->value());
}

void MainWindow::setDoneLearning()
{
    toggleUi();
    doneLabel->setText("Done, hit predict again.");
}

void MainWindow::learnWarningBox()
{
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, "Warning", "Learning a character may be time expensive.\n"
                                                "The whole UI will be blocked untill the end of learning process.\n"
                                                "There's also a chance that your character won't be learned after the process.\n"
                                                "Do you really want me to try to learn this?",
                               QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes)
    {
        doneLabel->setText("Learning. Please wait...");
        qApp->processEvents();
        emit startLearning();
    }
}

MainWindow::~MainWindow()
{
    delete doneLabel;
    delete actualLabelBox;
    delete actualLabel;
    delete learnButton;
    delete actual_hbox;
    delete predictedLabel;
    delete vbox_right;

    delete clearButton;
    delete mnistDemoButton;
    delete predictButton;
    delete vbox_left;

    delete hbox;
    delete centerWidget;
}
