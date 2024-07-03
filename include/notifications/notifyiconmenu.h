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
 * A menu for a NotifyIcon.
 */

#ifdef _WIN32
#ifndef NOTIFYICONMENU_H
#define NOTIFYICONMENU_H

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace Nickvision::Notifications
{
    /**
     * @brief Types of menu items for a NotifyIcon. 
     */
    enum class NotifyIconMenuItemType
    {
        Action = 0, ///< A menu item that can be clicked.
        Separator = 1 ///< A separator menu item.
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
        NotifyIconMenuItem(NotifyIconMenuItemType type);
        /**
         * @brief Destructs a NotifyIconMenuItem.
         */
        virtual ~NotifyIconMenuItem() = default;
        /**
         * @brief Gets the type of the menu item.
         * @return NotifyIconMenuItemType
         */
        NotifyIconMenuItemType getType() const;

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
        NotifyIconSeparatorMenuItem();
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
        NotifyIconActionMenuItem(const std::string& label, const std::function<void()>& action);
        /**
         * @brief Gets the label for the menu item.
         * @return The label for the menu item
         */
        const std::string& getLabel() const;
        /**
         * @brief Calls the callback function of the menu item.
         */
        void invoke() const;
        /**
         * @brief Calls the callback function of the menu item.
         */
        void operator()() const;

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
        NotifyIconMenu() = default;
        /**
         * @brief Gets the number of items in the menu.
         * @return The number of items in the menu
         */
        size_t size() const;
        /**
         * @brief Gets whether or not the menu is empty.
         * @return True if the menu is empty, else false
         */
        bool empty() const;
        /**
         * @brief Gets the NotifyIconMenuItem at a given index in the menu.
         * @param index The index of the item to get
         * @returns The NotifyIconMenuItem at the given index
         * @returns nullptr if the index is invalid
         */
        const std::shared_ptr<NotifyIconMenuItem>& get(size_t index) const;
        /**
         * @brief Adds a separator to the bottom of the menu.
         * @return The index at which the separator was added
         */
        size_t addSeparator();
        /**
         * @brief Inserts a separator at a given index in the menu.
         * @param index The index at which to insert the separator
         * @return True if the separator was successfully inserted, else false
         */
        bool insertSeparator(size_t index);
        /**
         * @brief Removes a separator at a given index in the menu.
         * @param index The index at which to remove the separator
         * @return True if the separator was successfully removed, else false
         */
        bool removeSeparator(size_t index);
        /**
         * @brief Adds an action item to the bottom of the menu.
         * @param label The label of the action
         * @param action The callback function of the action
         * @return The index at which the action item was added
         */
        size_t addAction(const std::string& label, const std::function<void()>& action);
        /**
         * @brief Inserts an action item at a given index in the menu.
         * @param index The index at which to insert the action item
         * @param label The label of the action
         * @param action The callback function of the action
         * @return True if the action item was successfully inserted, else false
         */
        bool insertAction(size_t index, const std::string& label, const std::function<void()>& action);
        /**
         * @brief Removes an action item at a given index in the menu.
         * @param index The index at which to remove the action item
         * @return True if the action item was successfully removed, else false
         */
        bool removeAction(size_t index);
        /**
         * @brief Gets the NotifyIconMenuItem at a given index in the menu.
         * @param index The index of the item to get
         * @returns The NotifyIconMenuItem at the given index
         * @returns nullptr if the index is invalid
         */
        const std::shared_ptr<NotifyIconMenuItem>& operator[](size_t index) const;

    private:
        std::vector<std::shared_ptr<NotifyIconMenuItem>> m_items;
    };
}

#endif //NOTIFYICONMENU_H
#endif