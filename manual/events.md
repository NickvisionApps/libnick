# Defining Events In Your Classes
libnick events are designed to easily integrate within your classes to easily notify consumers of changes in state of an object.

Let's take a look at `Nickvision::App::DataFileBase` and how it defines and uses events.

```cpp
namespace Nickvision::App
{
  class DataFileBase
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
}
```

Here we can see how `Nickvision::App::DataFileBase` defines a `saved` event, exposes it to the consumer, and triggers/invokes the event within its `save` method.

A consumer of `Nickvision::App::DataFileBase` can easily subscribe to the event and have its handler called when the configuration object is saved:
```cpp
using namespace Nickvision::App;
using namespace Nickvision::Events;

void handler(const EventArgs& e)
{
    std::cout << "Config saved." << std::endl;
}

int main()
{
    DataFileBase base{ ... };
    base.saved() += handler;
    base.save();
}
```

This program will print `Config saved.` as a result of the event being invoke once the save method was called.