#pragma once

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

class ServerSocket
{
public:
    ServerSocket();
    static std::shared_ptr<ServerSocket> StartListening();

};

