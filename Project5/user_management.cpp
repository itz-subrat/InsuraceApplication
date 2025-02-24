#include "user_management.h"
#include <iostream>
#include <cwchar>

// Constructor
UserManagement::UserManagement(SQLHANDLE sqlConnHandle) {
    this->sqlConnHandle = sqlConnHandle;
}

// Function to sign up a new user
bool UserManagement::SignUpUser(const std::wstring& username, const std::wstring& password, const std::wstring& role) {
    SQLHANDLE sqlStmtHandle;
    SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);

    std::wstring query = L"INSERT INTO users (Username, Password, Role) VALUES (?, ?, ?)";

    if (SQLPrepareW(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS) != SQL_SUCCESS) {
        std::cerr << "Error preparing SQL statement for user signup!\n";
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }

    // Bind parameters
    SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 255, 0, (SQLWCHAR*)username.c_str(), 0, NULL);
    SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 255, 0, (SQLWCHAR*)password.c_str(), 0, NULL);
    SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLWCHAR*)role.c_str(), 0, NULL);

    if (SQLExecute(sqlStmtHandle) != SQL_SUCCESS) {
        std::cerr << "Error executing user signup query!\n";
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    std::wcout << L"User signed up successfully!\n";
    return true;
}
