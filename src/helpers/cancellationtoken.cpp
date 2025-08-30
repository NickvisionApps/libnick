#include "helpers/cancellationtoken.h"

namespace Nickvision::Helpers
{
    CancellationToken::CancellationToken(const std::function<void()>& cancelFunction) noexcept
        : m_cancelled{ false },
        m_cancelFunction{ cancelFunction }
    {

    }

    bool CancellationToken::isCancelled() const noexcept
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return m_cancelled;
    }

    const std::function<void()>& CancellationToken::getCancelFunction() const noexcept
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return m_cancelFunction;
    }

    void CancellationToken::setCancelFunction(const std::function<void()>& cancelFunction) noexcept
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        m_cancelFunction = cancelFunction;
    }

    void CancellationToken::cancel() noexcept
    {
        std::unique_lock<std::mutex> lock{ m_mutex };
        if(m_cancelled)
        {
            return;
        }
        m_cancelled = true;
        if(m_cancelFunction)
        {
            lock.unlock();
            m_cancelFunction();
        }
    }

    void CancellationToken::reset() noexcept
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        m_cancelled = false;
        m_cancelFunction = {};
    }

    CancellationToken::operator bool() const noexcept
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return m_cancelled;
    }
}