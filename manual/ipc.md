# Performing Inter-Process Communication
libnick uses named-pipes on Windows and Unix Domain Sockets on Linux and macOS to establish inter process server and client communicators, while abstracting all of that away from the consumer in the easy to use `Nickvision::App::InterProcessCommunicator` API.

Upon creating an `Nickvision::App::InterProcessCommunicator` object, either `isServer()` or `isClient()` will return true depending on whether or not this instance is a server or client respectively. Server instances should register a callback to the `CommandReceived` event to be invoked when clients send commands to the server.

NOTE: Server instance of `Nickvision::App::InterProcessCommunicator` should be kept alive for the entire need for the communicator. This includes when the need to receive client messages is present.

Let's consider an example scenario for using the `Nickvision::App::InterProcessCommunicator`. Assume we have an application where we want the first instance to be considered the main, running instance. Assume that if other instances of said application are started, we want its arguments to be passed to the main instance and then have said other instances be closed. This will allow, for example, a GUI main instance to be manipulated via secondary CLI instances. 

Here's the code for this:
```cpp
using namespace Nickvision::App;
using namespace Nickvision::Events;

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
    ipc.commandReceived() += handleArguments;
    ipc.communicate(modernArgs, true);
}

void handleArguments(const ParamEventArgs<std::vector<std::string>>& args)
{
  ...
}
```
If this program is ran for the first time, `ipc` will be the server instance. The `handleArguments` function will be invoked after `communicate` with its own arguments, as `communicate` still invokes `CommandReceived` even if ipc is the server instance. 

If this program is ran *not* for the first time, its arguments will be sent to the first instance and this instance itself will close (as `true` was passed as the second argument of `communicate`). The first instance's `handleArguments` function will be called as a result of `CommandReceived` being invoked by the ipc server receiving the command. 