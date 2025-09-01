/**
 * @file
 * @author Nicholas Logozzo <nlogozzo225@gmail.com>
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * https://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * A monitor of the system's network connection.
 */

#ifndef NETWORKMONITOR_H
#define NETWORKMONITOR_H

#include <mutex>
#include "networkstatechangedeventargs.h"
#include "events/event.h"
#ifdef _WIN32
#include <windows.h>
#include <atlbase.h>
#include <netlistmgr.h>
#endif

namespace Nickvision::Network
{
    /**
     * @brief A monitor of the system's network connection.
     */
#ifdef _WIN32
    class NetworkMonitor : public INetworkListManagerEvents
#else
    class NetworkMonitor
#endif
    {
    public:
        /**
         * @brief Constructs a NetworkMonitor. This method will call checkConnectionState() to get the initial system network state.
         * @throw std::runtime_error Thrown if unable to create the NetworkMonitor
         */
        NetworkMonitor();
        /**
         * @brief Destructs a NetworkMonitor. 
         */
        ~NetworkMonitor() noexcept;
        /**
         * @brief Gets the StateChanged event. This event is invoked whenever the state of the network connection changes.
         * @return The StateChanged event
         */
        Events::Event<NetworkStateChangedEventArgs>& stateChanged() noexcept;
        /**
         * @brief Gets the state of the network connection.
         * @return NetworkState
         */
        NetworkState getConnectionState() const noexcept;
#ifdef _WIN32
        ULONG STDMETHODCALLTYPE AddRef() override;
        ULONG STDMETHODCALLTYPE Release() override;
        HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override;
        HRESULT STDMETHODCALLTYPE ConnectivityChanged(NLM_CONNECTIVITY newConnectivity) override;
        HRESULT STDMETHODCALLTYPE NetworkConnectionPropertyChanged(GUID connectionId, NLM_CONNECTION_PROPERTY_CHANGE newProperty);
#endif

    private:
        /**
         * @brief Manually checks the state of the system's network connection. If a change is detected, the StateChanged event will be invoked.
         */
        void checkConnectionState() noexcept;
        mutable std::mutex m_mutex;
        Events::Event<NetworkStateChangedEventArgs> m_stateChanged;
        NetworkState m_connectionState;
#ifdef _WIN32
        CComPtr<INetworkListManager> m_netListManager;
        CComPtr<IConnectionPoint> m_connectionPoint;
        DWORD m_cookie;
#else
        unsigned long m_networkChangedHandlerId;
#endif
    };
}

#endif //NETWORKMONITOR_H