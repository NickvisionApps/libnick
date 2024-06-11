#include "keyring/systemcredentials.h"
#include <vector>
#include "helpers/stringhelpers.h"
#include "keyring/passwordgenerator.h"
#ifdef _WIN32
#include <windows.h>
#include <wincred.h>
#elif defined(__linux__)
#include <libsecret/secret.h>
#endif

using namespace Nickvision::Helpers;

namespace Nickvision::Keyring
{
#ifdef __linux__
    static const SecretSchema KEYRING_SCHEMA = { "org.nickvision.aura.keyring", SECRET_SCHEMA_NONE, { { "application", SECRET_SCHEMA_ATTRIBUTE_STRING }, { "NULL", SECRET_SCHEMA_ATTRIBUTE_STRING } } };
#endif

    std::optional<Credential> SystemCredentials::getCredential(const std::string& name)
    {
#ifdef _WIN32
        CREDENTIALW* cred{ nullptr };
        std::wstring wName{ StringHelpers::wstr(name) };
        if (CredReadW(wName.c_str(), CRED_TYPE_GENERIC, 0, &cred))
        {
            if (cred->CredentialBlob)
            {
                std::wstring credName{ cred->TargetName ? cred->TargetName : L"" };
                std::wstring credUrl{ cred->Comment ? cred->Comment : L"" };
                std::wstring credUsername{ cred->UserName ? cred->UserName : L"" };
                std::wstring credPassword{ (const wchar_t*)cred->CredentialBlob, static_cast<size_t>(cred->CredentialBlobSize / sizeof(wchar_t)) };
                CredFree(cred);
                return Credential{ StringHelpers::str(credName), StringHelpers::str(credUrl), StringHelpers::str(credUsername), StringHelpers::str(credPassword) };
            }
        }
#elif defined(__linux__)
        GError* error{ nullptr };
        char* password{ secret_password_lookup_sync(&KEYRING_SCHEMA, nullptr, &error, "application", name.c_str(), NULL) };
        if (!error && password)
        {
            Credential c{ name, "", "default", password };
            secret_password_free(password);
            return c;
        }
        if (error)
        {
            g_error_free(error);
        }
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
        std::wstring name{ StringHelpers::wstr(credential.getName()) };
        std::wstring uri{ StringHelpers::wstr(credential.getUri()) };
        std::wstring username{ StringHelpers::wstr(credential.getUsername()) };
        std::wstring password{ StringHelpers::wstr(credential.getPassword()) };
        CREDENTIALW cred{ 0 };
        cred.AttributeCount = 0;
        cred.Attributes = nullptr;
        cred.Type = CRED_TYPE_GENERIC;
        cred.Persist = CRED_PERSIST_LOCAL_MACHINE;
        cred.TargetName = LPWSTR(name.c_str());
        cred.Comment = LPWSTR(uri.c_str());
        cred.UserName = LPWSTR(username.c_str());
        cred.CredentialBlobSize = static_cast<unsigned long>(password.size() * sizeof(wchar_t));
        cred.CredentialBlob = LPBYTE(password.c_str());
        return CredWriteW(&cred, 0);
#elif defined(__linux__)
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
        CREDENTIALW* cred{ nullptr };
        std::wstring name{ StringHelpers::wstr(credential.getName()) };
        std::wstring uri{ StringHelpers::wstr(credential.getUri()) };
        std::wstring username{ StringHelpers::wstr(credential.getUsername()) };
        std::wstring password{ StringHelpers::wstr(credential.getPassword()) };
        if (CredReadW(name.c_str(), CRED_TYPE_GENERIC, 0, &cred))
        {
            cred->AttributeCount = 0;
            cred->Attributes = nullptr;
            cred->Type = CRED_TYPE_GENERIC;
            cred->Persist = CRED_PERSIST_LOCAL_MACHINE;
            cred->TargetName = LPWSTR(name.c_str());
            cred->Comment = LPWSTR(uri.c_str());
            cred->UserName = LPWSTR(username.c_str());
            cred->CredentialBlobSize = static_cast<unsigned long>(password.size() * sizeof(wchar_t));
            cred->CredentialBlob = LPBYTE(password.c_str());
            bool res = CredWriteW(cred, 0);
            CredFree(cred);
            return res;
        }
#elif defined(__linux__)
        GError* error{ nullptr };
        char* password{ secret_password_lookup_sync(&KEYRING_SCHEMA, nullptr, &error, "application", credential.getName().c_str(), NULL) };
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
        if (error)
        {
            g_error_free(error);
        }
#endif
        return false;
    }
    
    bool SystemCredentials::deleteCredential(const std::string& name)
    {
#ifdef _WIN32
        std::wstring wName{ StringHelpers::wstr(name) };
        return CredDeleteW(wName.c_str(), CRED_TYPE_GENERIC, 0);
#elif defined(__linux__)
        GError* error{ nullptr };
        bool res{ secret_password_clear_sync(&KEYRING_SCHEMA, nullptr, &error, "application", name.c_str(), NULL) };
        if (!error)
        {
            return res;
        }
        if (error)
        {
            g_error_free(error);
        }
        return false;
#endif
    }
}