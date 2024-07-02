# Performing Inter-Process Communication
libnick uses named-pipes on Windows and Unix Domain Sockets on Linux to establish inter process server and client communicators, while abstracting all of that away from the consumer in the easy to use `InterProcessCommunicator` API.

Upon creating an `InterProcessCommunicator` object, either `isServer()` or `isClient()` will return true depending on whether or not this instance is a server or client respectively. Server instances should register a callback to the `CommandReceived` event to be invoked when clients send commands to the server.

NOTE: Whether or not `InterProcessCommunicator` is a client or server, its object should be kept alive for the entire running of an application to avoid accidentally removing a process as the server instance if the object is destroyed midway of an application running.

Let's consider an example scenario for using the `InterProcessCommunicator`. Assume we have an application where we want the first instance to be considered the main, running instance. Assume that if other instances of said application are started, we want its arguments to be passed to the main instance and then have said other instances be closed. This will allow, for example, a GUI main instance to be manipulated via secondary CLI instances. 

Here's the code for this:
```cpp
using namespace Nickvision::App;

int main(int argc, char*[] argv)
{
    std::vector<std::string> modernArgs;
    for(int i = 0; i < argc; i++)
    {
        if(argv[i])
        {
            modernArgs.push_back({ argv[i] });
        }
    }
    InterProcessCommunicator& ipc{ "appid" };
    ipc.commandReceived() += [](const Events::ParamEventArgs<std::vector<std::string>>& args) { ... };
    ipc.communicate(modernArgs, true);
}
```
If this program is ran for the first time, ipc will be the server instance. `handleArguments` will be invoked after `communicate` with its own arguments, as `communicate` still invokes `CommandReceived` even if ipc is the server instance. 

If this program is ran not for the first time, its arguments will be sent to the first instance and this instance itself will close. The first instance's `handleArguments` function will be called as a result of `CommandReceived` being invoked by the ipc server receiving the command. 