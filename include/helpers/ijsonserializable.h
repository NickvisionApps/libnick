#ifndef IJSONSERIALIZABLE_H
#define IJSONSERIALIZABLE_H

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
}

#endif //IJSONSERIALIZABLE_H