#pragma once
#include <iostream>
#include <stdexcept>

class BufferOverflowException : std::overflow_error
{
private:
	int currentBufferSize;
	int allowedBufferSize;

public:
	BufferOverflowException(std::string message, int currentBufferSize, int allowedBufferSize);

	inline const char* what() const override;
};

