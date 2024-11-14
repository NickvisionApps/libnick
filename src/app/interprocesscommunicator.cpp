#include "app/interprocesscommunicator.h"
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include "helpers/stringhelpers.h"
#ifndef _WIN32
#include <unistd.h>
#include <sys/socket.h>
#endif

using namespace Nickvision::Helpers;

namespace Nickvision::App
{
    InterProcessCommunicator::InterProcessCommunicator(const std::string& id)
        : m_serverRunning{ false }
    {
#ifdef _WIN32
        m_path = "\\\\.\\pipe\\" + id;
        m_serverPipe = nullptr;
        WIN32_FIND_DATAW fd;
        std::wstring wPath{ StringHelpers::wstr(m_path) };
        HANDLE find{ FindFirstFileW(wPath.c_str(), &fd) };
        if (find == INVALID_HANDLE_VALUE) //no server exists
        {
            m_serverPipe = CreateNamedPipeW(wPath.c_str(), PIPE_ACCESS_INBOUND | FILE_FLAG_OVERLAPPED, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, 1, 0, 0, NMPWAIT_USE_DEFAULT_WAIT, nullptr);
            if (m_serverPipe == INVALID_HANDLE_VALUE)
            {
                throw std::runtime_error("Unable to start IPC server.");
            }
            m_serverRunning = true;
            FindClose(find);
        }
#else
        m_path = "/tmp/" + id;
#ifdef __linux__
        if (m_path.size() >= 108)
#else
        if (m_path.size() >= 104)
#endif
        {
            throw std::runtime_error("Unable to create IPC server. Application ID is too long.");
        }
        memset(&m_sockaddr, 0, sizeof(m_sockaddr));
        m_sockaddr.sun_family = AF_UNIX;
        strcpy(m_sockaddr.sun_path, m_path.c_str());
#ifdef __linux__
        m_serverSocket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
#else
        m_serverSocket = socket(AF_UNIX, SOCK_STREAM, 0);
#endif
        if (m_serverSocket == -1)
        {
            throw std::runtime_error("Unable to check IPC server.");
        }
        if (bind(m_serverSocket, reinterpret_cast<const struct sockaddr*>(&m_sockaddr), sizeof(m_sockaddr)) == 0) //no server exists
        {
            if (listen(m_serverSocket, 5) == -1)
            {
                throw std::runtime_error("Unable to listen to IPC socket.");
            }
            m_serverRunning = true;
        }
        else
        {
            close(m_serverSocket);
            m_serverSocket = -1;
        }
#endif
        if (m_serverRunning)
        {
            m_server = std::thread(&InterProcessCommunicator::runServer, this);
        }
    }

    InterProcessCommunicator::~InterProcessCommunicator()
    {
        m_serverRunning = false;
#ifdef _WIN32
        if (m_serverPipe)
        {
            CancelSynchronousIo(m_serverPipe);
            CloseHandle(m_serverPipe);
        }
#else
        if (m_serverSocket != -1)
        {
#ifdef __linux__
            int connectSocket{ socket(AF_UNIX, SOCK_SEQPACKET, 0) };
#else
            int connectSocket{ socket(AF_UNIX, SOCK_STREAM, 0) };
#endif
            connect(connectSocket, reinterpret_cast<const struct sockaddr*>(&m_sockaddr), sizeof(m_sockaddr));
            close(connectSocket);
            close(m_serverSocket);
            unlink(m_path.c_str());
        }
#endif
        if(m_server.joinable())
        {
            m_server.join();
        }
    }

    Events::Event<Events::ParamEventArgs<std::vector<std::string>>>& InterProcessCommunicator::commandReceived()
    {
        return m_commandReceived;
    }

    bool InterProcessCommunicator::isServer() const
    {
        return m_serverRunning;
    }

    bool InterProcessCommunicator::isClient() const
    {
        return !m_serverRunning;
    }

    bool InterProcessCommunicator::communicate(const std::vector<std::string>& args, bool exitIfClient)
    {
        if (m_serverRunning)
        {
            m_commandReceived({ args });
            return true;
        }
        if (!args.empty())
        {
#ifdef _WIN32
            std::wstring wPath{ StringHelpers::wstr(m_path) };
            HANDLE connectPipe{ CreateFileW(wPath.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr) };
            if (connectPipe == INVALID_HANDLE_VALUE)
            {
                return false;
            }
            for (const std::string& arg : args)
            {
                WriteFile(connectPipe, arg.c_str(), DWORD(arg.size()), nullptr, nullptr);
            }
            CloseHandle(connectPipe);
#else
#ifdef __linux__
            int connectSocket{ socket(AF_UNIX, SOCK_SEQPACKET, 0) };
#else
            int connectSocket{ socket(AF_UNIX, SOCK_STREAM, 0) };
#endif
            if (connect(connectSocket, reinterpret_cast<const struct sockaddr*>(&m_sockaddr), sizeof(m_sockaddr)) == -1)
            {
                return false;
            }
#ifdef __linux__
            for (const std::string& arg : args)
#else
            for (std::string arg : args)
#endif
            {
#ifndef __linux__
                arg += "\n";
#endif
                if(write(connectSocket, arg.c_str(), arg.size()) == -1)
                {
                    close(connectSocket);
                    return false;
                }
            }
            close(connectSocket);
#endif
        }
        if (exitIfClient)
        {
            std::exit(0);
        }
        return true;
    }

    void InterProcessCommunicator::runServer()
    {
        std::vector<char> buffer(1024);
        while (m_serverRunning)
        {
#ifdef _WIN32
            if (ConnectNamedPipe(m_serverPipe, nullptr))
            {
                std::vector<std::string> args;
                DWORD read;
                do
                {
                    if(!ReadFile(m_serverPipe, &buffer[0], DWORD(buffer.size()), &read, nullptr))
                    {
                        break;
                    }
                    args.push_back({ &buffer[0], read });
                } while (read > 0);
                DisconnectNamedPipe(m_serverPipe);
                m_commandReceived({ args });
            }
#else
            struct sockaddr_un clientAddr;
            memset(&clientAddr, 0, sizeof(clientAddr));
            socklen_t clientAddrLen{ sizeof(clientAddr) };
            int clientSocket{ accept(m_serverSocket, reinterpret_cast<struct sockaddr*>(&clientAddr), &clientAddrLen) };
            if (!m_serverRunning)
            {
                break;
            }
            if (clientSocket != -1)
            {
                ssize_t bytes{ 0 };
#ifdef __linux__
                std::vector<std::string> args;
#else
                std::string info;
#endif
                do
                {
                    bytes = read(clientSocket, &buffer[0], buffer.size());
                    if (bytes > 0)
                    {
#ifdef __linux__
                        args.push_back({ &buffer[0], static_cast<size_t>(bytes) });
#else
                        info += { &buffer[0], static_cast<size_t>(bytes) };
#endif
                    }
                } while (bytes > 0);
                close(clientSocket);
#ifdef __linux__
                m_commandReceived({ args });
#else
                m_commandReceived({ StringHelpers::split(info, "\n") });
#endif
            }
#endif
        }
    }
} 