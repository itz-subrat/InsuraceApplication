#pragma once
#ifndef USER_MANAGEMENT_H
#define USER_MANAGEMENT_H

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <string>

class UserManagement {
private:
    SQLHANDLE sqlConnHandle; // Database connection handle

public:
    // Constructor
    UserManagement(SQLHANDLE sqlConnHandle);

    // User signup function
    bool SignUpUser(const std::wstring& username, const std::wstring& password, const std::wstring& role);

    // Additional functions (if needed)
    bool AuthenticateUser(const std::wstring& username, const std::wstring& password);
    bool DeleteUser(const std::wstring& username);
};

#endif // USER_MANAGEMENT_H
