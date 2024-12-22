#include "app/interprocesscommunicator.h"
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include "helpers/stringhelpers.h"

using namespace Nickvision::Helpers;
using namespace Nickvision::Network;

namespace Nickvision::App
{
    InterProcessCommunicator::InterProcessCommunicator(const std::string& id)
        : m_serverRunning{ false },
#ifdef _WIN32
        m_path{ L"\\\\.\\pipe\\" + StringHelpers::wstr(id) }
#else
        m_id{ id }
#endif
    {
#ifdef _WIN32
        m_serverPipe = nullptr;
        WIN32_FIND_DATAW fd;
        HANDLE find{ FindFirstFileW(m_path.c_str(), &fd) };
        if (find == INVALID_HANDLE_VALUE) //no server exists
        {
            m_serverPipe = CreateNamedPipeW(m_path.c_str(), PIPE_ACCESS_INBOUND | FILE_FLAG_OVERLAPPED, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, 1, 0, 0, NMPWAIT_USE_DEFAULT_WAIT, nullptr);
            if (m_serverPipe == INVALID_HANDLE_VALUE)
            {
                throw std::runtime_error("Unable to start IPC server.");
            }
            m_serverRunning = true;
            FindClose(find);
        }
#else
        try
        {
#ifdef __linux__
            m_serverSocket = std::make_unique<Socket>(SocketPurpose::Server, SocketType::SequencedPacket, AddressFamily::Unix, m_id, 0);
#else
            m_serverSocket = std::make_unique<Socket>(SocketPurpose::Server, SocketType::Stream, AddressFamily::Unix, m_id, 0);
#endif
            m_serverRunning = true;
        }
        catch(const std::logic_error&) {  }
        catch(const std::exception&)
        {
            throw;
        }
#endif
        if(m_serverRunning)
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
        m_serverSocket.reset();
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
            HANDLE connectPipe{ CreateFileW(m_path.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr) };
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
            Socket connectionSocket{ SocketPurpose::Client, SocketType::SequencedPacket, AddressFamily::Unix, m_id, 0 };
#else
            Socket connectionSocket{ SocketPurpose::Client, SocketType::Stream, AddressFamily::Unix, m_id, 0 };
#endif
            if(connectionSocket.connect())
            {
                return false;
            }
            for(const std::string& arg : args)
            {
                connectionSocket.sendMessage(arg);
            }
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
#ifdef _WIN32
        std::vector<char> buffer(1024);
#endif
        while (m_serverRunning)
        {
            std::vector<std::string> args;
#ifdef _WIN32
            if (ConnectNamedPipe(m_serverPipe, nullptr))
            {
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
            if(m_serverSocket->connect())
            {
                while(true)
                {
                    std::string message{ m_serverSocket->receiveMessage() };
                    if(!message.empty())
                    {
                        args.push_back(message);
                    }
                    else
                    {
                        break;
                    }
                }
                m_serverSocket->disconnect();
                if(!args.empty())
                {
                    m_commandReceived({ args });
                }
            }
#endif
        }
    }
} 
