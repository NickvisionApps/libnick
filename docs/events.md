# Nickvision::Events

This module provides a C#-inspired event system to be used in C++ applications. 

Consumers can register callbacks with an event which will be invoked when the event itself is triggered.

## Table of Contents
- [Event](#event)
- [EventArgs](#eventargs)
- [ParamEventArgs](#parameventargs)

## Event
Description: An event that can have handlers subscribe to it, which in turn will be called when the event is invoked.

Interface: [event.h](/include/events/event.h)

Type: `template<DerivedEventArgs T> class`
- The typename T must derive from `Nickvision::Events::EventArgs`

Path: `Nickvision::Events::Event`

### Methods
- ```cpp
  Event()
  ```
    - Constructs an Event.
- ```cpp
  Event(const Event& e)
  ```
    - Copies an Event object.
- ```cpp
  Event(Event&& e)
  ```
    - Moves an Event object.
- ```cpp
  void subscribe(const std::function<void(const T&)>& handler)
  ```
    - Accepts: A `std::function` with signature `void handler(const T&)` to register as a callback for the event, handler.
- ```cpp
  void unsubscribe(const std::function<void(const T&)>& handler)
  ```
    - Accepts: A `std::function` with signature `void handler(const T&)` to remove as a callback for the event, handler.
- ```cpp
  void invoke(const T& param) const
  ```
    - Accepts: A `T` object to pass as a parameter, param, as all registered callbacks are called on this event.
- ```cpp
  void operator+=(const std::function<void(const T&)>& handler)
  ```
    - Accepts: A `std::function` with signature `void handler(const T&)` to register as a callback for the event, handler. 
- ```cpp
  void operator-=(const std::function<void(const T&)>& handler)
  ```
    - Accepts: A `std::function` with signature `void handler(const T&)` to remove as a callback for the event, handler.
- ```cpp
  Event& operator=(const Event& e)
  ```
    - Copies an Event object.
- ```cpp
  Event& operator=(Event&& e)
  ```
    - Moves an Event object.

### Defining Events In Your Classes
libnick events are designed to easily integrate within your classes to easily notify consumers of changes in state of an object.

Let's take a look at [ConfigurationBase](/include/configurationbase.h) and how it defines and uses events.

```cpp
class ConfigurationBase
{
public:
    ...
    Events::Event<Events::EventArgs>& saved()
    {
        return m_saved;
    }

    bool save()
    {
        ...
        saved({}); //Same as saved.invoke({});
        ...
    }

private:
    ...
    Events::Event<Events::EventArgs> m_saved;
};
```

Here we can see how `ConfigurationBase` defines a `saved` event, exposes it to the consumer, and triggers/invokes the event within its `save` method.

A consumer of `ConfigurationBase` can easily subscribe to the event and have its handler called when the configuration object is saved:
```cpp
void handler(const Nickvision::Events::EventArgs& e)
{
    std::cout << "Config saved." << std::endl;
}

int main()
{
    ConfigurationBase base{ ... };
    base.saved() += handler;
    base.save();
}
```

This program will print "Config saved." as a result of the event being invoke once the save method was called.

## EventArgs
Description: A base class for event arguments.

Interface: [eventargs.h](/include/events/eventargs.h)

Type: `class`

Path: `Nickvision::Events::EventArgs`

### Methods
- ```cpp
  EventArgs() = default
  ```
    - Constructs an EventArgs.

### Note
This class should be the base class of all objects used as a parameter for a libnick Event. 

## ParamEventArgs
Description: An event argument that contains a single parameter.

Interface: [parameventargs.h](/include/events/parameventargs.h)

Type: `template<typename T> class`

Path: `Nickvision::Events::ParamEventArgs`

### Member Variables
- ```
  T Param: get
  ```
    - The parameter stored in the event args.

### Methods
- ```cpp
  ParamEventArgs(const T& param)
  ```
    - Constructs an EventArgs.
    - Accepts: An object to store within this event argument, param.

### Note
This class is a specialization of EventArgs that allows storing a single parameter to pass to events. 

For example, the event `Event<ParamEventArgs<std::string>>` represents an Event that accepts a string.