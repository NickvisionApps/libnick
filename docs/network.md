# Nickvision::Network

This module contains objects for managing a system's network state.

## Table of Contents
- [CurlEasy](#curleasy)
- [NetworkMonitor](#networkmonitor)
- [NetworkState](#networkstate)
- [NetworkStateChangedEventArgs](#networkstatechangedeventargs)
- [WebClient](#webclient)

## CurlEasy
Description: An object for making easy curl requests.

Interface: [curleasy.h](/include/network/curleasy.h)

Type: `class`

Path: `Nickvision::Network::CurlEasy`

### Member Variables
- ```
  std::string Url: get, set
  ```
    - The url to make requests to.
- ```
  bool NoBody: get, set
  ```
    - Whether or not to include the body in the response.
- ```
  std::vector<std::string> Headers: get, set
  ```
    - The headers to include in the request.
- ```
  std::string UserAgent: get, set
  ```
    - The user agent to use in the request.
- ```
  std::basic_ostream<char>* Stream: get, set
  ```
    - The stream to write the response to.
- ```
  std::function<int(curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)> Progress: get, set
  ```
    - The progress function to use during the request.

### Methods
- ```cpp
  WebClient(const std::string& url)
  ```
    - Constructs a CurlEasy.
    - Accepts: An optional url to make requests to, url.
    - Throws: std::runtime_error if curl fails to initialize. 
- ```cpp
  ~WebClient()
  ```
    - Destructs a CurlEasy.
- ```cpp
  void reset(const std::string& url)
  ```
    - Accept: An optional url to make requests to on the reset handle, url.
    - Throws: std::runtime_error if curl fails to initialize. 
- ```cpp
  CURLcode perform()
  ```
    - Returns: The code returned by the performed the curl request.

## NetworkMonitor
Description: An object to monitor the state of the system's network connection.

Interface: [networkmonitor.h](/include/network/networkmonitor.h)

Type: `class`

Path: `Nickvision::Network::NetworkMonitor`

### Member Variables
- ```
  Nickvision::Network::NetworkState ConnectionState: get
  ```
    - The state of the system's network connection.

### Events
- ```cpp
  Event<Nickvision::Network::NetworkStateChangedEventArgs> StateChanged
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

Path: `Nickvision::Network::NetworkState`

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

Path: `Nickvision::Network::NetworkStateChangedEventArgs`

### Member Variables
- ```
  NetworkState State: get
  ```
    - The new state of the network.

### Methods
- ```cpp
  NetworkStateChangedEventArgs(NetworkState state)
  ```
    - Constructs a NetworkStateChangedEventArgs.
    - Accepts: The new NetworkState for the system, state.

## WebClient
Description: An object for interacting with the web.

Interface: [webclient.h](/include/network/webclient.h)

Type: `class`

Path: `Nickvision::Network::WebClient`

### Methods
- ```cpp
  WebClient()
  ```
    - Constructs a WebClient.
- ```cpp
  ~WebClient()
  ```
    - Destructs a WebClient.
- ```cpp
  bool getWebsiteExists(const std::string& url)
  ```
    - Accepts: The url to check if it exists (i.e. points to a valid domain) or not, url.
    - Returns: True if url exists.
    - Returns: False if url does not exist.
- ```cpp
  std::string fetchJson(const std::string& url)
  ```
    - Accepts: The url to request a json string from, url.
    - Returns: The fetched json string
- ```cpp
  bool downloadFile(const std::string& url, const std::filesystem::path& path, const std::function<int(curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)>& progress, bool overwrite)
  ```
    - Accepts: The url of the file to download, url, the path to save the downloaded file to on disk, path, an optional function to track download progress, progress, and whether or not to overwrite existing files on disk, overwrite.
    - Returns: True if the file was downloaded and saved successfully.
    - Returns: False if the file was not downloaded successfully.