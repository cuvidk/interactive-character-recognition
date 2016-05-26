#ifndef INVALIDINPUTEXCEPTION_H
#define INVALIDINPUTEXCEPTION_H

#include <stdexcept>

class InvalidInputException : public std::runtime_error
{
public:
    InvalidInputException(const std::string& message)
        :std::runtime_error("Invalid input. " + message)
    {}
};

#endif // INVALIDINPUTEXCEPTION_H
