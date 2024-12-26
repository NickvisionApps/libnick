#include "network/socket.h"
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>
#endif
#include "helpers/stringhelpers.h"
#include "network/ipv4address.h"

#ifndef _WIN32
#ifdef __linux__
    #define MAX_UNIX_PATH_LENGTH 108
#else
    #define MAX_UNIX_PATH_LENGTH 104
#endif
#endif
#define BACKLOG 5

using namespace Nickvision::Helpers;

namespace Nickvision::Network
{
#ifdef _WIN32
    static bool isSocketValid(SOCKET socket)
    {
        return socket != INVALID_SOCKET;
    }

    static bool isSocketValid(HANDLE pipe)
    {
        return pipe != INVALID_HANDLE_VALUE;
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
        m_address{ address },
        m_port{ port }
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
        //Create pipe
        if(m_family == AddressFamily::Pipe)
        {
            if(m_purpose == SocketPurpose::Server)
            {
                std::wstring path{ L"\\\\.\\pipe\\" + StringHelpers::wstr(m_address) };
                WIN32_FIND_DATAW fd;
                HANDLE find{ FindFirstFileW(path.c_str(), &fd) };
                if(find == INVALID_HANDLE_VALUE)
                {
                    m_pipe = CreateNamedPipeW(path.c_str(), PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, 1, 0, 0, NMPWAIT_USE_DEFAULT_WAIT, nullptr);
                    if(m_pipe == INVALID_HANDLE_VALUE)
                    {
                        throw std::runtime_error("Unable to create pipe");
                    }
                    FindClose(find);
                }
                else
                {
                    throw std::logic_error("Unable to create pipe. Server already exists");
                }
            }
            return;
        }
#endif
        //Create the socket
        m_socket = socket(static_cast<int>(m_family), static_cast<int>(m_type), 0);
        if(!isSocketValid(m_socket))
        {
            throw std::runtime_error("Unable to create socket");
        }
        //Create the address struct
        int bindResult{ 0 };
        switch(m_family)
        {
#ifndef _WIN32
            case AddressFamily::Unix:
            {
                std::string domainPath{ "/tmp/" + m_address + ".socket" };
                domainPath.resize(MAX_UNIX_PATH_LENGTH);
                struct sockaddr_un addr;
                memset(&addr, 0, sizeof(addr));
                addr.sun_family = AF_UNIX;
                strcpy(addr.sun_path, domainPath.c_str());
                bindResult = bind(m_socket, reinterpret_cast<const struct sockaddr*>(&addr), sizeof(addr));
                break;
            }
#endif
            case AddressFamily::IPv4:
            {
                std::optional<IPv4Address> ipv4{ IPv4Address::parse(m_address) };
                if(!ipv4)
                {
                    throw std::invalid_argument("Invalid IPv4 Address");
                }
                struct sockaddr_in addr;
                memset(&addr, 0, sizeof(addr));
                addr.sin_family = AF_INET;
                addr.sin_port = m_port;
#ifdef _WIN32
                addr.sin_addr.S_un.S_un_b.s_b1 = ipv4->getFirst();
                addr.sin_addr.S_un.S_un_b.s_b2 = ipv4->getSecond();
                addr.sin_addr.S_un.S_un_b.s_b3 = ipv4->getThird();
                addr.sin_addr.S_un.S_un_b.s_b4 = ipv4->getFourth();
#else
                addr.sin_addr.s_addr = ipv4->getNetworkByteOrder();
#endif
                bindResult = bind(m_socket, reinterpret_cast<const struct sockaddr*>(&addr), sizeof(addr));
                break;
            }
        }
        //Bind and listen if server socket
        if(m_purpose == SocketPurpose::Server)
        {
            if(bindResult != 0)
            {
#ifdef _WIN32
                closesocket(m_socket);
#else
                close(m_socket);
#endif
                throw std::logic_error("Unable to bind socket. Server already exists");
            }
            if(listen(m_socket, BACKLOG) != 0)
            {
#ifdef _WIN32
                closesocket(m_socket);
#else
                close(m_socket);
#endif
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
        if(isSocketValid(m_socket))
        {
            shutdown(m_socket, SD_BOTH);
            closesocket(m_socket);
        }
        if(isSocketValid(m_pipe))
        {
            if(m_purpose == SocketPurpose::Server)
            {
                CancelSynchronousIo(m_pipe);
            }
            CloseHandle(m_pipe);
        }
#else
        shutdown(m_socket, SHUT_RDWR);
        close(m_socket);
        if(m_family == AddressFamily::Unix)
        {
            std::string domainPath{ "/tmp/" + m_address + ".socket" };
            domainPath.resize(MAX_UNIX_PATH_LENGTH);
            unlink(domainPath.c_str());
        }
#endif
    }

    bool Socket::connect()
    {        
        if(m_purpose == SocketPurpose::Server)
        {
#ifdef _WIN32
            if(m_family == AddressFamily::Pipe)
            {
                return ConnectNamedPipe(m_pipe, nullptr);
            }
#endif
            m_child = accept(m_socket, nullptr, nullptr);
            return isSocketValid(m_child);
        }
        else if(m_purpose == SocketPurpose::Client)
        {
            switch(m_family)
            {
#ifdef _WIN32
                case AddressFamily::Pipe:
                {
                    std::wstring path{ L"\\\\.\\pipe\\" + StringHelpers::wstr(m_address) };
                    m_pipe = CreateFileW(path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
                    return isSocketValid(m_pipe);
                }
#else
                case AddressFamily::Unix:
                {
                    std::string domainPath{ "/tmp/" + m_address + ".socket" };
                    domainPath.resize(MAX_UNIX_PATH_LENGTH);
                    struct sockaddr_un address;
                    memset(&address, 0, sizeof(address));
                    address.sun_family = AF_UNIX;
                    strcpy(address.sun_path, domainPath.c_str());
                    return ::connect(m_socket, reinterpret_cast<const struct sockaddr*>(&address), sizeof(address)) == 0;
                }
#endif
                case AddressFamily::IPv4:
                {
                    std::optional<IPv4Address> ipv4{ IPv4Address::parse(m_address) };
                    if(!ipv4)
                    {
                        throw std::invalid_argument("Invalid IPv4 Address");
                    }
                    struct sockaddr_in address;
                    memset(&address, 0, sizeof(address));
                    address.sin_family = AF_INET;
                    address.sin_port = m_port;
#ifdef _WIN32
                    address.sin_addr.S_un.S_un_b.s_b1 = ipv4->getFirst();
                    address.sin_addr.S_un.S_un_b.s_b2 = ipv4->getSecond();
                    address.sin_addr.S_un.S_un_b.s_b3 = ipv4->getThird();
                    address.sin_addr.S_un.S_un_b.s_b4 = ipv4->getFourth();
#else
                    address.sin_addr.s_addr = ipv4->getNetworkByteOrder();
#endif
                    return ::connect(m_socket, reinterpret_cast<const struct sockaddr*>(&address), sizeof(address)) == 0;
                }
            }
        }
        return false;
    }

    bool Socket::disconnect()
    {
#ifdef _WIN32
        if(m_family == AddressFamily::Pipe && m_purpose == SocketPurpose::Server)
        {
            return DisconnectNamedPipe(m_pipe);
        }
#endif
        if(!isSocketValid(m_child))
        {
            return false;
        }
#ifdef _WIN32
        closesocket(m_child);
        m_child = INVALID_SOCKET;
#else
        close(m_child);
        m_child = -1;
#endif
        return true;
    }

    std::string Socket::receiveMessage() const
    {
        std::vector<char> buffer(1024);
        std::string message;
#ifdef _WIN32
        if(m_family == AddressFamily::Pipe)
        {
            DWORD bytes;
            do
            {
                if(!ReadFile(m_pipe, &buffer[0], DWORD(buffer.size()), &bytes, nullptr))
                {
                    break;
                }
                message += std::string(&buffer[0], static_cast<size_t>(bytes));
            } while (bytes > 0);
            return message;
        }
        SOCKET socket{ m_socket };
#else
        int socket{ m_socket };
#endif
        if(m_purpose == SocketPurpose::Server && isSocketValid(m_child))
        {
            socket = m_child;
        }
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
        if(m_family == AddressFamily::Pipe)
        {
            return WriteFile(m_pipe, message.c_str(), DWORD(message.size()), nullptr, nullptr);
        }
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
