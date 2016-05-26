#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <iostream>
#include <vector>
#include <cmath>
#include <armadillo>

class NeuralNetwork
{
private:
    std::vector<int> m_numNeuronsOnLayer;
    std::vector<arma::mat> m_activationOnLayer;
    std::vector<arma::mat> m_partialOnLayer;
    std::vector<arma::mat> m_theta;
    unsigned int m_numLayers;
    double m_regFactor;
    double m_learningRate;
    bool m_featureNormalization;

public:
    NeuralNetwork(std::initializer_list<int> numNeuronsOnLayer);
    NeuralNetwork(std::initializer_list<int> numNeuronsOnLayer, float regularizationFactor, float learningRate);
    NeuralNetwork(const std::string& filename);
    arma::mat predict(const arma::mat& input);
    void trainOn(const std::string& filename, int numIterations, int iterationsBetweenReport);
    void trainOn(arma::mat& input, const arma::mat& output, int numIterations, int iterationsBetweenReport);
    void setRegularizationFactor(double regularizationFactor);
    void setLearningRate(double learningRate);
    void setFeatureNormalization(bool value);
    void loadWeights(const std::string& fileName);
    void exportNeuralNetwork(const std::string& filename) const;
    double getPredictionAccuracy(const arma::mat& expectedOutput, const arma::mat& actualOutput) const;

private:
    void randomlyInitWeights();
    arma::mat feedForward(const arma::mat& input, const std::vector<arma::mat>& theta);
    arma::mat sigmoid(arma::mat input) const;
    arma::mat sigmoidGradient(const arma::mat& input) const;
    arma::mat logarithm(arma::mat input) const;
    double computeCost(const arma::mat& input, const arma::mat& output, const std::vector<arma::mat>& theta);
    double computeRegTerm(const std::vector<arma::mat>& theta) const;
    void backprop(const arma::mat& input, const arma::mat& output);
    void gradientDescent(const std::vector<arma::mat>& gradients);
    void normalizeFeatures(arma::mat& input) const;
    void checkGradients(arma::mat& input, arma::mat& output, std::vector<arma::mat>& gradients);
};

#endif // NEURALNETWORK_H
