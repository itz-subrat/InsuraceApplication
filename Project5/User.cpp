#include "User.h"

// Default constructor
User::User() : id(0), firstName(L""), lastName(L""), email(L""), role(L"") {}

// Parameterized constructor
User::User(int userId, const std::wstring& fName, const std::wstring& lName, const std::wstring& userEmail, const std::wstring& userRole)
    : id(userId), firstName(fName), lastName(lName), email(userEmail), role(userRole) {
}

// Getter functions (Make sure each function is only defined once)
int User::getId() const { return id; }
std::wstring User::getFirstName() const { return firstName; }
std::wstring User::getLastName() const { return lastName; }
std::wstring User::getEmail() const { return email; }
std::wstring User::getRole() const { return role; }

// Setter functions
