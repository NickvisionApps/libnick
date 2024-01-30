#include "app/interprocesscommunicator.h"
#include <cstdlib>
#include <stdexcept>
#ifdef __linux__
#include <unistd.h>
#include <sys/socket.h>
#endif

namespace Nickvision::App
{
    InterProcessCommunicator::InterProcessCommunicator(const std::string& id)
        : m_serverRunning{ false }
    {
#ifdef _WIN32
        m_path = "\\\\.\\pipe\\" + id;
        m_serverPipe = nullptr;
        WIN32_FIND_DATAA fd;
        HANDLE find{ FindFirstFileA(m_path.c_str(), &fd) };
        if (find == INVALID_HANDLE_VALUE) //no server exists
        {
            m_serverPipe = CreateNamedPipeA(m_path.c_str(), PIPE_ACCESS_INBOUND | FILE_FLAG_OVERLAPPED, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, 1, 0, 0, NMPWAIT_USE_DEFAULT_WAIT, nullptr);
            if (m_serverPipe == INVALID_HANDLE_VALUE)
            {
                throw std::runtime_error("Unable to start IPC server.");
            }
            m_serverRunning = true;
            FindClose(find);
        }
#elif defined(__linux__)
        m_path = "/tmp/" + id;
        if (m_path.size() >= 108)
        {
            throw std::runtime_error("Unable to create IPC server. Application ID is too long. Must be < 103 characters.");
        }
        memset(&m_sockaddr, 0, sizeof(m_sockaddr));
        m_sockaddr.sun_family = AF_UNIX;
        strcpy(m_sockaddr.sun_path, m_path.c_str());
        m_serverSocket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
        if (m_serverSocket == -1)
        {
            throw std::runtime_error("Unable to check IPC server.");
        }
        if (bind(m_serverSocket, (const struct sockaddr*)&m_sockaddr, sizeof(m_sockaddr)) != -1) //no server exists
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
            m_server = std::jthread(&InterProcessCommunicator::runServer, this);
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
#elif defined(__linux__)
        if (m_serverSocket != -1)
        {
            int clientSocket{ socket(AF_UNIX, SOCK_SEQPACKET, 0) };
            connect(clientSocket, (const struct sockaddr*)&m_sockaddr, sizeof(m_sockaddr));
            close(clientSocket);
            close(m_serverSocket);
            unlink(m_path.c_str());
        }
#endif
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
            std::string argc{ std::to_string(args.size()) };
#ifdef _WIN32
            HANDLE clientPipe{ CreateFileA(m_path.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr) };
            if (clientPipe == INVALID_HANDLE_VALUE)
            {
                return false;
            }
            WriteFile(clientPipe, argc.c_str(), DWORD(argc.size()), nullptr, nullptr);
            for (const std::string& arg : args)
            {
                WriteFile(clientPipe, arg.c_str(), DWORD(arg.size()), nullptr, nullptr);
            }
            CloseHandle(clientPipe);
#elif defined(__linux__)
            int clientSocket{ socket(AF_UNIX, SOCK_SEQPACKET, 0) };
            if (connect(clientSocket, (const struct sockaddr*)&m_sockaddr, sizeof(m_sockaddr)) == -1)
            {
                return false;
            }
            write(clientSocket, argc.c_str(), argc.size());
            for (const std::string& arg : args)
            {
                write(clientSocket, arg.c_str(), arg.size());
            }
            close(clientSocket);
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
                DWORD read;
                ReadFile(m_serverPipe, &buffer[0], DWORD(buffer.size()), &read, nullptr);
                std::vector<std::string> args(std::stoull({ &buffer[0], read }));
                for (size_t i = 0; i < args.size(); i++)
                {
                    ReadFile(m_serverPipe, &buffer[0], DWORD(buffer.size()), &read, nullptr);
                    args[i] = { &buffer[0], read };
                }
                m_commandReceived({ args });
                DisconnectNamedPipe(m_serverPipe);
            }
#elif defined(__linux__)
            int clientSocket{ accept(m_serverSocket, nullptr, nullptr) };
            if (!m_serverRunning)
            {
                break;
            }
            if (clientSocket != -1)
            {
                ssize_t r{ read(clientSocket, &buffer[0], buffer.size()) };
                std::vector<std::string> args(std::stoull({ &buffer[0], static_cast<size_t>(r < 0 ? 0 : r) }));
                for (size_t i = 0; i < args.size(); i++)
                {
                    r = read(clientSocket, &buffer[0], buffer.size());
                    args[i] = { &buffer[0], static_cast<size_t>(r < 0 ? 0 : r) };
                }
                m_commandReceived({ args });
                close(clientSocket);
            }
#endif
        }
    }
} 