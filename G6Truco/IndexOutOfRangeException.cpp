#include "pch.h"
#include <stdexcept>
#include <string>

class IndexOutOfRangeException : public std::runtime_error {
public:
    IndexOutOfRangeException(const std::string& message)
        : std::runtime_error(message) {}
};
