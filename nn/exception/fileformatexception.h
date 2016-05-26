#ifndef FILEFORMATEXCEPTION_H
#define FILEFORMATEXCEPTION_H

#include <stdexcept>
#include <string>

#include "nn/utils/nnfiletype.h"

class FileFormatException : public std::runtime_error
{
private:
    NNFileType m_fileType;

public:
    FileFormatException(const std::string& fileName, NNFileType fileType);
    const char* what() const throw ();
};

#endif // FILEFORMATEXCEPTION_H
