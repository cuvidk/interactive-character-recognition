#include "characterclassifier.h"
#include "mnistdemo.h"
#include "nn/neural_network/neuralnetwork.h"

#include <iostream>

CharacterClassifier::CharacterClassifier()
{
    nn = new NeuralNetwork("resources/mnist-classifier");
    nn->setLearningRate(0.1);
    nn->setRegularizationFactor(0.1);
    mnistDemo = nullptr;
}

void CharacterClassifier::startDemo()
{
    if (mnistDemo == nullptr)
    {
        mnistDemo = new MnistDemo(nn);

        connect(mnistDemo, SIGNAL(newPrediction(QImage,int,int)), this,
                SLOT(emitNewPrediction(QImage,int,int)));

        mnistDemo->start();
    }
}

void CharacterClassifier::stopDemo()
{
    if (mnistDemo != nullptr)
    {
        mnistDemo->stop();
        if (!mnistDemo->wait(2000))
        {
            mnistDemo->terminate();
            mnistDemo->wait();
        }
        delete mnistDemo;
        mnistDemo = nullptr;
    }
}

void CharacterClassifier::emitNewPrediction(QImage image, int pred, int actual)
{
    emit nnPredictionLabel(pred);
    emit nnActualLabel(actual);
    emit newMnistImage(image);
}

void CharacterClassifier::preprocessImage(QImage &image, int finalImageSize)
{
    image = foregroundToRect(image);
    image = image.scaled(20, 20, Qt::KeepAspectRatio);

    QPoint centerOfMass = computeCenterOfMass(image);

    QImage finalImage(finalImageSize, finalImageSize, QImage::Format_RGB32);
    finalImage.fill(qRgb(0, 0, 0));

    QPainter painter(&finalImage);
    QPoint drawPoint(finalImage.width() / 2 - centerOfMass.x(),
                     finalImage.height() / 2 - centerOfMass.y());
    painter.drawImage(drawPoint, image);
    painter.end();

    image = finalImage;
}

arma::mat CharacterClassifier::getImageAsNormVec(const QImage &image, int finalImageSize)
{
    arma::mat vecImage(1, finalImageSize * finalImageSize);
    for (int col = 0; col < image.width(); ++col)
        for (int row = 0; row < image.height(); ++row)
        {
            QColor pixel = image.pixel(col, row);
            double normalizedPixel = pixel.red() / 127.5 - 1;
            vecImage(0, row * finalImageSize + col) = normalizedPixel;
        }

    return vecImage;
}

void CharacterClassifier::predictImage(QImage image)
{
    int finalImageSize = 28;

    preprocessImage(image, finalImageSize);
    arma::mat vecImage = getImageAsNormVec(image, finalImageSize);

    int firstPred, secondPred;
    computeBestPredictions(nn->predict(vecImage), firstPred, secondPred);

    emit nnPredictionLabel(firstPred);
}

void CharacterClassifier::learnCharacter(QImage image, int label)
{
    int finalImageSize = 28;

    preprocessImage(image, finalImageSize);
    arma::mat vecImage = getImageAsNormVec(image, finalImageSize);

    //////////
    //creates a vector that equals with the label of the image
    //this should be changed, to adapt to all characters
    arma::mat vecLabel = arma::zeros(1, 10);
    vecLabel(0, label) = 1.0;
    /////////

    nn->trainOn(vecImage, vecLabel, 200);

    emit doneLearning();
}

QPoint CharacterClassifier::computeCenterOfMass(const QImage &image)
{
    QPoint centerOfMass(0, 0);
    int sum_x = 0, sum_y = 0, cumulated_weights = 0;

    for (int row = 0; row < image.height(); ++row)
        for (int col = 0; col < image.width(); ++col)
        {
            QColor color = image.pixel(col, row);
            int pixel_intensity = color.red();
            cumulated_weights += pixel_intensity;
            sum_y += row * pixel_intensity;
            sum_x += col * pixel_intensity;
        }

    if (cumulated_weights > 0)
    {
        centerOfMass.setX(sum_x / cumulated_weights);
        centerOfMass.setY(sum_y / cumulated_weights);
    }
    return centerOfMass;
}

void CharacterClassifier::computeBestPredictions(const arma::mat &predictions, int &first, int &second)
{
    double firstMax = predictions(0, 0),
            secondMax = predictions(0, 1);
    first = 0;
    second = 1;

    for (unsigned int i = 0; i < predictions.n_cols; ++i)
    {
        if (predictions(0, i) > firstMax)
        {
            secondMax = firstMax;
            second = first;
            firstMax = predictions(0, i);
            first = i;
        }
        else if (predictions(0, i) > secondMax)
        {
            secondMax = predictions(0, i);
            second = i;
        }
    }
}

QImage CharacterClassifier::foregroundToRect(QImage &image)
{
    int upper = 0, bottom = 0, left = 0, right = 0;

    setForegroundLimits(image, upper, bottom, left, right);

    if (right > left && upper < bottom)
    {
        QRect foreground(left, upper,
                         right - left + 1, bottom - upper + 1);

        image = image.copy(foreground);
    }

    return image;
}

bool static isBlack(const QColor &color)
{
    if (color.red() == 0 &&
            color.green() == 0 &&
            color.blue() == 0)
        return true;
    return false;
}

void CharacterClassifier::setForegroundLimits(const QImage &image, int &upper,
                                              int &bottom, int &left, int&right)
{
    for (int row = 0; (row < image.height()) && !upper; ++row)
        for (int col = 0; (col < image.width()) && !upper; ++col)
        {
            QColor color = image.pixel(col, row);

            if (!isBlack(color))
                upper = row;
        }

    for (int col = 0; col < image.width() && !left; ++col)
        for (int row = 0; row < image.height() && !left; ++row)
        {
            QColor color = image.pixel(col, row);

            if (!isBlack(color))
                left = col;
        }

    for (int row = image.height() - 1; row >= 0 && !bottom; --row)
        for (int col = 0; col < image.width() && !bottom; ++col)
        {
            QColor color = image.pixel(col, row);

            if (!isBlack(color))
                bottom = row;
        }

    for (int col = image.width() - 1; col >= 0 && !right; --col)
        for (int row = 0; row < image.height() && !right; ++row)
        {
            QColor color = image.pixel(col, row);

            if (!isBlack(color))
                right = col;
        }
}

CharacterClassifier::~CharacterClassifier()
{
    stopDemo();
    delete nn;
}
