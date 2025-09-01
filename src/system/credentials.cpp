#include "system/credentials.h"
#include <vector>
#include "helpers/stringhelpers.h"
#include "keyring/passwordgenerator.h"
#ifdef _WIN32
#include <windows.h>
#include <wincred.h>
#elif defined(__APPLE__) && !defined(APPLE_USE_LIBSECRET)
#include <CoreFoundation/CoreFoundation.h>
#include <Security/Security.h>
#else
#include <libsecret/secret.h>
#endif

using namespace Nickvision::Helpers;
using namespace Nickvision::Keyring;

namespace Nickvision::System
{
#if !defined(_WIN32) && (!defined(__APPLE__) || defined(APPLE_USE_LIBSECRET))
    static const SecretSchema KEYRING_SCHEMA = { "org.nickvision.libnick", SECRET_SCHEMA_NONE, { { "application", SECRET_SCHEMA_ATTRIBUTE_STRING }, { "NULL", SECRET_SCHEMA_ATTRIBUTE_STRING } } };
#endif

    std::optional<Credential> Credentials::get(const std::string& name) noexcept
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
#elif defined(__APPLE__) && !defined(APPLE_USE_LIBSECRET)
        CFStringRef nameRef{ CFStringCreateWithCString(nullptr, name.c_str(), kCFStringEncodingUTF8) };
        CFMutableDictionaryRef query{ CFDictionaryCreateMutable(nullptr, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks) };
        CFDictionaryAddValue(query, kSecClass, kSecClassGenericPassword);
        CFDictionaryAddValue(query, kSecAttrService, nameRef);
        CFDictionaryAddValue(query, kSecMatchLimit, kSecMatchLimitOne);
        CFDictionaryAddValue(query, kSecReturnAttributes, kCFBooleanTrue);
        CFDictionaryAddValue(query, kSecReturnData, kCFBooleanTrue);
        CFTypeRef result{ nullptr };
        if(SecItemCopyMatching(query, &result) == errSecSuccess)
        {
            CFDictionaryRef attributes{ reinterpret_cast<CFDictionaryRef>(result) };
            CFStringRef username{ reinterpret_cast<CFStringRef>(CFDictionaryGetValue(attributes, kSecAttrAccount)) };
            std::vector<char> usernameBuffer(username ? static_cast<size_t>(CFStringGetLength(username) + 1) : static_cast<size_t>(0));
            if(username)
            {
                CFStringGetCString(username, usernameBuffer.data(), usernameBuffer.size(), kCFStringEncodingUTF8);
            }
            CFStringRef password{ CFStringCreateFromExternalRepresentation(nullptr, reinterpret_cast<CFDataRef>(CFDictionaryGetValue(attributes, kSecValueData)), kCFStringEncodingUTF8) };
            std::vector<char> passwordBuffer(static_cast<size_t>(CFStringGetLength(password) + 1));
            CFStringGetCString(password, passwordBuffer.data(), passwordBuffer.size(), kCFStringEncodingUTF8);
            CFStringRef uri{ reinterpret_cast<CFStringRef>(CFDictionaryGetValue(attributes, kSecAttrComment)) };
            std::vector<char> uriBuffer(uri ? static_cast<size_t>(CFStringGetLength(uri) + 1) : static_cast<size_t>(0));
            if(uri)
            {
                CFStringGetCString(uri, uriBuffer.data(), uriBuffer.size(), kCFStringEncodingUTF8);
            }
            Credential cred{ name, uriBuffer.size() == 0 ? "" : uriBuffer.data(), usernameBuffer.size() == 0 ? "" : usernameBuffer.data(), passwordBuffer.size() == 0 ? "" : passwordBuffer.data() };
            CFRelease(password);
            CFRelease(result);
            CFRelease(query);
            CFRelease(nameRef);
            return cred;
        }
        CFRelease(query);
        CFRelease(nameRef);
#else
        GError* error{ nullptr };
        char* password{ secret_password_lookup_sync(&KEYRING_SCHEMA, nullptr, &error, "application", name.c_str(), nullptr) };
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

    std::optional<Credential> Credentials::create(const std::string& name) noexcept
    {
        PasswordGenerator passGen;
        Credential c{ name, "", "default", passGen.next(64) };
        if (add(c))
        {
            return c;
        }
        return std::nullopt;
    }

    bool Credentials::add(const Credential& credential) noexcept
    {
        if(credential.getPassword().empty())
        {
            return false;
        }
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
#elif defined(__APPLE__) && !defined(APPLE_USE_LIBSECRET)
        CFStringRef name{ CFStringCreateWithCString(nullptr, credential.getName().c_str(), kCFStringEncodingUTF8) };
        CFStringRef uri{ CFStringCreateWithCString(nullptr, credential.getUri().c_str(), kCFStringEncodingUTF8) };
        CFStringRef username{ CFStringCreateWithCString(nullptr, credential.getUsername().c_str(), kCFStringEncodingUTF8) };
        CFStringRef password{ CFStringCreateWithCString(nullptr, credential.getPassword().c_str(), kCFStringEncodingUTF8) };
        CFDataRef passwordData{ CFStringCreateExternalRepresentation(nullptr, password, kCFStringEncodingUTF8, 0) };
        CFMutableDictionaryRef query{ CFDictionaryCreateMutable(nullptr, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks) };
        CFDictionaryAddValue(query, kSecClass, kSecClassGenericPassword);
        CFDictionaryAddValue(query, kSecAttrService, name);
        CFDictionaryAddValue(query, kSecAttrAccount, username);
        CFDictionaryAddValue(query, kSecValueData, passwordData);
        CFDictionaryAddValue(query, kSecAttrComment, uri);
        CFDictionaryAddValue(query, kSecAttrSynchronizable, kCFBooleanFalse);
        OSStatus status{ SecItemAdd(query, nullptr) };
        CFRelease(passwordData);
        CFRelease(password);
        CFRelease(username);
        CFRelease(uri);
        CFRelease(name);
        CFRelease(query);
        return status == errSecSuccess;
#else
        GError* error{ nullptr };
        secret_password_store_sync(&KEYRING_SCHEMA, SECRET_COLLECTION_DEFAULT, credential.getName().c_str(), credential.getPassword().c_str(), nullptr, &error, "application", credential.getName().c_str(), nullptr);
        if (error)
        {
            g_error_free(error);
            return false;
        }
        return true;
#endif
    }

    bool Credentials::update(const Credential& credential) noexcept
    {
        if(credential.getPassword().empty())
        {
            return false;
        }
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
#elif defined(__APPLE__) && !defined(APPLE_USE_LIBSECRET)
        CFStringRef name{ CFStringCreateWithCString(nullptr, credential.getName().c_str(), kCFStringEncodingUTF8) };
        CFMutableDictionaryRef query{ CFDictionaryCreateMutable(nullptr, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks) };
        CFDictionaryAddValue(query, kSecClass, kSecClassGenericPassword);
        CFDictionaryAddValue(query, kSecAttrService, name);
        CFDictionaryAddValue(query, kSecMatchLimit, kSecMatchLimitOne);
        CFDictionaryAddValue(query, kSecReturnAttributes, kCFBooleanTrue);
        CFDictionaryAddValue(query, kSecReturnData, kCFBooleanTrue);
        CFTypeRef result{ nullptr };
        if(SecItemCopyMatching(query, &result) == errSecSuccess)
        {
            CFStringRef newUsername{ CFStringCreateWithCString(nullptr, credential.getUsername().c_str(), kCFStringEncodingUTF8) };
            CFStringRef newPassword{ CFStringCreateWithCString(nullptr, credential.getPassword().c_str(), kCFStringEncodingUTF8) };
            CFDataRef newPasswordData{ CFStringCreateExternalRepresentation(nullptr, newPassword, kCFStringEncodingUTF8, 0) };
            CFStringRef newUri{ CFStringCreateWithCString(nullptr, credential.getUri().c_str(), kCFStringEncodingUTF8) };
            CFMutableDictionaryRef updatedAttributes{ CFDictionaryCreateMutable(nullptr, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks) };
            CFDictionaryAddValue(updatedAttributes, kSecAttrAccount, newUsername);
            CFDictionaryAddValue(updatedAttributes, kSecValueData, newPasswordData);
            CFDictionaryAddValue(updatedAttributes, kSecAttrComment, newUri);
            OSStatus status{ SecItemUpdate(query, updatedAttributes) };
            CFRelease(updatedAttributes);
            CFRelease(newUri);
            CFRelease(newPasswordData);
            CFRelease(newPassword);
            CFRelease(newUsername);
            CFRelease(result);
            CFRelease(query);
            CFRelease(name);
            return status == errSecSuccess;
        }
        CFRelease(query);
        CFRelease(name);
#else
        GError* error{ nullptr };
        char* password{ secret_password_lookup_sync(&KEYRING_SCHEMA, nullptr, &error, "application", credential.getName().c_str(), nullptr) };
        if (!error && password)
        {
            secret_password_free(password);
            secret_password_store_sync(&KEYRING_SCHEMA, SECRET_COLLECTION_DEFAULT, credential.getName().c_str(), credential.getPassword().c_str(), nullptr, &error, "application", credential.getName().c_str(), nullptr);
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
    
    bool Credentials::remove(const std::string& name) noexcept
    {
#ifdef _WIN32
        std::wstring wName{ StringHelpers::wstr(name) };
        return CredDeleteW(wName.c_str(), CRED_TYPE_GENERIC, 0);
#elif defined(__APPLE__) && !defined(APPLE_USE_LIBSECRET)
        CFStringRef nameRef{ CFStringCreateWithCString(nullptr, name.c_str(), kCFStringEncodingUTF8) };
        CFMutableDictionaryRef query{ CFDictionaryCreateMutable(nullptr, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks) };
        CFDictionaryAddValue(query, kSecClass, kSecClassGenericPassword);
        CFDictionaryAddValue(query, kSecAttrService, nameRef);
        OSStatus status{ SecItemDelete(query) };
        CFRelease(query);
        CFRelease(nameRef);
        return status == errSecSuccess;
#else
        GError* error{ nullptr };
        bool res{ static_cast<bool>(secret_password_clear_sync(&KEYRING_SCHEMA, nullptr, &error, "application", name.c_str(), nullptr)) };
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