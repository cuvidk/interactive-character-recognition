#ifndef CHARACTERCLASSIFIER_H
#define CHARACTERCLASSIFIER_H

#include <QObject>
#include <QImage>
#include <QPainter>
#include <QColor>
#include <QRect>
#include <QPoint>
#include <QThread>

#include <armadillo>

class MnistDemo;
class NeuralNetwork;

class CharacterClassifier : public QObject
{
    Q_OBJECT
public:
    CharacterClassifier();
    ~CharacterClassifier();

public slots:
    void predictImage(QImage image);
    void emitNewPrediction(QImage image, int pred, int second);
    void startDemo();
    void stopDemo();

signals:
    void nnActualLabel(int actual);
    void nnPredictionLabel(int prediction);
    void newMnistImage(QImage image);

private:
    MnistDemo *mnistDemo;
    NeuralNetwork *nn;

    QPoint computeCenterOfMass(const QImage &image);
    void computeBestPredictions(const arma::mat &predictions, int &first, int &second);
    QImage foregroundToRect(QImage &image);
    void setForegroundLimits(const QImage &image, int &upper, int &bottom,
                             int &left, int &right);
};

#endif // CHARACTERCLASSIFIER_H
