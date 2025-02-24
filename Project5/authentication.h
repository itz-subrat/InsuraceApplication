#pragma once

#include "db_connection.h"
#include "User.h"
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <string>

class Authentication {
public:
    static bool AuthenticateUser(SQLHANDLE sqlConnHandle, const std::wstring& email, const std::wstring& password, User& authenticatedUser);
};

// Declare SignIn function here
void SignIn(SQLHANDLE sqlConnHandle);
