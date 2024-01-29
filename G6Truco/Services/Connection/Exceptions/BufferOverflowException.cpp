#include "pch.h"
#include "BufferOverflowException.h"

using namespace G6Truco::Services::Connection::Exceptions;


BufferOverflowException::BufferOverflowException(string message, int currentBufferSize, int allowedBufferSize) : overflow_error(message)
{
	this->currentBufferSize = currentBufferSize;
	this->allowedBufferSize = allowedBufferSize;
}

inline const char* BufferOverflowException::what() const
{
	string errorMessage = overflow_error::what();

	errorMessage += "\nAllowed Buffer Size: ";
	errorMessage += allowedBufferSize;
	errorMessage += "\nCurrent Buffer Size: ";
	errorMessage += currentBufferSize;

	return errorMessage.c_str();
}
