#include "nnio.h"
#include "nn/exception/fileopenexception.h"
#include "nn/exception/fileformatexception.h"

std::istringstream NnIO::m_iss;
std::stringstream NnIO::m_ss;

NnIO::NnIO() {}

void NnIO::feedISSOneLine(std::ifstream &file)
{
    m_iss.str(std::string());
    m_iss.clear();

    std::string line;
    std::getline(file, line);
    m_iss.str(line);
}

void NnIO::loadWeights(const std::string& filename, std::vector<arma::mat>& weights)
{
    std::ifstream file(filename);

    if (file.is_open())
    {
        feedISSOneLine(file);

        unsigned int numMatrices = 0;
        m_iss >> numMatrices;

        if (!numMatrices || !m_iss.eof())
            throw FileFormatException(filename, NNFileType::WEIGHTS);

        unsigned int matrixIndex = 0;
        while (matrixIndex++ < numMatrices)
        {
            feedISSOneLine(file);

            unsigned int numRows = 0, numCols = 0;
            m_iss >> numRows >> numCols;

            if ((!numRows && !numCols) || !m_iss.eof())
                throw FileFormatException(filename, NNFileType::WEIGHTS);

            arma::mat matrix(numRows, numCols);

            unsigned int rowIndex = 0;
            while (rowIndex < numRows)
            {
                feedISSOneLine(file);

                std::vector<double> row {std::istream_iterator<double>{m_iss}, std::istream_iterator<double>{}};

                if (row.size() != numCols)
                    throw FileFormatException(filename, NNFileType::WEIGHTS);

                matrix.row(rowIndex++) = arma::conv_to<arma::rowvec>::from(row);
            }
            weights.push_back(matrix);
        }
        if (!file.eof())
            throw FileFormatException(filename, NNFileType::UNIFIED);
    }
    else
        throw FileOpenException(filename);

    file.close();
}

void NnIO::loadSimpleData(const std::string& filename, arma::mat& container)
{
    std::ifstream file(filename);
    if (file.is_open())
    {
        feedISSOneLine(file);

        unsigned int numRows = 0, numCols = 0;
        m_iss >> numRows >> numCols;

        if ((!numRows && !numCols) || !m_iss.eof())
            throw FileFormatException(filename, NNFileType::SIMPLE);

        container = arma::mat(numRows, numCols);

        unsigned int rowCount = 0;
        while (rowCount < numRows)
        {
            feedISSOneLine(file);

            std::vector<double> rowToInsert {std::istream_iterator<double>{m_iss}, std::istream_iterator<double>{}};

            if (rowToInsert.size() != numCols)
                throw FileFormatException(filename, NNFileType::SIMPLE);

            container.row(rowCount++) = arma::conv_to<arma::rowvec>::from(rowToInsert);
        }
        if (!file.eof())
            throw FileFormatException(filename, NNFileType::UNIFIED);
    }
    else
        throw FileOpenException(filename);

    file.close();
}

void NnIO::loadUnifiedData(const std::string& filename, arma::mat& input, arma::mat& output)
{
    std::ifstream file(filename);

    if (file.is_open())
    {
        feedISSOneLine(file);

        unsigned int numRows = 0, numColsIn = 0, numColsOut = 0;
        m_iss >> numRows >> numColsIn >> numColsOut;

        if ((!numRows && !numColsIn && !numColsOut) || !m_iss.eof())
            throw FileFormatException(filename, NNFileType::UNIFIED);

        input = arma::mat(numRows, numColsIn);
        output = arma::mat(numRows, numColsOut);

        unsigned int rowCount = 0;
        while (rowCount < numRows)
        {
            feedISSOneLine(file);
            std::vector<double> in = {std::istream_iterator<double>{m_iss}, std::istream_iterator<double>{}};

            feedISSOneLine(file);
            std::vector<double> out = {std::istream_iterator<double>{m_iss}, std::istream_iterator<double>{}};

            if (in.size() != numColsIn || out.size() != numColsOut)
                throw FileFormatException(filename, NNFileType::UNIFIED);

            input.row(rowCount) = arma::conv_to<arma::rowvec>::from(in);
            output.row(rowCount++) = arma::conv_to<arma::rowvec>::from(out);
        }

        if (!file.eof())
            throw FileFormatException(filename, NNFileType::UNIFIED);
    }
    else
        throw FileOpenException(filename);

    file.close();
}

void NnIO::saveWeights(const std::string& filename, const std::vector<arma::mat>& weights)
{
    std::ofstream file(filename);

    if (file.is_open())
    {
        file << weights.size() << "\n";
        for (unsigned int layer = 0; layer < weights.size(); ++layer)
        {
            file << weights[layer].n_rows << " " << weights[layer].n_cols << "\n";
            for (unsigned int rowIndex = 0; rowIndex < weights[layer].n_rows; ++rowIndex)
            {
                m_ss.str(std::string());
                m_ss.clear();
                std::vector<double> row = arma::conv_to<std::vector<double>>::from(weights[layer].row(rowIndex));
                std::copy(row.begin(), row.end(), std::ostream_iterator<double>(m_ss, " "));
                if (static_cast<unsigned int>(rowIndex) == weights[layer].n_rows - 1 &&
                        layer == weights.size() - 1)
                    file << m_ss.rdbuf();
                else
                    file << m_ss.rdbuf() << "\n";
            }
        }
    }
    else
        throw FileOpenException(filename);
}
