# Nickvision::Aura::Network

This module contains objects for managing a system's network state.

## Table of Contents
- [NetworkMonitor](#networkmonitor)
- [NetworkState](#networkstate)
- [NetworkStateChangedEventArgs](#networkstatechangedeventargs)

## NetworkMonitor
Description: An object to monitor the state of the system's network connection.

Interface: [networkmonitor.h](/include/network/networkmonitor.h)

Type: `class`

Path: `Nickvision::Aura::Network::NetworkMonitor`

### Member Variables
- ```
  Nickvision::Aura::Network::NetworkState ConnectionState: get
  ```
    - The state of the system's network connection.

### Events
- ```
  Event<Nickvision::Aura::Network::NetworkStateChangedEventArgs> StateChanged
  ```
    - Invoked when the system's network state is changed.

### Methods
- ```cpp
  NetworkMonitor()
  ```
    - Constructs a NetworkMonitor.
- ```cpp
  ~NetworkMonitor()
  ```
    - Destructs a NetworkMonitor.
- ```cpp
  void checkConnectionState()
  ```
    - Manually checks the state of the system's network connection. 
    - Note: If a change is detected, the StateChanged event will be invoked.

### Monitoring Network Connectivity
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

## NetworkState
Description: States of a network connection. 

Interface: [networkstate.h](/include/network/networkstate.h)

Type: `enum class`

Path: `Nickvision::Aura::Network::NetworkState`

### Values
- ```
  Disconnected = 0
  ```
    - Represents no network connectivity.
- ```
  ConnectedLocal = 1
  ```
    - Represents local network connectivity, but not global. Access to Internet not available.
- ```
  ConnectedGlobal = 2
  ```
    - Represents full network & internet connectivity.

## NetworkStateChangedEventArgs
Description: Event args for when the network state is changed.

Interface: [networkstatechangedeventargs.h](/include/network/networkstatechangedeventargs.h)

Type: `class`

Path: `Nickvision::Aura::Network::NetworkStateChangedEventArgs`

## Member Variables
- ```
  NetworkState State: get
  ```
    - The new state of the network.

## Methods
- ```cpp
  NetworkStateChangedEventArgs(NetworkState state)
  ```
    - Constructs a NetworkStateChangedEventArgs.
    - Accepts: The new NetworkState for the system, state.