# Monitoring Network Connectivity
The `NetworkMonitor` provides an easy to use API to monitor for changes in the system's network connectivity.

Take the following program that checks the network status on startup and watches for changes:
```cpp
void onNetChanged(const NetworkStateChangedEventArgs& e)
{
    std::cout << "Network state changed: " << (int)e.getState() << std::endl;
}

int main()
{
    NetworkMonitor netmon;
    netmon.stateChanged() += onNetChanged;
    //Have to check initial state
    if(netmon.getConnectedState() == NetworkState::ConnectedGlobal)
    {
        std::cout << "Connected to internet on startup." << std::endl;
    }
    else
    {
        std::cout << "No connectivity on startup." << std::endl;
    }
    std::cout << "Listening for changes..." << std::endl;
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
```