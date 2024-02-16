#include "pch.h"
#include <exception>
#include <string>

class PlayerCreationException : public std::exception {
private:
    std::string message;
public:
    PlayerCreationException(const std::string& msg) : message(msg) {}

    virtual const char* what() const throw() {
        return message.c_str();
    }
};