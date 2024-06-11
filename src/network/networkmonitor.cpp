#include "network/networkmonitor.h"
#include <stdexcept>
#include "helpers/stringhelpers.h"
#include "system/environment.h"
#ifdef __linux__
#include <gio/gio.h>
#endif

using namespace Nickvision::Helpers;
using namespace Nickvision::System;

namespace Nickvision::Network
{
    NetworkMonitor::NetworkMonitor()
        : m_connectionState{ NetworkState::Disconnected },
#ifdef _WIN32
        m_netListManager{ nullptr },
        m_connectionPoint{ nullptr },
        m_cookie{ 0 }
#elif defined(__linux__)
        m_networkChangedHandlerId{ 0 }
#endif
    {
#ifdef _WIN32
        CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        CComPtr<IConnectionPointContainer> connectionPointContainer{ nullptr };
        CComPtr<IUnknown> sink{ nullptr };
        if (CoCreateInstance(CLSID_NetworkListManager, nullptr, CLSCTX_ALL, __uuidof(INetworkListManager), (LPVOID*)&m_netListManager) != S_OK)
        {
            throw std::runtime_error("Unable to create network list manager.");
        }
        if (m_netListManager->QueryInterface(IID_PPV_ARGS(&connectionPointContainer)) != S_OK)
        {
            throw std::runtime_error("Unable to create connection point container.");
        }
        if (connectionPointContainer->FindConnectionPoint(IID_INetworkListManagerEvents, &m_connectionPoint) != S_OK)
        {
            throw std::runtime_error("Unable to find connection point.");
        }
        if (QueryInterface(IID_IUnknown, reinterpret_cast<void**>(&sink)) != S_OK)
        {
            throw std::runtime_error("Unable to get sink pointer.");
        }
        if (m_connectionPoint->Advise(sink, &m_cookie) != S_OK)
        {
            throw std::runtime_error("Unable to get advice connection point.");
        }
#elif defined(__linux__)
        m_networkChangedHandlerId = g_signal_connect_data(G_OBJECT(g_network_monitor_get_default()), "network-changed", G_CALLBACK((void(*)(GNetworkMonitor*, bool, void*))([](GNetworkMonitor*, bool, void* data)
        {
            static_cast<NetworkMonitor*>(data)->checkConnectionState();
        })), this, nullptr, G_CONNECT_DEFAULT);
        if (m_networkChangedHandlerId <= 0)
        {
            throw std::runtime_error("Unable to connect to network monitor signal.");
        }
#endif
        checkConnectionState();
    }

    NetworkMonitor::~NetworkMonitor()
    {
#ifdef _WIN32
        m_connectionPoint->Unadvise(m_cookie);
#elif defined(__linux__)
        g_signal_handler_disconnect(G_OBJECT(g_network_monitor_get_default()), m_networkChangedHandlerId);
#endif
    }

    Events::Event<NetworkStateChangedEventArgs>& NetworkMonitor::stateChanged()
    {
        return m_stateChanged;
    }

    NetworkState NetworkMonitor::getConnectionState() const
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return m_connectionState;
    }

    void NetworkMonitor::checkConnectionState()
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        NetworkState newState{ NetworkState::Disconnected };
        std::string noNetCheck{ StringHelpers::lower(Environment::getVariable("AURA_DISABLE_NETCHECK")) };
        if (!noNetCheck.empty() && (noNetCheck == "true" || noNetCheck == "t" || noNetCheck == "yes" || noNetCheck == "y" || noNetCheck == "1"))
        {
            newState = NetworkState::ConnectedGlobal;
        }
        else
        {
#ifdef _WIN32
            NLM_CONNECTIVITY connection{ NLM_CONNECTIVITY_DISCONNECTED };
            if (m_netListManager->GetConnectivity(&connection) == S_OK)
            {
                if (connection == NLM_CONNECTIVITY_DISCONNECTED)
                {
                    newState = NetworkState::Disconnected;
                }
                else if (connection & NLM_CONNECTIVITY_IPV4_INTERNET || connection & NLM_CONNECTIVITY_IPV6_INTERNET)
                {
                    newState = NetworkState::ConnectedGlobal;
                }
                else
                {
                    newState = NetworkState::ConnectedLocal;
                }
            }
#elif defined(__linux__)
            GNetworkConnectivity connection{ g_network_monitor_get_connectivity(g_network_monitor_get_default()) };
            if (connection == G_NETWORK_CONNECTIVITY_LOCAL)
            {
                newState = NetworkState::Disconnected;
            }
            else if (connection == G_NETWORK_CONNECTIVITY_FULL)
            {
                newState = NetworkState::ConnectedGlobal;
            }
            else
            {
                newState = NetworkState::ConnectedLocal;
            }
#endif
        }
        if (m_connectionState != newState)
        {
            m_connectionState = newState;
            m_stateChanged({ m_connectionState });
        }
    }

#ifdef _WIN32
    ULONG NetworkMonitor::AddRef()
    {
        return 1;
    }

    ULONG NetworkMonitor::Release()
    {
        return 1;
    }

    HRESULT NetworkMonitor::QueryInterface(REFIID riid, LPVOID* ppvObj)
    {
        if (!ppvObj)
        {
            return E_POINTER;
        }
        *ppvObj = nullptr;
        if (riid == IID_IUnknown || riid == IID_INetworkListManagerEvents)
        {
            AddRef();
            *ppvObj = reinterpret_cast<void*>(this);
            return S_OK;
        }
        return E_NOINTERFACE;
    }

    HRESULT NetworkMonitor::ConnectivityChanged(NLM_CONNECTIVITY)
    {
        checkConnectionState();
        return S_OK;
    }

    HRESULT NetworkMonitor::NetworkConnectionPropertyChanged(GUID, NLM_CONNECTION_PROPERTY_CHANGE)
    {
        return S_OK;
    }
#endif
}