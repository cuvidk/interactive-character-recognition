#ifndef NEURALNETWORKLOADER_H
#define NEURALNETWORKLOADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <armadillo>

class NnIO
{
private:
    static std::istringstream m_iss;
    static std::stringstream m_ss;

private:
    NnIO();
    static void feedISSOneLine(std::ifstream& file);

public:
    static void loadWeights(const std::string& filename, std::vector<arma::mat>& weights);
    static void loadSimpleData(const std::string& filename, arma::mat& container);
    static void loadUnifiedData(const std::string& filename, arma::mat& input, arma::mat& output);
	static void exportAsUnifiedData(const std::string& filename, arma::mat& input, arma::mat& output);
    static void saveWeights(const std::string& filename, const std::vector<arma::mat>& weights);
};

#endif // NEURALNETWORKLOADER_H
