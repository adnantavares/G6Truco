#pragma once

#include <vector>
#include "SocketBase.h"


namespace G6Truco::Services::Connection
{
	class ServerSocket : public SocketBase
	{
	private:
		static shared_ptr<ServerSocket> instance;

		ServerSocket();
	public:
		static shared_ptr<ServerSocket> getInstance();
		vector<SOCKET> ConnectToClients(int numberOfConnections);
	};
}

