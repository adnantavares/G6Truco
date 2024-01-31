#pragma once

#include "SocketBase.h"


namespace G6Truco::Services::Connection
{
	class ClientSocket : public SocketBase
	{
	private:
		static shared_ptr<ClientSocket> instance;

		ClientSocket();
	public:
		static shared_ptr<ClientSocket> getInstance();
	};
}

