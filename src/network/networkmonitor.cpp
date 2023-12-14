#include "network/networkmonitor.h"
#include <cstdlib>
#include "helpers/stringhelpers.h"
#ifdef _WIN32
#include <windows.h>
#include <atlbase.h>
#include <netlistmgr.h>
#elif defined(__linux__)
#include <glib.h>
#include <gio/gio.h>
#endif

namespace Nickvision::Aura::Network
{
	NetworkMonitor::NetworkMonitor()
		: m_connectionState{ false }
	{
#ifdef _WIN32
		CoInitialize(nullptr);
#elif defined(__linux__)
		m_networkChangedHandlerId = g_signal_connect_data(G_OBJECT(g_network_monitor_get_default()), "network-changed", G_CALLBACK((void(*)(GNetworkMonitor*, bool, void*))([](GNetworkMonitor*, bool, void* data)
		{
			static_cast<NetworkMonitor*>(data)->checkConnectionState();
		})), this, nullptr, G_CONNECT_DEFAULT);
#endif
	}

	NetworkMonitor::~NetworkMonitor()
	{
#ifdef __linux__
		g_signal_handler_disconnect(G_OBJECT(g_network_monitor_get_default()), m_networkChangedHandlerId);
#endif
	}

	Events::Event<NetworkStateChangedEventArgs>& NetworkMonitor::stateChanged()
	{
		return m_stateChanged;
	}

	NetworkState NetworkMonitor::getConnectionState() const
	{
		return m_connectionState;
	}

	void NetworkMonitor::checkConnectionState()
	{
		NetworkState newState{ NetworkState::Disconnected };
		char* envNoNetCheck{ std::getenv("AURA_DISABLE_NETCHECK") };
		std::string noNetCheck{ envNoNetCheck ? StringHelpers::toLower(envNoNetCheck) : "" };
		if (!noNetCheck.empty() && (noNetCheck == "true" || noNetCheck == "t" || noNetCheck == "yes" || noNetCheck == "y" || noNetCheck == "1"))
		{
			newState = NetworkState::ConnectedGlobal;
		}
		else
		{
#ifdef _WIN32
			CComPtr<INetworkListManager> pNLM;
			if (CoCreateInstance(CLSID_NetworkListManager, nullptr, CLSCTX_ALL, __uuidof(INetworkListManager), (LPVOID*)&pNLM) == S_OK)
			{
				NLM_CONNECTIVITY connection;
				if (pNLM->GetConnectivity(&connection) == S_OK)
				{
					if ((connection & NLM_CONNECTIVITY_DISCONNECTED) == NLM_CONNECTIVITY_DISCONNECTED)
					{
						newState = NetworkState::Disconnected;
					}
					else if ((connection & NLM_CONNECTIVITY_IPV4_INTERNET) == NLM_CONNECTIVITY_IPV4_INTERNET || (connection & NLM_CONNECTIVITY_IPV6_INTERNET) == NLM_CONNECTIVITY_IPV6_INTERNET)
					{
						newState = NetworkState::ConnectedGlobal;
					}
					else
					{
						newState = NetworkState::ConnectedLocal;
					}
				}
			}
#elif defined(__linux__)
			GNetworkMonitor* netMon{ g_network_monitor_get_default() };
			GNetworkConnectivity connection{ g_network_monitor_get_connectivity(netMon) };
			switch (connection)
			{
			case G_NETWORK_CONNECTIVITY_LOCAL:
				newState = NetworkState::Disconnected;
				break;
			case G_NETWORK_CONNECTIVITY_FULL:
				newState = NetworkState::ConnectedGlobal;
				break;
			default:
				newState = NetworkState::ConnectedLocal;
				break;
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