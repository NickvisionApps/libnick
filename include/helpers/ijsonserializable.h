#ifndef IJSONSERIALIZABLE_H
#define IJSONSERIALIZABLE_H

#include <string>
#include <type_traits>
#include <boost/json.hpp>

namespace Nickvision::Helpers
{
    /**
     * @brief An interface for objects serializable to Json.
     */
    class IJsonSerializable
    {
    public:
        virtual ~IJsonSerializable() = default;
        /**
         * @brief Serializes the object to Json.
         * @return The Json representation of the object
         */
        virtual boost::json::value toJson() const noexcept = 0;
    };

    template<typename T>
    concept DerivedJsonSerializable = std::is_base_of_v<IJsonSerializable, T>;

    template<typename T>
    concept SupportedJsonValue = std::is_same_v<T, int> || 
        std::is_same_v<T, std::int64_t> || 
        std::is_same_v<T, double> || 
        std::is_same_v<T, bool> || 
        std::is_same_v<T, std::string> ||
        std::is_same_v<T, boost::json::array> ||
        std::is_same_v<T, boost::json::object>;
}

#endif //IJSONSERIALIZABLE_H