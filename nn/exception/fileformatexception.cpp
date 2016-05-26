#include "fileformatexception.h"

FileFormatException::FileFormatException(const std::string& fileName, NNFileType fileType)
    :std::runtime_error(fileName + "'s file format is invalid.\nFileformat:\n"),
      m_fileType(fileType)
{}

const char* FileFormatException::what() const throw ()
{
    std::string message;
    message += std::runtime_error::what();

    switch (m_fileType)
    {
    case NNFileType::SIMPLE:
        message += "\t#dataRows #colsPerRow\n\trow[1]\n\trow[2]\n\t...\n\trow[#dataRows]\n";
        message += "Make sure length(row[i]) = #colsPerRow, where i = 1..#dataRows\n";
        break;
    case NNFileType::UNIFIED:
        message += "\t#pairs #features #classes\n";
        message += "\tfeature[1][1] feature[1][2] ... feature[1][#features]\n";
        message += "\tclass[1][1] class[1][2] ... class[1][#classes]\n\t...\n";
        message += "\tfeature[#pairs][1] feature[#pairs][2] ... feature[#pairs][#features]\n";
        message += "\tclass[#pairs][1] class[#pairs][2] ... class[#pairs][#classes]\n\t...";
        break;
    case NNFileType::WEIGHTS:
        message += "\t#layers - 1\n\t#neurons_layer_2 (#neurons_layer_1) + 1(bias)\n";
        message += "\tweight_[1][0] weight_[1][1] ... weight_[1][#neurons_layer1]\n";
        message += "\tweight_[2][0] weight_[2][1] ... weight_[2][#neurons_layer1]\n\t...\n";
        message += "\tweight_[#neurons_layer_2][0] weight_[#neurons_layer_2][1] ... weight_[#neurons_layer_2][#neurons_layer_1]\n";
        message += "\t#neurons_layer_3 (#neurons_layer_2) + 1(bias)\n";
        message += "\tweight_[1][0] weight_[1][1] ... weight_[1][#neurons_layer2]\n\t...";
        break;
    }

    return message.c_str();
}
