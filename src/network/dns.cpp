#include "network/dns.h"
#ifdef _WIN32
#include <windows.h>
#include <windns.h>
#else
#include <gio/gio.h>
#endif

namespace Nickvision::Network
{
    std::vector<IPv4Address> DNS::a(const std::string& hostname)
    {
        std::vector<IPv4Address> addresses;
#ifdef _WIN32
        DNS_RECORDA* record{ nullptr };
        if(DnsQuery_A(hostname.c_str(), DNS_TYPE_A, DNS_QUERY_STANDARD, nullptr, &record, nullptr) == 0)
        {
            for(DNS_RECORDA* r = record; r; r = r->pNext)
            {
                if(r->wType == DNS_TYPE_A)
                {
                    addresses.push_back({ r->Data.A.IpAddress });
                }
            }
        }
        DnsFree(record, DnsFreeRecordList);

#else
        GResolver* resolver{ g_resolver_get_default() };
        GList* record{ g_resolver_lookup_by_name_with_flags(resolver, hostname.c_str(), G_RESOLVER_NAME_LOOKUP_FLAGS_IPV4_ONLY, nullptr, nullptr) };
        if(record)
        {
            for(GList* r = record; r; r = r->next)
            {
                GInetAddress* address{ G_INET_ADDRESS(r->data) };
                if(g_inet_address_get_family(address) == G_SOCKET_FAMILY_IPV4 && g_inet_address_get_native_size(address) == 4)
                {
                    const unsigned char* bytes{ g_inet_address_to_bytes(address) };
                    addresses.push_back({ bytes[0], bytes[1], bytes[2], bytes[3] });
                }
            }
            g_resolver_free_addresses(record);
        }
        g_object_unref(resolver);
#endif
        return addresses;
    }

    std::vector<std::string> DNS::txt(const std::string& hostname)
    {
        std::vector<std::string> aliases;
#ifdef _WIN32
        DNS_RECORDA* record{ nullptr };
        if(DnsQuery_A(hostname.c_str(), DNS_TYPE_TEXT, DNS_QUERY_STANDARD, nullptr, &record, nullptr) == 0)
        {
            for(DNS_RECORDA* r = record; r; r = r->pNext)
            {
                if(r->wType == DNS_TYPE_TEXT)
                {
                    for(unsigned long i = 0; i < r->Data.TXT.dwStringCount; i++)
                    {
                        aliases.push_back(r->Data.TXT.pStringArray[i]);
                    }
                }
            }
        }
        DnsFree(record, DnsFreeRecordList);
#else
        GResolver* resolver{ g_resolver_get_default() };
        GList* records{ g_resolver_lookup_records(resolver, hostname.c_str(), G_RESOLVER_RECORD_TXT, nullptr, nullptr) };
        if(records)
        {
            for(GList* r = records; r; r = r->next)
            {
                const gchar** contents;
                g_variant_get(static_cast<GVariant*>(r->data), "(^a&s)", &contents);
                for(int i = 0; contents[i]; i++)
                {
                    aliases.push_back(contents[i]);
                }
                g_free(contents);
            }
            g_list_free_full(records, GDestroyNotify(+[](gpointer data){ g_variant_unref(static_cast<GVariant*>(data)); }));
        }
        g_object_unref(resolver);
#endif
        return aliases;
    }
}