#include "network/socket.h"
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#ifndef _WIN32
#include <unistd.h>
#include <netinet/in.h>
#include <sys/un.h>
#endif
#include "network/ipv4address.h"

#ifndef _WIN32
#ifdef __linux__
    #define MAX_UNIX_PATH_LENGTH 108
#else
    #define MAX_UNIX_PATH_LENGTH 104
#endif
#endif
#define BACKLOG 5

namespace Nickvision::Network
{
#ifdef _WIN32
    static bool isSocketValid(SOCKET socket)
    {
        return socket != INVALID_SOKCET;
    }
#else
    static bool isSocketValid(int socket)
    {
        return socket != -1;
    }
#endif

    Socket::Socket(SocketPurpose purpose, SocketType type, AddressFamily family, const std::string& address, int port)
        : m_purpose{ purpose },
        m_type{ type },
        m_family{ family },
        m_child{ -1 }
    {
#ifdef _WIN32
        //Check if winsock is initalized
        static bool winsockInitialized{ false };
        if(!winsockInitialized)
        {
            WSADATA wsaData;
            if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
            {
                throw std::runtime_error("Unable to initalize winsock");
            }
            winsockInitialized = true;
        }
#endif
        //Create the socket
        m_socket = socket(static_cast<int>(family), static_cast<int>(type), 0);
        if(!isSocketValid(m_socket))
        {
            throw std::runtime_error("Unable to create socket");
        }
        //Create the address struct
        switch(m_family)
        {
#ifndef _WIN32
            case AddressFamily::Unix:
            {
                std::string path{ "/tmp/" + address };
                path.resize(MAX_UNIX_PATH_LENGTH);
                struct sockaddr_un* addr = new struct sockaddr_un();
                addr->sun_family = static_cast<sa_family_t>(family);
                strcpy(addr->sun_path, path.c_str());
                m_address = reinterpret_cast<struct sockaddr*>(addr);
                break;
            }
#endif
            case AddressFamily::IPv4:
            {
                std::optional<IPv4Address> ipv4{ IPv4Address::parse(address) };
                if(!ipv4)
                {
                    throw std::invalid_argument("Invalid IPv4 Address");
                }
                struct sockaddr_in* addr = new struct sockaddr_in();
                addr->sin_family = static_cast<sa_family_t>(family);
                addr->sin_port = port;
#ifdef _WIN32
                addr->sin_addr.S_un_b.s_b1 = ipv4->getFirst();
                addr->sin_addr.S_un_b.s_b2 = ipv4->getSecond();
                addr->sin_addr.S_un_b.s_b3 = ipv4->getThird();
                addr->sin_addr.S_un_b.s_b4 = ipv4->getFourth();
#else
                addr->sin_addr.s_addr = ipv4->getNetworkByteOrder();
#endif
                m_address = reinterpret_cast<struct sockaddr*>(addr);
                break;
            }
        }
        //Bind and listen if server socket
        if(m_purpose == SocketPurpose::Server)
        {
            if(bind(m_socket, m_address, sizeof(m_address)) != 0)
            {
                throw std::runtime_error("Unable to bind socket");
            }
            if(listen(m_socket, BACKLOG) != 0)
            {
                throw std::runtime_error("Unable to listen on socket");
            }
        }
    }

    Socket::~Socket()
    {
        //Disconnect from child
        disconnect();
        //Close the socket
#ifdef _WIN32
        closesocket(m_socket);
#else
        close(m_socket);
#endif
        //Cleanup the address struct
        if(m_address)
        {
            switch(m_family)
            {
#ifndef _WIN32
            case AddressFamily::Unix:
                delete reinterpret_cast<struct sockaddr_un*>(m_address);
                break;
#endif
            case AddressFamily::IPv4:
                delete reinterpret_cast<struct sockaddr_in*>(m_address);
                break;
            }
        }
    }

    bool Socket::connect()
    {
        if(m_purpose == SocketPurpose::Server)
        {
#ifdef _WIN32
            m_child = accept(m_socket, nullptr, nullptr);
#else
            m_child = accept(m_socket, nullptr, nullptr);
#endif
            return isSocketValid(m_child);
        }
        else if(m_purpose == SocketPurpose::Client)
        {
            if(::connect(m_socket, m_address, sizeof(m_address)) != 0)
            {
                return false;
            }
            return true;
        }
        return false;
    }

    bool Socket::disconnect()
    {
        if(!isSocketValid(m_child))
        {
            return false;
        }
#ifdef _WIN32
        closesocket(m_child);
        m_child = INVLAID_SOCKET;
#else
        close(m_child);
        m_child = -1;
#endif
        return true;
    }

    std::string Socket::receiveMessage() const
    {
#ifdef _WIN32
        SOCKET socket{ m_socket };
#else
        int socket{ m_socket };
#endif
        if(m_purpose == SocketPurpose::Server && isSocketValid(m_child))
        {
            socket = m_child;
        }
        std::vector<char> buffer(1024);
        std::string message;
#ifdef _WIN32
        int bytes{ 0 };
#else
        ssize_t bytes{ 0 };
#endif
        do
        {
            bytes = recv(socket, &buffer[0], buffer.size(), 0);
            if(bytes > 0)
            {
                message += std::string(&buffer[0], static_cast<size_t>(bytes));
            }
        } while(bytes > 0);
        return message;
    }

    bool Socket::sendMessage(const std::string& message) const
    {
#ifdef _WIN32
        SOCKET socket{ m_socket };
#else
        int socket{ m_socket };
#endif
        if(m_purpose == SocketPurpose::Server && isSocketValid(m_child))
        {
            socket = m_child;
        }
        return static_cast<size_t>(send(socket, message.c_str(), message.size(), 0)) == message.size();
    }
}
