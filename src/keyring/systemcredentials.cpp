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
				std::string credName{ cred->TargetName ? cred->TargetName : "" };
				std::string credUrl{ cred->Comment ? cred->Comment : "" };
				std::string credUsername{ cred->UserName ? cred->UserName : "" };
				std::string credPassword{ LPCSTR(cred->CredentialBlob) ? LPCSTR(cred->CredentialBlob) : "", cred->CredentialBlobSize };
				CredFree(cred);
				return { { credName, credUrl, credUsername, credPassword } };
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

	std::optional<Credential> SystemCredentials::addCredential(const std::string& name)
	{
		PasswordGenerator passGen;
		Credential c{ name, "", "default", passGen.next() };
		if (addCredential(c))
		{
			return c;
		}
		return std::nullopt;
	}

	bool SystemCredentials::addCredential(const Credential& credential)
	{
#ifdef _WIN32
		CREDENTIALA* cred{ new CREDENTIALA() };
		cred->AttributeCount = 0;
		cred->Attributes = nullptr;
		cred->Comment = nullptr;
		cred->Type = CRED_TYPE_GENERIC;
		cred->Persist = CRED_PERSIST_LOCAL_MACHINE;
		cred->TargetName = LPSTR(credential.getName().c_str());
		cred->UserName = LPSTR(credential.getUsername().c_str());
		cred->CredentialBlobSize = (unsigned long)credential.getPassword().size();
		cred->CredentialBlob = LPBYTE(credential.getPassword().c_str());
		bool res = CredWriteA(cred, 0);
		CredFree(cred);
		return res;
#else
		GError* error{ nullptr };
		secret_password_store_sync(&KEYRING_SCHEMA, SECRET_COLLECTION_DEFAULT, credential.getName().c_str(), credential.getPassword().c_str(), nullptr, &error, "application", credential.getName().c_str(), NULL);
		if (error)
		{
			g_error_free(error);
			return false;
		}
		return true;
#endif
	}

	bool SystemCredentials::updateCredential(const Credential& credential)
	{
#ifdef _WIN32
		CREDENTIALA* cred{ nullptr };
		if (CredReadA(credential.getName().c_str(), CRED_TYPE_GENERIC, 0, &cred))
		{
			cred->AttributeCount = 0;
			cred->Attributes = nullptr;
			cred->Comment = nullptr;
			cred->Type = CRED_TYPE_GENERIC;
			cred->Persist = CRED_PERSIST_LOCAL_MACHINE;
			cred->TargetName = LPSTR(credential.getName().c_str());
			cred->UserName = LPSTR(credential.getUsername().c_str());
			cred->CredentialBlobSize = (unsigned long)credential.getPassword().size();
			cred->CredentialBlob = LPBYTE(credential.getPassword().c_str());
			bool res = CredWriteA(cred, 0);
			CredFree(cred);
			return res;
		}
#else
		GError* error{ nullptr };
		char* password = secret_password_lookup_sync(&KEYRING_SCHEMA, nullptr, &error, "application", credential.getName().c_str(), NULL);
		if (!error && password)
		{
			secret_password_free(password);
			secret_password_store_sync(&KEYRING_SCHEMA, SECRET_COLLECTION_DEFAULT, credential.getName().c_str(), credential.getPassword().c_str(), nullptr, &error, "application", credential.getName().c_str(), NULL);
			if (error)
			{
				g_error_free(error);
				return false;
			}
			return true;
		}
		g_error_free(error);
#endif
		return false;
	}

	bool SystemCredentials::deleteCredential(const std::string& name)
	{
#ifdef _WIN32
		return CredDeleteA(name.c_str(), CRED_TYPE_GENERIC, 0);
#else
		GError* error{ nullptr };
		bool res = secret_password_clear_sync(&KEYRING_SCHEMA, nullptr, &error, "application", name.c_str(), NULL);
		if (!error)
		{
			return res;
		}
		g_error_free(error);
#endif
		return false;
	}
}