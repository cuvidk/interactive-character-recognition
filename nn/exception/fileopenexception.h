#ifndef FILEOPENEXCEPTION_H
#define FILEOPENEXCEPTION_H

#include <stdexcept>
#include <string>

class FileOpenException : public std::runtime_error
{
public:
    FileOpenException(const std::string& fileName)
        :std::runtime_error("Failed to open file : " + fileName + ". Are you sure that's a real file name?")
    {}
};

#endif // FILEOPENEXCEPTION_H
