# Defining Events In Your Classes
libnick events are designed to easily integrate within your classes to easily notify consumers of changes in state of an object.

Let's take a look at `ConfigurationBase` and how it defines and uses events.

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