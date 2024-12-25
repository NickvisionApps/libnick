#ifndef SOCKET_H
#define SOCKET_H

#include <memory>
#include <string>
#include <vector>
#ifdef _WIN32
#include <Winsock2.h>
#endif
#include "addressfamily.h"
#include "socketpurpose.h"
#include "sockettype.h"

namespace Nickvision::Network
{
    /**
     * @brief A network socket (an endpoint for communication).
     */
    class Socket
    {
    public:
        /**
         * @brief Constructs a Socket.
         * @param purpose The purpose of the socket
         * @param type The SocketType of the socket
         * @param family The AddressFamily of the socket
         * @param address The address to bind/connect the socket to
         * @param port The port to bind/connect the socket to (Ignored when AddressFamily::Unix is used)
         * @throw std::invalid_argument Thrown if the address is invalid
         * @throw std::logic_error Thrown if the socket cannot be binded to (i.e. A server socket already exists)
         * @throw std::runtime_error Thrown on Windows if winsock cannot be initalized
         * @throw std::runtime_error Thrown if the socket cannot be created or listened
         */
        Socket(SocketPurpose purpose, SocketType type, AddressFamily family, const std::string& address, int port);
        /**
         * @brief Destructs a Socket.
         * @brief This will disconnect from a child socket if disconnect was not already called.
         */
        ~Socket();
        /**
         * @brief Establishes a connection.
         * @brief If the socket's purpose is SocketPurpose::Server, this method will block until a client is connected.
         * @brief Is the socket's purpose is SocketPurpose::Client, this method will connect to the server.
         * @return True if connected, else false
         */
        bool connect();
        /**
         * @brief Closes a connection.
         * @brief If the socket's purpose is SocketPurpose::Server, this method will drop the connection with the client.
         * @brief If the socket's purpose is SocketPurpose::Client, this method will have no effect.
         * @return True if disconnected, else false
         */
        bool disconnect();
        /**
         * @brief Receives a message.
         * @brief connect() must have been called first and have returned true.
         * @brief If the socket's purpose is SocketPurpose::Server, this method will receive a message from the client.
         * @brief If the socket's purpose is SocketPurpose::Client, this method will receive a message from the server.
         * @return The received message
         */
        std::string receiveMessage() const;
        /**
         * @brief Sends a message.
         * @brief connect() must have been called first and have returned true.
         * @brief If the socket's purpose is SocketPurpose::Server, this method will send a message to the client.
         * @brief If the socket's purpose is SocketPurpose::Client, this method will send a message to the server.
         * @param message The message to send
         * @return True if message sent successfully, else false
         */
        bool sendMessage(const std::string& message) const;

    private:
        SocketPurpose m_purpose;
        SocketType m_type;
        AddressFamily m_family;
        std::string m_address;
        int m_port;
#ifdef _WIN32
        SOCKET m_socket;
        SOCKET m_child;
#else
        int m_socket;
        int m_child;
#endif
    };
}

#endif //SOCKET_H
