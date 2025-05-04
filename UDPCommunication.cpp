#include "UDPCommunication.hpp"

namespace rodr
{
    namespace udp
    {
        //creates and openes the socket
        UDP::UDP(const char* local_ip, const char* remote_ip, u_short local_port, u_short remote_port)
        {
        if (WSAStartup(MAKEWORD(2,2), &data_) != 0)
        {
            std::cerr << "WSAStartup failed with error code: " << WSAGetLastError() << std::endl;
            return;
        }

        int ret;

        //local sock setup
        local_.sin_family = AF_INET;
        ret = inet_pton(local_.sin_family, local_ip, &local_.sin_addr.S_un.S_addr);
        
        if (ret <= 0)
        {
            if (ret == 0) std::cerr << "Invalid local address format: " << local_ip << std::endl; 
            else std::cerr << "inet_pton failed for local with error code: " << WSAGetLastError() << std::endl;

            WSACleanup();
            return;
        }
        
        local_.sin_port = htons(local_port);

        //remote sock setup
        remote_.sin_family = AF_INET;
        ret = inet_pton(remote_.sin_family, remote_ip, &remote_.sin_addr.S_un.S_addr);
        
        if (ret <= 0)
        {
            if (ret == 0) std::cerr << "Invalid remote address format: " << local_ip << std::endl; 
            else std::cerr << "inet_pton failed for remote with error code: " << WSAGetLastError() << std::endl;

            WSACleanup();
            return;
        }
        
        remote_.sin_port = htons(remote_port);

        //opening the socket
        socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (socket_ == INVALID_SOCKET) 
        {
            std::cerr << "Socket creation failed with error code: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return;
        }
        bind(socket_, (sockaddr*) &local_, sizeof(local_));


        }

        UDP::~UDP()
        {
        closesocket(socket_);
        WSACleanup();
        }

        void UDP::SendMsg(const char* msg) const
        {
            int sent_bytes = sendto(socket_, msg, strlen(msg), 0, (sockaddr*) &remote_, sizeof(remote_));
            if (sent_bytes == SOCKET_ERROR) std::cerr << "Send failed with error code: " << WSAGetLastError() << std::endl;
        }

        void UDP::ReceiveAndHandle(rodr::handler handler_function) const
        {
            char buffer[DEFAULT_BUFFER_SIZE] = { 0 };
            int remote_length = sizeof(remote_);

            int received = recvfrom(socket_, buffer, DEFAULT_BUFFER_SIZE, 0, (sockaddr*) &remote_, &remote_length);

            std::cout << buffer << std::endl;

        if (received > 0) 
        {
            std::cout << "UDP Received: " << buffer << std::endl;
            handler_function(buffer);
        }
        else std::cerr << "UDP Receive failed with error code: " << WSAGetLastError() << std::endl;
        }

        //can be given empty function to handle data from buffer outside the class
        inline void UDP::ReceiveAndHandle(char* buffer, unsigned int buffer_size, rodr::handler handler_function) const
        {
            int remote_length = sizeof(remote_);

            int received = recvfrom(socket_, buffer, buffer_size, 0, (sockaddr*) &remote_, &remote_length);

        if (received > 0) 
        {
            std::cout << "UDP Received: " << buffer << std::endl;
            handler_function(buffer);
        }
        else std::cerr << "UDP Receive failed with error code: " << WSAGetLastError() << std::endl;
        }
    }
}