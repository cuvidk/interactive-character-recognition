#include "mnistloader.h"

bool MnistLoader::IsLittleEndian()
{
	int x = 1;
	char *byte = (char*)&x;
	return (int)byte[0] == 1 ? true : false;	
}

int MnistLoader::ReverseInt(int x)
{
	return (x >> 24 ) + (x >> 8 & 0x0000ff00) + (x << 8 & 0x00ff0000) + (x << 24);
}

arma::mat MnistLoader::LoadImages(const char* filename, bool normalize)
{
	std::ifstream file(filename, std::ios::binary);

	if (file.is_open())
	{
		int magic_number, num_images, num_rows, num_cols;

		file.read((char*)&magic_number, 4);
		file.read((char*)&num_images, 4);
		file.read((char*)&num_rows, 4);
		file.read((char*)&num_cols, 4);

		if (IsLittleEndian())
		{
			magic_number = ReverseInt(magic_number);
			num_images = ReverseInt(num_images);
			num_rows = ReverseInt(num_rows);
			num_cols = ReverseInt(num_cols);
		}
		if (magic_number != 2051)
			throw std::runtime_error(std::string(filename) + " is not valid mnist images file.");

		arma::mat images(num_images, num_rows * num_cols);
		for (int image_index = 0; image_index < num_images; ++image_index)
		{	
			unsigned char *pixel_value = new unsigned char [num_rows * num_cols];
			file.read((char*)pixel_value, num_rows * num_cols);
			for (int pixel = 0; pixel < num_rows * num_cols; ++pixel)
			{
				if (normalize)
					images(image_index, pixel) = (2.0 * (int)pixel_value[pixel]) / 255 - 1;
				else
					images(image_index, pixel) = (double)pixel_value[pixel];
			}
			delete[] pixel_value;
		}
		file.close();
		return images;
	}
	else
		throw std::runtime_error("Failed to open: " + std::string(filename));	
}

arma::mat MnistLoader::LoadLabels(const char* filename, bool vectorize)
{
	std::ifstream file(filename);

	if (file.is_open())
	{
		int magic_number, num_items;

		file.read((char*)&magic_number, 4);
		file.read((char*)&num_items, 4);

		if (IsLittleEndian())
		{
			magic_number = ReverseInt(magic_number);
			num_items = ReverseInt(num_items);	
		}
		if (magic_number != 2049)
			throw std::runtime_error(std::string(filename) + " is not valid mnist labels file.");
		
		unsigned char *labels = new unsigned char [num_items];
		file.read((char*)labels, num_items);

		arma::mat vec_labels;
		if (vectorize)
		{
			vec_labels = arma::zeros(num_items, 10);
		
			for (int label_index = 0; label_index < num_items; ++label_index)
				vec_labels(label_index, labels[label_index]) = 1.0;
		}
		else
		{
			vec_labels = arma::zeros(num_items, 1);
		
			for (int label_index = 0; label_index < num_items; ++label_index)
				vec_labels(label_index, 0) = labels[label_index];
		}

		delete[] labels;
		file.close();
		return vec_labels;
	}
	else
		throw std::runtime_error("Failed to open: " + std::string(filename));
}

arma::mat MnistLoader::Normalize(arma::mat container)
{
	container.for_each( [](arma::mat::elem_type& val) { val = (2.0 * val) / 255 - 1; } );
	return container;
}

