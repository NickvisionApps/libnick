#include "keyring/systemcredentials.h"
#ifdef _WIN32
#include <windows.h>
#include <wincred.h>
#endif

namespace Nickvision::Aura::Keyring
{
	std::optional<Credential> SystemCredentials::getCredential(const std::string& name)
	{
		return std::nullopt;
	}

	std::optional<Credential> SystemCredentials::addNewCredential(const std::string& name)
	{
		return std::nullopt;
	}

	void SystemCredentials::deleteCredential(const Credential& name)
	{

	}
}