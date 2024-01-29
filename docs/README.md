# libnick Documentation

This folder contains all of the necessary documentation for using libnick and it's provided modules.

Each module (separated by namespace) contains its own documentation file that can be found within this folder or via one of the links below.

## Table of Contents
- [Documentation Syntax](#documentationsyntax)
- [Modules](#modules)

## Documentation Syntax

### Member Variables
Many class types contain member variables exposed by getter and/or setter methods depending on the class' exposure of the variable. 

For example, assume a class exposes the member variable `std::string m_var`. We will document this as `std::string Var: get, set`. 

This means that the class provides the following methods for `m_var`: `std::string getVar() const` and `void setVar(const std::string& var)`.

If a member variable is get or set only, we will document this as `std::string Var: get` and `std::string Var: set` respectively. 

### Events
libnick provides a C#-inspired event system module under the Nickvision::Events namespace. This module contains a class `Event<T>` that handles registering and calling callbacks when said event is invoked.

For example, assume a class has an event defined as `Nickvision::Events::Event<Nickvision::Events::EventArgs> m_saved`. We will document exposure of this event as `Event<Nickvision::Events::EventArgs> Saved`.

This means that the class provides the following method, allowing consumers to register callbacks to the event using the `+=` operator: `Nickvision::Events::Event<Nickvision::Events::EventArgs>& saved()`.

## Modules
- [Nickvision::App](app.md)
- [Nickvision::Events](events.md)
- [Nickvision::Filesystem](filesystem.md)
- [Nickvision::Helpers](helpers.md)
- [Nickvision::Keyring](keyring.md)
- [Nickvision::Localization](localization.md)
- [Nickvision::Network](network.md)
- [Nickvision::Notifications](notifications.md)
- [Nickvision::Taskbar](taskbar.md)
- [Nickvision::Update](update.md)