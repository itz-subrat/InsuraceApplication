#pragma once
#include <string>

class User {
private:
    int id;
    std::wstring firstName;
    std::wstring lastName;
    std::wstring email;
    std::wstring role;

public:
    User();
    User(int userId, const std::wstring& fName, const std::wstring& lName, const std::wstring& userEmail, const std::wstring& userRole);

    int getId() const;
    std::wstring getFirstName() const;
    std::wstring getLastName() const;
    std::wstring getEmail() const;
    std::wstring getRole() const;

    void setId(int userId);
    void setFirstName(const std::wstring& fName);
    void setLastName(const std::wstring& lName);
    void setEmail(const std::wstring& userEmail);
    void setRole(const std::wstring& userRole);
};
