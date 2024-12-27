#include "app/interprocesscommunicator.h"
#include <vector>
#include "helpers/stringhelpers.h"

using namespace Nickvision::Helpers;
using namespace Nickvision::Network;

namespace Nickvision::App
{
    InterProcessCommunicator::InterProcessCommunicator(const std::string& id)
        : m_id{ id }, 
        m_serverRunning{ false }
    {
        try
        {
#ifdef _WIN32
            m_serverSocket = std::make_unique<Socket>(SocketPurpose::Server, SocketType::Stream, AddressFamily::Pipe, m_id, 0);
#elif defined(__linux__)
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
        if(m_serverRunning)
        {
            m_server = std::thread(&InterProcessCommunicator::runServer, this);
        }
    }

    InterProcessCommunicator::~InterProcessCommunicator()
    {
        m_serverRunning = false;
        m_serverSocket.reset();
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
            Socket connectionSocket{ SocketPurpose::Client, SocketType::Stream, AddressFamily::Pipe, m_id, 0 };
#elif defined(__linux__)
            Socket connectionSocket{ SocketPurpose::Client, SocketType::SequencedPacket, AddressFamily::Unix, m_id, 0 };
#else
            Socket connectionSocket{ SocketPurpose::Client, SocketType::Stream, AddressFamily::Unix, m_id, 0 };
#endif
            if(!connectionSocket.connect())
            {
                return false;
            }
            std::string msg{ StringHelpers::join(args, "\n") };
            if(!connectionSocket.sendMessage(msg))
            {
                return false;
            }
        }
        if (exitIfClient)
        {
            std::exit(0);
        }
        return true;
    }

    void InterProcessCommunicator::runServer()
    {
        while (m_serverRunning)
        {
            if(m_serverSocket->connect())
            {
                std::vector<std::string> args;
                while(true)
                {
                    std::string message{ m_serverSocket->receiveMessage() };
                    if(!message.empty())
                    {
                        for(const std::string& arg : StringHelpers::split(message, "\n"))
                        {
                            args.push_back(arg);
                        }
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
        }
    }
} 
