# Performing Inter-Process Communication
libnick allows for the use of `Nickvision::Network::Socket`s to establish inter-process communication via named pipes on Windows and domain sockets on Linux and macOS.

A process must first create a server socket object that will act as the "receiver" for when other client processes send messages:
```cpp
using namespace Nickvision::Network;

#ifdef _WIN32
Socket receiver{ SocketPurpose::Server, SocketType::Stream, AddressFamily::Pipe, "org.nickvision.libnick.tests", 0 };
#else
Socket receiver{ SocketPurpose::Server, SocketType::Stream, AddressFamily::Unix, "org.nickvision.libnick.tests", 0 };
#endif
while(true)
{
    if(receiver.connect()) // Wait for and connect to new client
    {
        std::string msg{ receiver.receiveMessage() }; // Wait for and receive message from client
        // Handle msg
        receiver.sendMessage("ACK"); // Send a message back to connected client
        receiver.disconnect(); // Close connection with client
    }
    else
    {
        // There was an error connecting to this client
    }
}
```

A child/client process can now create client sockets of the same name to send messages to the receiver:
```cpp
using namespace Nickvision::Network;

#ifdef _WIN32
Socket receiver{ SocketPurpose::Client, SocketType::Stream, AddressFamily::Pipe, "org.nickvision.libnick.tests", 0 };
#else
Socket receiver{ SocketPurpose::Client, SocketType::Stream, AddressFamily::Unix, "org.nickvision.libnick.tests", 0 };
#endif
receiver.connect() // Wait for and connect to server
receiver.sendMessage("Hello") // Send message to server
std::string msg{ receiver.receiveMessage() } // Wait for and receive message from server
// Handle msg
receiver.disconnect() // Close connection with server
```