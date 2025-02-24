#include "client_management.h"
#include <iostream>
#include <cwchar>

bool client_management::create_client(dbconnection& db, const std::wstring& name, const std::wstring& dob,
    const std::wstring& gender, double income, const std::wstring& city,
    const std::wstring& occupation, const std::wstring& education) {
    SQLHANDLE sqlStmtHandle;
    SQLAllocHandle(SQL_HANDLE_STMT, db.get_connection_handle(), &sqlStmtHandle);

    std::wstring query =
        L"INSERT INTO clients (name, dob, gender, income, city, occupation, educational_qualification, created_at) "
        L"VALUES (?, ?, ?, ?, ?, ?, ?, GETDATE())";

    if (SQLPrepareW(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS) != SQL_SUCCESS) {
        std::cerr << "Error preparing SQL statement!\n";
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }

    SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 255, 0, (SQLWCHAR*)name.c_str(), 0, NULL);
    SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 10, 0, (SQLWCHAR*)dob.c_str(), 0, NULL);
    SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 10, 0, (SQLWCHAR*)gender.c_str(), 0, NULL);
    SQLBindParameter(sqlStmtHandle, 4, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DECIMAL, 10, 2, (SQLPOINTER)&income, 0, NULL);
    SQLBindParameter(sqlStmtHandle, 5, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 100, 0, (SQLWCHAR*)city.c_str(), 0, NULL);
    SQLBindParameter(sqlStmtHandle, 6, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 100, 0, (SQLWCHAR*)occupation.c_str(), 0, NULL);
    SQLBindParameter(sqlStmtHandle, 7, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 100, 0, (SQLWCHAR*)education.c_str(), 0, NULL);

    if (SQLExecute(sqlStmtHandle) != SQL_SUCCESS) {
        std::cerr << "Error executing client registration query!\n";
        SQLWCHAR sqlState[6], message[256];
        SQLINTEGER nativeError;
        SQLSMALLINT msgLength;
        SQLGetDiagRecW(SQL_HANDLE_STMT, sqlStmtHandle, 1, sqlState, &nativeError, message, sizeof(message), &msgLength);
        std::wcerr << L"SQL Error: " << message << L"\n";
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    std::wcout << L"Client successfully registered!\n";
    return true;
}
