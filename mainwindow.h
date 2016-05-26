#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

class ScribbleArea;
class OcrAppManager;

class MainWindow : public QMainWindow
{
    friend class OcrAppManager;
    Q_OBJECT    

public slots:
    void updatePredictionLabel(int pred);
    void updateActualLabel(int label);

public:
    explicit MainWindow(ScribbleArea* scribbleArea, QWidget *parent = 0);
    ~MainWindow();

private:
    QWidget *centerWidget;

    QHBoxLayout *hbox;
    QVBoxLayout *vbox_left;
    QVBoxLayout *vbox_right;

    ScribbleArea *scribbleArea;

    QLabel *predictedLabel;
    QLabel *actualLabel;
    QLabel *correctWrongLabel;

    QPushButton *predictButton;
    QPushButton *clearButton;
    QPushButton *mnistDemoButton;

    void setupUiElements();
};

#endif // MAINWINDOW_H
