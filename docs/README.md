# libaura Documentation

This folder contains all of the necessary documentation for using libaura and it's provided modules.

Each module (separated by namespace) contains its own documentation file that can be found within this folder or via one of the links below.

## Documentation Syntax

### Member Variables
Many class types contain member variables exposed by getter and/or setter methods depending on the class' exposure of the variable. 

For example, assume a class exposes the member variable `std::string m_var`. We will document this as `std::string Var: get, set`. 

This means that the class provides the following methods for `m_var`: `std::string getVar() const` and `void setVar(const std::string& var)`.

If a member variable is get or set only, we will document this as `std::string Var: get` and `std::string Var: set` respectively. 

## Modules
- [Nickvision::Aura](aura.md)
- [Nickvision::Aura::Events](events.md)
- [Nickvision::Aura::Filesystem](filesystem.md)
- [Nickvision::Aura::Helpers](helpers.md)
- [Nickvision::Aura::Keyring](keyring.md)
- [Nickvision::Aura::Localization](localization.md)
- [Nickvision::Aura::Network](network.md)
- [Nickvision::Aura::Notifications](notifications.md)
- [Nickvision::Aura::Taskbar](taskbar.md)
- [Nickvision::Aura::Update](update.md)