# Nickvision::Keyring

The module contains objects for storing and managing credentials, either through a database file or within a system's credential (secret) manager.

## Table of Contents
- [Credential](#credential)
- [CredentialCheckStatus](#credentialcheckstatus)
- [Keyring](#keyring)
- [KeyringDialogController](#keyringdialogcontroller)
- [PasswordContent](#passwordcontent)
- [PasswordGenerator](#passwordgenerator)
- [PasswordStrength](#passwordstrength)
- [Store](#store)
- [SystemCredentials](#systemcredentials)

## Credential
Description: A model of a credential stored in a keyring.

Interface: [credential.h](/include/keyring/credential.h)

Type: `class`

Path: `Nickvision::Keyring::Credential`

### Member Variables
- ```
  int Id: get
  ```
	- The id of the credential.
	- Note: This value is the hash of a random guid. It is only meant to be used for storing the Credential in a database setting, and hence is readonly.
- ```
  std::string Name: get, set
  ```
	- The name of the credential.
- ```
  std::string Uri: get, set
  ```
	- The uri of the credential.
	- Note: This value may also hold a comment for the credential, which usually represents the uri of the credential.
- ```
  std::string Username: get, set
  ```
	- The username of the credential.
- ```
  std::string Password: get, set
  ```
	- The password of the credential.

### Methods
- ```cpp
  bool operator<(const Credential& compare) const
  ```
	- Accepts: The credential object to compare to this object, compare.
    - Returns: `true` if this < compare.
    - Returns: `false` if this >= compare.
- ```cpp
  bool operator>(const Credential& compare) const
  ```
	- Accepts: The credential object to compare to this object, compare.
    - Returns: `true` if this > compare.
    - Returns: `false` if this <= compare.
- ```cpp
  bool operator==(const Credential& compare) const
  ```
	- Accepts: The credential object to compare to this object, compare.
    - Returns: `true` if this == compare.
    - Returns: `false` if this != compare.
- ```cpp
  bool operator!=(const Credential& compare) const
  ```
	- Accepts: The credential object to compare to this object, compare.
    - Returns: `true` if this != compare.
    - Returns: `false` if this == compare.
- ```cpp
  friend std::ostream& operator<<(std::ostream& os, const Credential& credential)
  ```
    - Accepts: The ostream to output the credential to, os, and the Credential object to output, credential.
    - Returns: A reference to the ostream object accepted by the function.

## CredentialCheckStatus
Description: Flags to describe the status of a validated credential.

Interface: [credentialcheckstatus.h](/include/keyring/credentialcheckstatus.h)

Type: `enum class`

Path: `Nickvision::Keyring::CredentialCheckStatus`

### Values
- ```cpp
  Valid = 1
  ```
    - Represents that the credential is valid.
- ```cpp
  EmptyName = 2
  ```
    - Represents that the credential is missing a name.
- ```cpp
  EmptyUsernamePassword = 4
  ```
    - Represents that the credential is missing either a username or a password.
- ```cpp
  InvalidUri = 8
  ```
    - Represents that the credential's url is incorrectly formatted.

## Keyring
Description: A model of a keyring object for managing credentials.

Interface: [keyring.h](/include/keyring/keyring.h)

Type: `class`

Path: `Nickvision::Keyring::Keyring`

### Member Variables
- ```
  std::string Name: get
  ```
    - The name of the keyring.

### Methods
- ```cpp
  std::vector<Credential> getAllCredentials() const
  ``` 
    - Returns: The list of all credentials stored in the keyring.
- ```cpp
  std::optional<Credential> getCredential(int id)
  ```
    - Accepts: The id of a credential to find, id.
    - Returns: The `Credential` object with the matching id in the keyring.
    - Returns: `std::nullopt` if the credential was not found.
- ```cpp
  std::vector<Credential> getCredentials(const std::string& name) const
  ``` 
    - Accepts: The name of credentials to find, name.
    - Returns: The list of all credentials stored in the keyring with a name matching name.
- ```cpp
  bool addCredential(const Credential& credential)
  ```
    - Accepts: The credential object to add to the keyring, credential.
    - Returns: `true` if credential was added to the keyring.
    - Returns: `false` if credential was not added to the keyring.
- ```cpp
  bool updateCredential(const Credential& credential)
  ```
    - Accepts: The credential object to update in the keyring, credential.
    - Returns: `true` if credential was updated in the keyring.
    - Returns: `false` if credential was not updated in the keyring.
- ```cpp
  bool deleteCredential(int id)
  ``` 
    - Accepts: The id of the credential to remove from the keyring, id.
    - Returns: `true` if the credential was removed from the keyring.
    - Returns: `false` if the credential was not removed from the kering.
- ```cpp
  bool destroy()
  ``` 
    - Returns: `true` if the keyring and all of its data from disk were deleted.
    - Returns: `false` if destroying the keyring failed.
    - Note: Once this method is called, the object should no longer be referenced, regardless of what was returned.

### Static Functions
- ```cpp
  static std::optional<Keyring> access(const std::string& name, std::string password = "")
  ```
    - Accepts: The name of the keyring to access, name, and the password of the keyring, password, if known. If password is not known, it will be retrieved from the system's credential store.
    - Returns: The `Keyring` object of the successfully loaded keyring.
    - Returns: `std::nullopt` if accessing the keyring failed.
    - Note: The Keyring will first attempt to load an existing Store. If the Store doesn't exist, it will create a new Store.
- ```cpp
  static bool exists(const std::string& name)
  ``` 
    - Accepts: The name of a keyring, name.
    - Returns: `true` if a keyring with name exists.
    - Returns: `false` if a keyring with name does not exist.
- ```cpp
  static bool destroy(const std::string& name)
  ``` 
    - Accepts: The name of a keyring to destroy, name.
    - Returns: `true` if the keyring and all of its data from disk were deleted.
    - Returns: `false` if destroying the keyring failed.

## KeyringDialogController
Description: A controller for a KeyringDialog. 

Interface: [keyringdialogcontroller.h](/include/keyring/keyringdialogcontroller.h)

Type: `class`

Path: `Nickvision::Keyring::KeyringDialogController`

### Member Variables
- ```
  std::optional<Keyring> Keyring: get
  ``` 
    - The keyring managed by the controller. `std::nullopt` if no keyring is being managed by the controller.
- ```
  bool isEnabled: get
  ```  
    - Whether or not the keyring is enabled (unlocked).
- ```
  bool isValid: get
  ```  
    - Whether or not the keyring state is valid.

### Methods
- ```cpp
  bool enableKeyring(const std::string& password = "")
  ``` 
    - Accepts: The password of the keyring, password, if known. If password is not known, it will be retrieved from the system's credential store.
    - Returns: `true` if the keyring was enabled.
    - Returns: `false` if the keyring was not enabled.
- ```cpp
  bool disableKeyring()
  ``` 
    - Returns: `true` if the keyring was disabled and its data was destroyed.
    - Returns: `false` if the keyring was not disabled.
- ```cpp
  bool resetKeyring()
  ```
    - Returns: `true` if the keyring was reset and its data was destroyed.
    - Returns: `false` if the keyring was not reset.
    - Note: This method is to be used only if the keyring is locked. If unlocked, use `disableKeyring()`.
- ```cpp
  CredentialCheckStatus validateCredential(const Credential& credential) const
  ``` 
    - Accepts: The `Credential` object to validate, credential.
    - Returns: The `CredentialCheckStatus` flag for the validation of credential.
- ```cpp
  std::vector<Credential> getAllCredentials() const
  ``` 
    - Returns: The list of all credentials stored in the keyring.
- ```cpp
  bool addCredential(const Credential& credential)
  ```
    - Accepts: The credential object to add to the keyring, credential.
    - Returns: `true` if credential was added to the keyring.
    - Returns: `false` if credential was not added to the keyring.
- ```cpp
  bool updateCredential(const Credential& credential)
  ```
    - Accepts: The credential object to update in the keyring, credential.
    - Returns: `true` if credential was updated in the keyring.
    - Returns: `false` if credential was not updated in the keyring.
- ```cpp
  bool deleteCredential(int id)
  ``` 
    - Accepts: The id of the credential to remove from the keyring, id.
    - Returns: `true` if the credential was removed from the keyring.
    - Returns: `false` if the credential was not removed from the kering.

## PasswordContent
Description: Flags to describe the content of a password.

Interface: [passwordcontent.h](/include/keyring/passwordcontent.h)

Type: `enum class`

Path: `Nickvision::Keyring::PasswordContent`

### Values
- ```
  Numeric = 1
  ``` 
    - Specifies that a password should contain numeric characters.
- ```
  Uppercase = 2
  ``` 
    - Specifies that a password should contain uppercase characters.
- ```
  Lowercase = 4
  ``` 
    - Specifies that a password should contain lowercase characters.
- ```
  Special = 8
  ``` 
    - Specifies that a password should contain special characters (i.e. @, #, $, etc...).

## PasswordGenerator
Description: A random password generator.

Interface: [passwordgenerator.h](/include/keyring/passwordgenerator.h)

Type: `class`

Path: `Nickvision::Keyring::PasswordGenerator`

### Member Variables
- ```
  Nickvision::Keyring::PasswordContent ContentFlags: get, set
  ``` 
    - The flags of possible characters in a generated password.

### Methods
- ```cpp
  PasswordGenerator(PasswordContent contentFlags = PasswordContent::Numeric | PasswordContent::Uppercase | PasswordContent::Lowercase | PasswordContent::Special)
  ``` 
    - Constructs a PasswordGenerator
    - Accepts: Flags of what the generated password should contain, contentFlags.
- ```cpp
  std::string next(size_t length = 16)
  ```
    - Accepts: The number of characters for the generated password, length.
    - Returns: The new generated password of length length with characters specified by ContentFlags.

## PasswordStrength
Description: Strengths for a password. 

Interface: [passwordstrength.h](/include/keyring/passwordstrength.h)

Type: `enum class`

Path: `Nickvision::Keyring::PasswordStrength`

### Values
- ```
  Blank = 0
  ```  
    - Represents a blank password.
- ```
  VeryWeak = 1
  ```  
    - Represents a very weak password.
- ```
  Weak = 2
  ```  
    - Represents a weak password.
- ```
  Medium = 3
  ```  
    - Represents a medium password.
- ```
  Strong = 4
  ```  
    - Represents a strong password.
- ```
  VeryStrong = 5
  ```  
    - Represents a very strong password.

### Functions
- ```cpp
  PasswordStrength getPasswordStrength(const std::string& password)
  ``` 
    - Accepts: A password who's strength will be tested, password.
    - Returns: The `PasswordStrength` result for password.

## Store
Description: A store object for credentials. Backed by sqlcipher.

Interface: [store.h](/include/keyring/store.h)

Type: `class`

Path: `Nickvision::Keyring::Store`

### Member Variables
- ```
  std::string Name: get
  ```
    - The name of the store.
- ```
  std::string Path: get
  ```
    - The path of the store on disk.

### Methods
- ```cpp
  Store(const std::string& name, const std::string& password);
  ``` 
    - Constructs a Store.
    - Accepts: The name of the store, name, and the password for the store, password.
- ```cpp
  std::vector<Credential> getAllCredentials() const
  ``` 
    - Returns: The list of all credentials stored in the store.
- ```cpp
  std::optional<Credential> getCredential(int id)
  ```
    - Accepts: The id of a credential to find, id.
    - Returns: The `Credential` object with the matching id in the store.
    - Returns: `std::nullopt` if the credential was not found.
- ```cpp
  std::vector<Credential> getCredentials(const std::string& name) const
  ``` 
    - Accepts: The name of credentials to find, name.
    - Returns: The list of all credentials stored in the store with a name matching name.
- ```cpp
  bool addCredential(const Credential& credential)
  ```
    - Accepts: The credential object to add to the store, credential.
    - Returns: `true` if credential was added to the store.
    - Returns: `false` if credential was not added to the store.
- ```cpp
  bool updateCredential(const Credential& credential)
  ```
    - Accepts: The credential object to update in the store, credential.
    - Returns: `true` if credential was updated in the store.
    - Returns: `false` if credential was not updated in the store.
- ```cpp
  bool deleteCredential(int id)
  ``` 
    - Accepts: The id of the credential to remove from the store, id.
    - Returns: `true` if the credential was removed from the store.
    - Returns: `false` if the credential was not removed from the kering.
- ```cpp
  bool destroy()
  ``` 
    - Returns: `true` if the store and all of its data from disk were deleted.
    - Returns: `false` if destroying the store failed.
    - Note: Once this method is called, the object should no longer be referenced, regardless of what was returned.

### Static Functions
- ```cpp
  static bool exists(const std::string& name)
  ``` 
    - Accepts: The name of a store, name.
    - Returns: `true` if a store with name exists.
    - Returns: `false` if a store with name does not exist.
- ```cpp
  static bool destroy(const std::string& name)
  ``` 
    - Accepts: The name of a store to destroy, name.
    - Returns: `true` if the store and all of its data from disk were deleted.
    - Returns: `false` if destroying the store failed.

## SystemCredentials
Description: Functions for managing credentials in the system's credential (secret) store.

Note: Utilizes `libsecret` on Linux and `Windows Credential Manager` on Windows.

Interface: [systemcredentials.h](/include/keyring/systemcredentials.h)

Type: `namespace`

Path: `Nickvision::Keyring::SystemCredentials`

### Functions
- ```cpp
  std::optional<Credential> getCredential(const std::string& name)
  ```
    - Accepts: The name of a credential to retrieve from the store, name.
    - Returns: The `Credential` object for the credential matching name.
    - Returns: `std::nullopt` if no matching credential was found.
- ```cpp
  std::optional<Credential> addCredential(const std::string& name)
  ```
    - Accepts: The name of a credential to add to the store, name.
    - Returns: The `Credential` object for the new added credential. This credential will contain a randomly generated password from `PasswordGenerator`.
    - Returns: `std::nullopt` if the credential was unable to be added to the store.
- ```cpp
  bool addCredential(const Credential& credential)
  ``` 
    - Accepts: A `Credential` object to add to the store, credential.
    - Returns: `true` if the credential was added to the store.
    - Returns: `false` if the credential was not added to the store.
    - Note: On Linux, only the Name and Password fields of the credential will be stored. Uri and Username will be ignored.
    - Note: On Windows, all fields of the credential will be stored.
- ```cpp
  bool updateCredential(const Credential& credential)
  ``` 
    - Accepts: The `Credential` object to update in the store, credential.
    - Returns: `true` if the credential was updated in the store.
    - Returns: `false` if the credential was not updated in the store.
    - Note: On Linux, only the Name and Password fields of the credential will be stored. Uri and Username will be ignored.
    - Note: On Windows, all fields of the credential will be stored.
- ```cpp
  bool deleteCredential(const std::string& name)
  ``` 
    - Accepts: The name of a credential to remove from the store, name.
    - Returns: `true` if the credential was removed from the store.
    - Returns: `false` if the credential was not removed from the store.