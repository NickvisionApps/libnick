#ifdef _WIN32
#ifndef NOTIFYICONMENU_H
#define NOTIFYICONMENU_H

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace Nickvision::Aura::Notifications
{
	/**
	 * @brief Types of menu items for a NotifyIcon. 
	 */
	enum class NotifyIconMenuItemType
	{
		Action = 0,
		Separator = 1
	};

	/**
	 * @brief A generic menu item for a NotifyIcon.
	 */
	class NotifyIconMenuItem
	{
	public:
		/**
		 * @brief Constructs a NotifyIconMenuItem.
		 * @param type The type of menu item
		 */
		NotifyIconMenuItem(NotifyIconMenuItemType type) noexcept;
		/**
		 * @brief Destructs a NotifyIconMenuItem.
		 */
		virtual ~NotifyIconMenuItem() = default;
		/**
		 * @brief Gets the type of the menu item.
		 * @return NotifyIconMenuItemType
		 */
		NotifyIconMenuItemType getType() const noexcept;

	private:
		NotifyIconMenuItemType m_type;
	};

	/**
	 * @brief A separator menu item for a NotifyIcon.
	 */
	class NotifyIconSeparatorMenuItem : public NotifyIconMenuItem
	{
	public:
		/**
		 * @brief Constructs a NotifyIconSeparatorMenuItem.
		 */
		NotifyIconSeparatorMenuItem() noexcept;
	};

	/**
	 * @brief An actionable menu item for a NotifyIcon.
	 */
	class NotifyIconActionMenuItem : public NotifyIconMenuItem
	{
	public:
		/**
		 * @brief Constructs a NotifyIconActionMenuItem.
		 * @param label The label for the menu item
		 * @param action The function to call when the menu item is clicked
		 */
		NotifyIconActionMenuItem(const std::string& label, const std::function<void()>& action) noexcept;
		/**
		 * @brief Gets the label for the menu item.
		 * @return The label for the menu item
		 */
		const std::string& getLabel() const noexcept;
		/**
		 * @brief Calls the callback function of the menu item.
		 */
		void invoke() const noexcept;
		/**
		 * @brief Calls the callback function of the menu item.
		 */
		void operator()() const noexcept;

	private:
		std::string m_label;
		std::function<void()> m_action;
	};

	/**
	 * @brief A menu for a NotifyIcon.
	 */
	class NotifyIconMenu
	{
	public:
		/**
		 * @brief Constructs a NotifyIconMenu.
		 */
		NotifyIconMenu() noexcept = default;
		/**
		 * @brief Gets the number of items in the menu.
		 * @return The number of items in the menu
		 */
		size_t size() const noexcept;
		/**
		 * @brief Gets whether or not the menu is empty.
		 * @return True if the menu is empty, else false
		 */
		bool empty() const noexcept;
		/**
		 * @brief Gets the NotifyIconMenuItem at a given index in the menu.
		 * @returns The NotifyIconMenuItem at the given index. nullptr if the index is invalid;
		 */
		const std::shared_ptr<NotifyIconMenuItem>& get(size_t index) const noexcept;
		/**
		 * @brief Adds a separator to the bottom of the menu.
		 * @return The index at which the separator was added
		 */
		size_t addSeparator() noexcept;
		/**
		 * @brief Inserts a separator at a given index in the menu.
		 * @param index The index at which to insert the separator
		 * @return True if the separator was successfully inserted, else false
		 */
		bool insertSeparator(size_t index) noexcept;
		/**
		 * @brief Removes a separator at a given index in the menu.
		 * @param index The index at which to remove the separator
		 * @return True if the separator was successfully removed, else false
		 */
		bool removeSeparator(size_t index) noexcept;
		/**
		 * @brief Adds an action to the bottom of the menu.
		 * @param label The label of the action
		 * @param action The callback function of the action
		 * @return The index at which the action was added
		 */
		size_t addAction(const std::string& label, const std::function<void()>& action) noexcept;
		/**
		 * @brief Inserts an action at a given index in the menu.
		 * @param index The index at which to insert the action
		 * @param label The label of the action
		 * @param action The callback function of the action
		 * @return True if the action was successfully inserted, else false
		 */
		bool insertAction(size_t index, const std::string& label, const std::function<void()>& action) noexcept;
		/**
		 * @brief Removes an action at a given index in the menu.
		 * @param index The index at which to remove the action
		 * @return True if the action was successfully removed, else false
		 */
		bool removeAction(size_t index) noexcept;

	private:
		std::vector<std::shared_ptr<NotifyIconMenuItem>> m_items;
	};
}

#endif //NOTIFYICONMENU_H
#endif