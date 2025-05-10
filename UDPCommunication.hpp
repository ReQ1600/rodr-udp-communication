#pragma once
#include <string>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <iostream>

#include "rodrUtils.hpp"

#pragma comment(lib, "ws2_32.lib")
//if vsc add "-lws2_32" to tasks.json/args

namespace rodr
{
    namespace udp
    {
        constexpr unsigned int DEFAULT_BUFFER_SIZE = 64; 

        class UDP
        {
        public:
        UDP(const char* src_ip, const char* remote_ip, u_short local_port, u_short remote_port);
        ~UDP();

        void SendMsg(const char* msg) const;
        void ReceiveAndHandle(rodr::handler handler_function) const;
        void ReceiveAndHandle(char* buffer, unsigned int buffer_size, rodr::handler handler_function) const;

        private:
        const char* msg_;

        SOCKET socket_;
        sockaddr_in remote_;
        sockaddr_in local_;

        WSAData data_;
        };
    }
}
