#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QMessageBox>

class ScribbleArea;
class OcrAppManager;

class MainWindow : public QMainWindow
{
    friend class OcrAppManager;
    Q_OBJECT    

signals:
    void startLearning();
    void learnDataSignal(QImage image, int label);

public slots:
    void updatePredictionLabel(int pred);
    void updateActualLabel(int label);
    void disableWhenDemoRunning();
    void enableWhenDemoDone();
    void learnWarningBox();
    void emitLearnData(QImage image);
    void toggleUi();
    void setDoneLearning();

public:
    explicit MainWindow(ScribbleArea* scribbleArea, QWidget *parent = 0);
    ~MainWindow();

private:
    QWidget *centerWidget;

    QHBoxLayout *hbox;
    QHBoxLayout *actual_hbox;
    QVBoxLayout *vbox_left;
    QVBoxLayout *vbox_right;

    ScribbleArea *scribbleArea;

    QLabel *predictedLabel;
    QLabel *actualLabel;
    QLabel *doneLabel;

    QSpinBox *actualLabelBox;

    QPushButton *predictButton;
    QPushButton *clearButton;
    QPushButton *mnistDemoButton;
    QPushButton *learnButton;

    void setupUiElements();
};

#endif // MAINWINDOW_H
