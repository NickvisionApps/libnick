# Working with Json configuration files
The purpose of `Nickvision::Helpers::JsonFileBase` is to act as a base when defining your own json data objects that you would like to be saved and retrieved from disk.

Here are some key points when defining your own configuration objects:
- Your data object's constructor must take `const std::filesystem::path& path` parameter and pass it to `JsonFileBase`'s constructor. 
- `JsonFileBase` exposes protected templated `T get(const std::stirng& key, const T& defaultValue)` and `void set(const std::string& key, const T& value)` methods which you must use in your implementation to get and store data from the json object.
- You must explicitly call the `save` method on your object when you want to save the configuration json file to disk. Calling `get` and `set` are not enough to trigger saving the file on disk.

Here is an example of a custom configuration object using `JsonFileBase`:
```cpp
using namespace Nickvision::Events;
using namespace Nickvision::Helpers;

class AppConfig : public JsonFileBase
{
public:
    AppConfig(const std::filesystem::path& path)
        : JsonFileBase{ path } 
    { 

    }

    bool getAutomaticallyCheckForUpdates() const
    {
        return get("AutomaticallyCheckForUpdates", true);
    }

    void setAutomaticallyCheckForUpdates(bool value)
    {
        set("AutomaticallyCheckForUpdates", value);
    }
};

int main()
{
    AppConfig config{ "config.json" };
    config.saved() += [](const EventArgs& e) { std::cout << "Config saved to disk." << std::endl; };
    if(config.getAutomaticallyCheckForUpdates())
    {
        std::cout << "Checking for updates..." << std::endl;
    }
    config.setAutomaticallyCheckForUpdates(false);
    config.save(); //above lambda will be invoked
    return 0;
}
```