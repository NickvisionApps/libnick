#ifdef _WIN32
#include "notifications/notifyiconmenu.h"

namespace Nickvision::Notifications
{
	NotifyIconMenuItem::NotifyIconMenuItem(NotifyIconMenuItemType type)
		: m_type{ type }
	{

	}

	NotifyIconMenuItemType NotifyIconMenuItem::getType() const
	{
		return m_type;
	}

	NotifyIconSeparatorMenuItem::NotifyIconSeparatorMenuItem()
		: NotifyIconMenuItem{ NotifyIconMenuItemType::Separator }
	{

	}

	NotifyIconActionMenuItem::NotifyIconActionMenuItem(const std::string& label, const std::function<void()>& action)
		: NotifyIconMenuItem{ NotifyIconMenuItemType::Action },
		m_label{ label },
		m_action{ action }
	{

	}

	const std::string& NotifyIconActionMenuItem::getLabel() const
	{
		return m_label;
	}

	void NotifyIconActionMenuItem::invoke() const
	{
		if (m_action)
		{
			m_action();
		}
	}

	void NotifyIconActionMenuItem::operator()() const
	{
		invoke();
	}

	size_t NotifyIconMenu::size() const
	{
		return m_items.size();
	}

	bool NotifyIconMenu::empty() const
	{
		return m_items.size() == 0;
	}

	const std::shared_ptr<NotifyIconMenuItem>& NotifyIconMenu::get(size_t index) const
	{
		if (index < 0 || index >= m_items.size())
		{
			static std::shared_ptr<NotifyIconMenuItem> null{ nullptr };
			return null;
		}
		return m_items[index];
	}

	size_t NotifyIconMenu::addSeparator()
	{
		m_items.push_back(std::make_shared<NotifyIconSeparatorMenuItem>());
		return m_items.size() - 1;
	}

	bool NotifyIconMenu::insertSeparator(size_t index)
	{
		if (index < 0 || index > m_items.size())
		{
			return false;
		}
		m_items.insert(m_items.begin() + index, std::make_shared<NotifyIconSeparatorMenuItem>());
		return true;
	}

	bool NotifyIconMenu::removeSeparator(size_t index)
	{
		if (index < 0 || index >= m_items.size())
		{
			return false;
		}
		const std::shared_ptr<NotifyIconMenuItem>& item{ m_items[index] };
		if (item->getType() != NotifyIconMenuItemType::Separator)
		{
			return false;
		}
		m_items.erase(m_items.begin() + index);
		return true;
	}

	size_t NotifyIconMenu::addAction(const std::string& label, const std::function<void()>& action)
	{
		m_items.push_back(std::make_shared<NotifyIconActionMenuItem>(label, action));
		return m_items.size() - 1;
	}

	bool NotifyIconMenu::insertAction(size_t index, const std::string& label, const std::function<void()>& action)
	{
		if (index < 0 || index > m_items.size())
		{
			return false;
		}
		m_items.insert(m_items.begin() + index, std::make_shared<NotifyIconActionMenuItem>(label, action));
		return true;
	}

	bool NotifyIconMenu::removeAction(size_t index)
	{
		if (index < 0 || index > m_items.size() - 1)
		{
			return false;
		}
		const std::shared_ptr<NotifyIconMenuItem>& item{ m_items[index] };
		if (item->getType() != NotifyIconMenuItemType::Action)
		{
			return false;
		}
		m_items.erase(m_items.begin() + index);
		return true;
	}
}

#endif