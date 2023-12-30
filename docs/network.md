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