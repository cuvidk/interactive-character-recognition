#include "neuralnetwork.h"
#include "nn/neural_network_io/nnio.h"
#include "nn/exception/invalidinputexception.h"
#include "nn/utils/nnfiletype.h"

NeuralNetwork::NeuralNetwork(std::initializer_list<int> numNeuronsOnLayer)
    :NeuralNetwork(numNeuronsOnLayer, 0.0, 0.01)
{}

NeuralNetwork::NeuralNetwork(std::initializer_list<int> numNeuronsOnLayer, float regularizationFactor, float learningRate)
    :m_numNeuronsOnLayer(numNeuronsOnLayer),
      m_numLayers(m_numNeuronsOnLayer.size()),
      m_regFactor(regularizationFactor),
      m_learningRate(learningRate),
      m_featureNormalization(false)
{
    if (m_numLayers < 2)
        throw InvalidInputException("A neural network cannot have less than 2 layers.");

    for (const auto& numNeurons : numNeuronsOnLayer)
        if (numNeurons <= 0)
            throw InvalidInputException("You can't have less than 1 neuron per layer.");

    randomlyInitWeights();
}

NeuralNetwork::NeuralNetwork(const std::string& filename)
    :m_regFactor(0.0),
      m_learningRate(0.01),
      m_featureNormalization(false)
{
    NnIO::loadWeights(filename, m_theta);
    m_numLayers = m_theta.size() + 1;

    unsigned int layer;
    for (layer = 0; layer < m_numLayers - 1; ++layer)
        m_numNeuronsOnLayer.push_back(m_theta[layer].n_cols - 1);
    m_numNeuronsOnLayer.push_back(m_theta[--layer].n_rows);
}

arma::mat NeuralNetwork::predict(const arma::mat& input)
{
    return feedForward(input, m_theta);
}

void NeuralNetwork::trainOn(const std::string& filename, int numIterations, int iterationsBetweenReport)
{
    arma::mat in, out;
    NnIO::loadUnifiedData(filename, in, out);
    trainOn(in, out, numIterations, iterationsBetweenReport);
}

void NeuralNetwork::trainOn(arma::mat& input, const arma::mat& output, int numIterations, int iterationsBetweenReport)
{
    if (input.n_cols != static_cast<unsigned int>(m_numNeuronsOnLayer[0]) ||
            output.n_cols != static_cast<unsigned int>(m_numNeuronsOnLayer[m_numLayers - 1]))
        throw InvalidInputException("File's input / output length doesn't match with the"
                                    "number of neurons on input / output layer.");

    if (m_featureNormalization)
        normalizeFeatures(input);

    double prevCost = computeCost(input, output, m_theta);
    double crtCost = prevCost;
    for (int iteration = 0; iteration < numIterations; ++iteration)
    {
        if (iterationsBetweenReport)
            if (iteration % iterationsBetweenReport == 0 || iteration + 1 == numIterations)
                std::cout << "Iteration: " << iteration << " | Cost: " << crtCost << std::endl;
        if (crtCost > prevCost)
        {
            std::cout << "The cost is increasing. Choose a smaller learning rate." << std::endl;
            return;
        }
        backprop(input, output);
        prevCost = crtCost;
        crtCost = computeCost(input, output, m_theta);
    }
}

void NeuralNetwork::setRegularizationFactor(double regularizationFactor)
{
    m_regFactor = regularizationFactor;
}

void NeuralNetwork::setLearningRate(double learningRate)
{
    m_learningRate = learningRate;
}

void NeuralNetwork::setFeatureNormalization(bool value)
{
    m_featureNormalization = value;
}

void NeuralNetwork::loadWeights(const std::string& fileName)
{
    NnIO::loadWeights(fileName, m_theta);

    if (m_theta.size() != m_numLayers -1)
        throw InvalidInputException("The weights you are trying to load don't correspond with "
                                    "the number of layers in the network.\n");

    for (unsigned int layer = 0; layer < m_numLayers - 1 ; ++layer)
    {
        if (m_theta[layer].n_rows != static_cast<unsigned int>(m_numNeuronsOnLayer[layer + 1]) ||
                m_theta[layer].n_cols - 1 != static_cast<unsigned int>(m_numNeuronsOnLayer[layer]))
        {
            std::stringstream ss;
            ss << "There are too many / few weights on layer " << (layer + 1) << "\n";
            throw InvalidInputException(ss.str());
        }
    }
}

void NeuralNetwork::exportNeuralNetwork(const std::string& filename) const
{
    NnIO::saveWeights(filename, m_theta);
}

double NeuralNetwork::getPredictionAccuracy(const arma::mat& expectedOutput, const arma::mat& actualOutput) const
{
    //YOU NEED CHECKS HERE!!
    unsigned int numTests = expectedOutput.n_rows;
    unsigned int correctlyClassified = 0;
    for (unsigned int testIndex = 0; testIndex < numTests; ++testIndex)
    {
        arma::uword expectedLabel, actualLabel, garbage;
        expectedOutput.row(testIndex).max(garbage, expectedLabel);
        actualOutput.row(testIndex).max(garbage, actualLabel);

        if (expectedLabel == actualLabel)
            ++correctlyClassified;
    }
    return (correctlyClassified * 100.0) / numTests;
}

void NeuralNetwork::randomlyInitWeights()
{
    arma::arma_rng::set_seed_random();
    for (unsigned int layer = 0; layer < m_numLayers - 1; ++layer)
    {
        double epsilon_init = sqrt(6) / sqrt(m_numNeuronsOnLayer[layer] + m_numNeuronsOnLayer[layer + 1]);
        arma::mat crtTheta = arma::randu(m_numNeuronsOnLayer[layer + 1], m_numNeuronsOnLayer[layer] + 1);
        crtTheta = crtTheta * 2 * epsilon_init - epsilon_init;
        m_theta.push_back(crtTheta);
    }
}

arma::mat NeuralNetwork::feedForward(const arma::mat& input, const std::vector<arma::mat>& theta)
{
    m_activationOnLayer.clear();
    m_partialOnLayer.clear();

    arma::mat activation = input;
    for (unsigned int layer = 0; layer < theta.size(); ++layer)
    {
        activation = arma::join_horiz(arma::ones(activation.n_rows, 1), activation);
        m_activationOnLayer.push_back(activation);

        arma::mat z = activation * theta[layer].t();
        m_partialOnLayer.push_back(z);

        activation = sigmoid(z);
    }
    m_activationOnLayer.push_back(activation);
    return activation;
}

arma::mat NeuralNetwork::sigmoid(arma::mat input) const
{
    input.for_each( [](arma::mat::elem_type& val) { val = 1 / (1 + exp(-val)); } );
    return input;
}

arma::mat NeuralNetwork::sigmoidGradient(const arma::mat& input) const
{
    return sigmoid(input) % (1 - sigmoid(input));
}

arma::mat NeuralNetwork::logarithm(arma::mat input) const
{
    input.for_each( [](arma::mat::elem_type& val) { val = log(val); } );
    return input;
}

double NeuralNetwork::computeCost(const arma::mat& input, const arma::mat& output, const std::vector<arma::mat>& theta)
{
    arma::mat h =  feedForward(input, theta);
    arma::mat h_1 = 1 - h;
    unsigned int m = input.n_rows;
    double cost = (-1.0 / m) * arma::accu(output % logarithm(h) + (1 - output) % logarithm(h_1));
    return cost + (m_regFactor / (2.0 * m)) * computeRegTerm(theta);
}

double NeuralNetwork::computeRegTerm(const std::vector<arma::mat>& theta) const
{
    double regularizationTerm = 0.0;

    for (unsigned int layer = 0; layer < theta.size(); ++layer)
    {
        arma::mat toRegularize = theta[layer].cols(1, theta[layer].n_cols - 1);
        regularizationTerm += arma::accu(toRegularize % toRegularize);
    }

    return regularizationTerm;
}

void NeuralNetwork::backprop(const arma::mat& input, const arma::mat& output)
{
    std::vector<arma::mat> gradients;

    gradients.push_back(m_activationOnLayer[m_numLayers - 1] - output);

    unsigned int prevErrorIndex = 0;
    for (int layer = m_numLayers - 2; layer > 0; --layer)
    {
        arma::mat error;
        error = gradients[prevErrorIndex++] * m_theta[layer].cols(1, m_theta[layer].n_cols - 1);
        error = error % sigmoidGradient(m_partialOnLayer[layer - 1]);
        gradients.push_back(error);
    }

    int errorIndex = 0;
    for (int layer = m_numLayers - 2; layer >= 0; --layer)
    {
        gradients[errorIndex] = (1.0 / input.n_rows) * (gradients[errorIndex].t() * m_activationOnLayer[layer]);
        ++errorIndex;
    }

    std::reverse(gradients.begin(), gradients.end());

    for (unsigned int layer = 0; layer < m_numLayers - 1; ++layer)
    {
        int lastCol = gradients[layer].n_cols - 1;
        gradients[layer].cols(1, lastCol) += (m_regFactor / input.n_rows) * m_theta[layer].cols(1, lastCol);
    }

    //checkGradients(input, output, gradients);
    gradientDescent(gradients);
}

void NeuralNetwork::gradientDescent(const std::vector<arma::mat>& gradients)
{
    for (unsigned int layer = 0; layer < m_numLayers - 1; ++layer)
        m_theta[layer] -= m_learningRate * gradients[layer];
}

void NeuralNetwork::normalizeFeatures(arma::mat& input) const
{
    //this needs to be tweaked. not okay..
    for (unsigned int feature = 0; feature < input.n_cols; ++feature)
    {
        double mean = arma::mean(input.col(feature));
        double stddev = arma::stddev(input.col(feature));
        input.col(feature) = (input.col(feature) - mean) / stddev;
    }
}

void NeuralNetwork::checkGradients(arma::mat &input, arma::mat &output, std::vector<arma::mat>& gradients)
{
    double e = 0.0001;
    std::vector<arma::mat> numericalGrads;
    for (unsigned int layer = 0; layer < gradients.size(); ++layer)
    {
        std::vector<arma::mat> thetas = m_theta;
        arma::mat grads = arma::ones(m_theta[layer].n_rows, m_theta[layer].n_cols);
        for (unsigned int i = 0; i < grads.n_rows; ++i)
        {
            for (unsigned int j = 0; j < grads.n_cols; ++j)
            {
                arma::mat thetaPlus = m_theta[layer];
                arma::mat thetaMinnus = m_theta[layer];
                thetaPlus(i, j) += e;
                thetaMinnus(i, j) -= e;

                thetas[layer] = thetaPlus;
                double loss1 = computeCost(input, output, thetas);
                thetas[layer] = thetaMinnus;
                double loss2 = computeCost(input, output, thetas);

                grads(i, j) = (loss1 - loss2) / (2 * e);
            }
        }
        numericalGrads.push_back(grads);
    }
    for (unsigned int layer = 0; layer < gradients.size(); ++layer)
    {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Numerical computed gradients" << std::endl;
        std::cout << numericalGrads[layer] << std::endl;
        std::cout << "Analytical computed gradients" << std::endl;
        std::cout << gradients[layer] << std::endl;
        std::cout << "----------------------------------------" << std::endl;
    }
}
