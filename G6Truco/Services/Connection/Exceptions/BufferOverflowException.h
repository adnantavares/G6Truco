#pragma once

#include <iostream>
#include <stdexcept>

using namespace std;


namespace G6Truco::Services::Connection::Exceptions
{
	class BufferOverflowException : overflow_error
	{
	private:
		int currentBufferSize;
		int allowedBufferSize;

	public:
		BufferOverflowException(string message, int currentBufferSize, int allowedBufferSize);

		inline const char* what() const override;
	};
}
