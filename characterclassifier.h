#ifndef CHARACTERCLASSIFIER_H
#define CHARACTERCLASSIFIER_H

#include <QObject>
#include <QImage>
#include <QPainter>
#include <QColor>
#include <QRect>
#include <QPoint>
#include <QThread>
#include <QtConcurrent/QtConcurrent>

#include <iostream>
#include <stdexcept>
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
    void learnCharacter(QImage image, int label);
    void emitNewPrediction(QImage image, int pred, int second);    
    void startDemo();
    void stopDemo();

signals:
    void nnActualLabel(int actual);
    void nnPredictionLabel(int prediction);
    void newMnistImage(QImage image);
    void doneLearning();

private:
    MnistDemo *mnistDemo;
    NeuralNetwork *nn;

    arma::mat training_images;
    arma::mat training_labels;

    QPoint computeCenterOfMass(const QImage &image);
    QImage foregroundToRect(QImage &image);
    arma::mat getImageAsNormVec(const QImage &image);
    void computeBestPredictions(const arma::mat &predictions, int &first, int &second);
    void setForegroundLimits(const QImage &image, int &upper, int &bottom,
                             int &left, int &right);
    void preprocessImage(QImage &image, int finalImageSize);
    void loadTrainingData();
    void saveNnAndTrainingData();
};

#endif // CHARACTERCLASSIFIER_H
