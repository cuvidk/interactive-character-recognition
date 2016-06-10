#ifndef MNISTDEMO_H
#define MNISTDEMO_H

#include <QThread>
#include <QImage>

#include <armadillo>

class NeuralNetwork;

class MnistDemo : public QThread
{
    Q_OBJECT

public:
    MnistDemo(NeuralNetwork *nn);
    void stop();
    void run();

signals:
    void newPrediction(QImage image, int predictedLabel, int actualLabel);

private:
    NeuralNetwork *nn;
    bool m_aborted;
};

#endif // MNISTDEMO_H
