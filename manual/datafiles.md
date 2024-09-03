# Creating Your Own Data Files
The purpose of `DataFileBase` is to act as a base when defining your own data objects that you would like to be saved and retrieved from disk.

Here are some key points when defining your own configuration objects:
- Your data object's constructor must take `const std::string& key` and `const std::string& appName` parameters and pass it to `DataFileBase`'s constructor. 
    - Although you will not use `key` and `appName` in your own implementation, it is required for `DataFileBase`'s functionality and will be filled-in by the `DataFileManager`.
- `DataFileBase` exposes a protected `m_json` object which you must use in your implementation of getting and storing variables of your data object. 
    - If this `m_json` object is not used, your data object will not be stored to disk correctly.
- You must explicitly call the `save` method on your configuration object when you want to save the configuration to disk. Writing to the `m_json` (of type `boost::json::object`) object is not enough to trigger saving the file on disk.

Here is an example of a custom configuration object using `DataFileBase`:
```cpp
using namespace Nickvision::App;
using namespace Nickvision::Events;

class AppConfig : public DataFileBase
{
public:
    AppConfig(const std::string& key, const std::string& appName)
        : DataFileBase{ key, appName } 
    { 

    }

    bool getAutomaticallyCheckForUpdates() const
    {
        const boost::json::value& value{ m_json["AutomaticallyCheckForUpdates"] };
        if(!value.is_bool())
        {
            return true;
        }
        return value.as_bool();
    }

    void setAutomaticallyCheckForUpdates(bool value)
    {
        m_json["AutomaticallyCheckForUpdates"] = value;
    }
};

//This object can now be used with the DataFileManager:

int main()
{
    DataFileManager dfm{ "AppName" };
    AppConfig& config{ dfm.get<AppConfig>("config") };
    config.saved() += [](const EventArgs& e) { std::cout << "Config saved to disk." << std::endl; };
    if(config.getPreviousCount() > 0)
    {
        std::cout << config.getPreviousCount() << std::endl;
    }
    config.setPreviousCount(6);
    config.save(); //above lambda will be invoked on success
    return 0;
}
```