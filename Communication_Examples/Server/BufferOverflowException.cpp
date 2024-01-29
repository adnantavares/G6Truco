#include "BufferOverflowException.h"

BufferOverflowException::BufferOverflowException(std::string message, int currentBufferSize, int allowedBufferSize) : std::overflow_error(message)
{
	this->currentBufferSize = currentBufferSize;
	this->allowedBufferSize = allowedBufferSize;
}

inline const char* BufferOverflowException::what() const
{
	std::string errorMessage = std::overflow_error::what();

	errorMessage += "\nAllowed Buffer Size: ";
	errorMessage += allowedBufferSize;
	errorMessage += "\nCurrent Buffer Size: ";
	errorMessage += currentBufferSize;

	return errorMessage.c_str();
}
