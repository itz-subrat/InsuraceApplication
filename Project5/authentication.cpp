#include "authentication.h"
#include <iostream>
#include <cwchar>

bool AuthenticateUser(SQLHANDLE sqlConnHandle, const std::wstring& username, const std::wstring& password, std::wstring& role) {
    SQLHANDLE sqlStmtHandle;
    SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);

    std::wstring query = L"SELECT Role FROM users WHERE Username = '" + username + L"' AND Password = '" + password + L"'";
    if (SQL_SUCCESS != SQLExecDirectW(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS)) {
        std::cerr << "Error executing query!\n";
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }

    SQLWCHAR roleData[256];
    SQLLEN dataLen;
    if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
        SQLGetData(sqlStmtHandle, 1, SQL_C_WCHAR, roleData, sizeof(roleData), &dataLen);
        role = roleData;
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return true;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return false;
}