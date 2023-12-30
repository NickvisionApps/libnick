#include "network/networkmonitor.h"
#include <stdexcept>
#include "aura.h"
#include "helpers/stringhelpers.h"
#ifdef __linux__
#include <glib.h>
#include <gio/gio.h>
#endif

namespace Nickvision::Aura::Network
{
	NetworkMonitor::NetworkMonitor()
		: m_connectionState{ NetworkState::Disconnected }
	{
#ifdef _WIN32
		CoInitialize(nullptr);
		if (CoCreateInstance(CLSID_NetworkListManager, nullptr, CLSCTX_ALL, __uuidof(INetworkListManager), (LPVOID*)&m_netListManager) != S_OK)
		{
			throw std::runtime_error("Unable to create network list manager.");
		}
		//TODO:: Watch for changes on Windows
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

	NetworkMonitor::~NetworkMonitor() noexcept
	{
#ifdef __linux__
		g_signal_handler_disconnect(G_OBJECT(g_network_monitor_get_default()), m_networkChangedHandlerId);
#endif
	}

	Events::Event<NetworkStateChangedEventArgs>& NetworkMonitor::stateChanged() noexcept
	{
		return m_stateChanged;
	}

	NetworkState NetworkMonitor::getConnectionState() const noexcept
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		return m_connectionState;
	}

	void NetworkMonitor::checkConnectionState() noexcept
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		NetworkState newState{ NetworkState::Disconnected };
		std::string noNetCheck{ StringHelpers::toLower(Aura::getEnvVar("AURA_DISABLE_NETCHECK")) };
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
			m_stateChanged.invoke({ m_connectionState });
		}
	}
}