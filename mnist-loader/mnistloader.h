#ifndef MNISTLOADER_H_
#define MNISTLOADER_H_

#include <iostream>
#include <fstream>
#include <armadillo>
#include <stdexcept>

class MnistLoader
{
private:
	static bool IsLittleEndian();
	static int ReverseInt(int x);

public:
	static arma::mat LoadImages(const char* filename, bool normalize = true);
	static arma::mat LoadLabels(const char* filename, bool vectorize = true);
	static arma::mat Normalize(arma::mat container);
};

#endif //MNISTLOADER_H_
