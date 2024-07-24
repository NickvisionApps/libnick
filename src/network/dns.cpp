#include "network/dns.h"
#ifdef _WIN32
#include <windows.h>
#include <windns.h>
#else
#include <gio/gio.h>
#endif

namespace Nickvision::Network
{
    std::vector<IPv4Address> DNS::resolve(const std::string& hostname)
    {
        std::vector<IPv4Address> addresses;
#ifdef _WIN32
        DNS_RECORDA* record{ nullptr };
        if(DnsQuery_A(hostname.c_str(), DNS_TYPE_A, DNS_QUERY_STANDARD, nullptr, &record, nullptr) == 0)
        {
            DNS_RECORDA* next{ record };
            while(next)
            {
                if(next->wType == DNS_TYPE_A)
                {
                    addresses.push_back({ next->Data.A.IpAddress });
                }
                next = next->pNext;
            }
        }
        DnsFree(record, DnsFreeRecordList);

#else
        GResolver* resolver{ g_resolver_get_default() };
        GList* records{ g_resolver_lookup_by_name(resolver, hostname.c_str(), nullptr, nullptr) };
        if(records)
        {
            GList* next{ records };
            while(next)
            {
                GInetAddress* address{ G_INET_ADDRESS(next->data) };
                if(g_inet_address_get_family(address) == G_SOCKET_FAMILY_IPV4 && g_inet_address_get_native_size(address) == 4)
                {
                    const unsigned char* bytes{ g_inet_address_to_bytes(address) };
                    addresses.push_back({ bytes[0], bytes[1], bytes[2], bytes[3] });
                }
                next = next->next;
            }
            g_resolver_free_addresses(records);
        }
        g_object_unref(resolver);
#endif
        return addresses;
    }
}