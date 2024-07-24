#ifndef IPV4ADDRESS_H
#define IPV4ADDRESS_H

#include <optional>
#include <string>

namespace Nickvision::Network
{
    /**
     * @brief A model of an IPv4 address.
     */
    class IPv4Address
    {
    public:
        /**
         * @brief Constructs an IPv4Address.
         * @param first The first byte of the address
         * @param second The second byte of the address
         * @param third The third byte of the address
         * @param fourth The fourth byte of the address
         */
        IPv4Address(unsigned char first, unsigned char second, unsigned char third, unsigned char fourth);
        /**
         * @brief Gets the first byte of the address.
         * @return The first byte of the address
         */
        unsigned char getFirst() const;
        /**
         * @brief Gets the second byte of the address.
         * @return The second byte of the address
         */
        unsigned char getSecond() const;
        /**
         * @brief Gets the third byte of the address.
         * @return The third byte of the address
         */
        unsigned char getThird() const;
        /**
         * @brief Gets the fourth byte of the address.
         * @return The fourth byte of the address
         */
        unsigned char getFourth() const;
        /**
         * @brief Gets the string representation of the address.
         * @return The string representation of the address
         */
        std::string str() const;
        /**
         * @brief Parses an IPv4Address from a string.
         * @param address The string address to parse
         * @return The parsed address if successful, else std::nullopt
         */
        static std::optional<IPv4Address> parse(const std::string& address);

    private:
        unsigned char m_first;
        unsigned char m_second;
        unsigned char m_third;
        unsigned char m_fourth;
    };
}

#endif //IPV4ADDRESS_H