#include "keyring/systemcredentials.h"
#include <vector>
#include "keyring/passwordgenerator.h"
#ifdef _WIN32
#include <windows.h>
#include <wincred.h>
#else
#include <libsecret/secret.h>
#endif

namespace Nickvision::Aura::Keyring
{
#ifndef _WIN32
	static const SecretSchema KEYRING_SCHEMA = { "org.nickvision.aura.keyring", SECRET_SCHEMA_NONE, { { "application", SECRET_SCHEMA_ATTRIBUTE_STRING }, { "NULL", SECRET_SCHEMA_ATTRIBUTE_STRING } } };
#endif

	std::optional<Credential> SystemCredentials::getCredential(const std::string& name)
	{
#ifdef _WIN32
		CREDENTIALA* cred{ nullptr };
		if (CredReadA(name.c_str(), CRED_TYPE_GENERIC, 0, &cred))
		{
			if (cred->CredentialBlob)
			{
				Credential c{ cred->TargetName, cred->Comment, cred->UserName, LPCSTR(cred->CredentialBlob) };
				CredFree(cred);
				return c;
			}
		}
#else
		GError* error{ nullptr };
		char* password = secret_password_lookup_sync(&KEYRING_SCHEMA, nullptr, &error, "application", name.c_str(), NULL);
		if (!error && password)
		{
			Credential c{ name, "", "default", password };
			secret_password_free(password);
			return c;
		}
		g_error_free(error);
#endif
		return std::nullopt;
	}

	std::optional<Credential> SystemCredentials::addNewCredential(const std::string& name)
	{
		PasswordGenerator passGen;
		Credential c{ name, "", "NickvisionKeyring", passGen.next() };
#ifdef _WIN32
		CREDENTIALA* cred{ new CREDENTIALA() };
		cred->AttributeCount = 0;
		cred->Attributes = nullptr;
		cred->Comment = nullptr;
		cred->Type = CRED_TYPE_GENERIC;
		cred->Persist = CRED_PERSIST_LOCAL_MACHINE;
		cred->TargetName = LPSTR(c.getName().c_str());
		cred->UserName = LPSTR(c.getUsername().c_str());
		cred->CredentialBlobSize = (unsigned long)c.getPassword().size();
		cred->CredentialBlob = LPBYTE(c.getPassword().c_str());
		bool res = CredWriteA(cred, 0);
		CredFree(cred);
		if (!res)
		{
			return std::nullopt;
		}
		return c;
#else
		GError* error{ nullptr };
		secret_password_store_sync(&KEYRING_SCHEMA, SECRET_COLLECTION_DEFAULT, c.getName().c_str(), c.getPassword().c_str(), nullptr, &error, "application", c.getName().c_str(), NULL);
		if (error)
		{
			g_error_free(error);
			return std::nullopt;
		}
		return c;
#endif
	}

	bool SystemCredentials::deleteCredential(const Credential& credential)
	{
#ifdef _WIN32
		return CredDeleteA(credential.getName().c_str(), CRED_TYPE_GENERIC, 0);
#else
		GError* error{ nullptr };
		bool res = secret_password_clear_sync(&KEYRING_SCHEMA, nullptr, &error, "application", credential.getName().c_str(), NULL);
		if (!error)
		{
			return res;
		}
		g_error_free(error);
#endif
		return false;
	}
}