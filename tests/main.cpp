#include <gtest/gtest.h>
#include "app/aura.h"

int main(int argc, char* argv[])
{
    Nickvision::App::Aura::getActive().init("org.nickvision.aura.test", "Nickvision Aura Tests", "Aura");
    Nickvision::App::Aura::getActive().getAppInfo().setHtmlDocsStore("https://github.com/NickvisionApps/Denaro/blob/main/NickvisionMoney.Shared/Docs/html");
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}