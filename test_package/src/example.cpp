#include "aura.h"

int main() 
{
    Aura::init("io.conan.aura.test", "Conan Aura Test", "Test");
    std::cout << Aura::getActive().getAppInfo().getName() << std::endl;
    return 0;
}