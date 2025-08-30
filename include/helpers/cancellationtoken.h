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
 * A token that can be used to cancel operations.
 */

#ifndef CANCELLATIONTOKEN_H
#define CANCELLATIONTOKEN_H

#include <functional>
#include <mutex>

namespace Nickvision::Helpers
{
    /**
     * @brief A token that can be used to cancel an operation.
     */
    class CancellationToken
    {
    public:
        /**
         * @brief Constructs a CancellationToken.
         * @param cancelFunction A callback function to call when the token is cancelled
         */
        CancellationToken(const std::function<void()>& cancelFunction = {}) noexcept;
        /**
         * @brief Gets whether or not the token is cancelled.
         * @return True if token is cancelled, else false
         */
        bool isCancelled() const noexcept;
        /**
         * @brief Gets the cancel function to be called when the token is cancelled.
         * @return The cancel function
         */
        const std::function<void()>& getCancelFunction() const noexcept;
        /**
         * @brief Sets the cancel function to be called when the token is cancelled.
         * @param cancelFunction The cancel function
         */
        void setCancelFunction(const std::function<void()>& cancelFunction) noexcept;
        /**
         * @brief Cancels the token.
         */
        void cancel() noexcept;
        /**
         * @brief Resets the token to its initial state.
         * @brief Removes the cancel function and sets the cancelled state to false.
         */
        void reset() noexcept;
        /**
         * @brief Converts the token to a boolean.
         * @return True if token is cancelled, else false
         */
        operator bool() const noexcept;

    private:
        mutable std::mutex m_mutex;
        bool m_cancelled;
        std::function<void()> m_cancelFunction;
    };
}

#endif //CANCELLATIONTOKEN_H