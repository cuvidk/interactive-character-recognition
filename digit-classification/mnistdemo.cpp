#include "mnistdemo.h"

#include "nn/neural_network/neuralnetwork.h"
#include "mnist-loader/mnistloader.h"
#include "sleeper.h"

MnistDemo::MnistDemo(NeuralNetwork *nn)
    :nn(nn), m_aborted(false)
{}

void MnistDemo::run()
{
    try {
        Sleeper sleeper;

        arma::mat test_images = MnistLoader::LoadImages("resources/t10k-images-idx3-ubyte", false);
        arma::mat test_labels = MnistLoader::LoadLabels("resources/t10k-labels-idx1-ubyte", false);

        QImage digit(28, 28, QImage::Format_RGB32);
        for (unsigned int image = 0; image < test_images.n_rows; ++image)
        {
            for (int pixel = 0; pixel < 28 * 28; ++pixel)
            {
                int col = test_images(image, pixel);
                digit.setPixel(pixel - (pixel / 28) * 28,
                               pixel / 28, qRgb(col, col, col));
            }

            arma::uword garbage, predicted_label;
            nn->predict(MnistLoader::Normalize(test_images.row(image))).max(garbage, predicted_label);

            if (!m_aborted)
                emit newPrediction(digit,
                                   static_cast<int>(predicted_label),
                                   static_cast<int>(test_labels(image, 0)));
            else
                break;

            sleeper.sleep(1);
        }
    } catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
}

void MnistDemo::stop()
{
    m_aborted = true;
}
