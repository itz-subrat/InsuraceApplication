#include "Authentication.h"
#include <iostream>
#include "UnderwriterManagement.h"
#include "FrontdeskManagement.h"
#include "ManagerManagement.h"

bool Authentication::AuthenticateUser(SQLHANDLE sqlConnHandle, const std::wstring& email, const std::wstring& password, User& authenticatedUser) {
    SQLHANDLE sqlStmtHandle;
	std::wcout << L"Password: " << password << L"\n";
    SQLWCHAR sqlQuery[] = L"SELECT id, first_name, last_name, role FROM Users WHERE email = ? and password = ? ";

    if (SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle) != SQL_SUCCESS) {
        std::wcerr << L"Error allocating statement handle.\n";
        return false;
    }

    if (SQLPrepare(sqlStmtHandle, sqlQuery, SQL_NTS) != SQL_SUCCESS) {
        std::wcerr << L"Error preparing SQL query.\n";
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }

    if (SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 100, 0, (SQLWCHAR*)email.c_str(), 0, NULL) != SQL_SUCCESS) {
        std::wcerr << L"Error binding email parameter.\n";
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }

    if (SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 100, 0, (SQLWCHAR*)password.c_str(), 0, NULL) != SQL_SUCCESS) {
        std::wcerr << L"Error binding password parameter.\n";
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }

    if (SQLExecute(sqlStmtHandle) != SQL_SUCCESS) {
        std::wcerr << L"Error executing query.\n";
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }

    int id;
    wchar_t firstName[50], lastName[50], role[50];

    SQLBindCol(sqlStmtHandle, 1, SQL_C_LONG, &id, 0, NULL);
    SQLBindCol(sqlStmtHandle, 2, SQL_C_WCHAR, firstName, sizeof(firstName), NULL);
    SQLBindCol(sqlStmtHandle, 3, SQL_C_WCHAR, lastName, sizeof(lastName), NULL);
    SQLBindCol(sqlStmtHandle, 4, SQL_C_WCHAR, role, sizeof(role), NULL);

    if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
        authenticatedUser = User(id, std::wstring(firstName), std::wstring(lastName), email, std::wstring(role));
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return true;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return false;
}

void SignIn(SQLHANDLE sqlConnHandle) {
    std::wstring email, password;

    std::wcout << L"Enter Email: ";
    std::wcin.ignore();
    std::getline(std::wcin, email);

    std::wcout << L"Enter Password: ";
    std::getline(std::wcin, password);

    User authenticatedUser;

    if (Authentication::AuthenticateUser(sqlConnHandle, email, password, authenticatedUser)) {
        std::wcout << L"Welcome, " << authenticatedUser.getFirstName() << L" " << authenticatedUser.getLastName() << L"!\n";
        std::wcout << L"Your role is: " << authenticatedUser.getRole() << L"\n";

        // Role-based redirection
        if (authenticatedUser.getRole() == L"FrontDesk") {
            std::wcout << L"Redirecting to FrontDesk Dashboard...\n";
			FrontDeskManagement frontDesk(sqlConnHandle);
			frontDesk.FrontDeskDashboard();
            // Call FrontDeskDashboard() when implemented
        }
        else if (authenticatedUser.getRole() == L"UnderWriter") {
            UnderwriterManagement underwriter(sqlConnHandle);
            underwriter.UnderwriterDashboard();
        }
        else if (authenticatedUser.getRole() == L"Manager") {
            std::wcout << L"Redirecting to Manager Dashboard...\n";
            // Call ManagerDashboard() when implemented
			ManagerManagement manager(sqlConnHandle);
			manager.ManagerDashboard();
        }
        else if (authenticatedUser.getRole() == L"SuperManager") {
            std::wcout << L"Redirecting to SuperManager Dashboard...\n";
            // Call SuperManagerDashboard() when implemented
        }
        else {
            std::wcout << L"Unknown role, access denied.\n";
        }
    }
    else {
        std::wcout << L"Invalid email or password.\n";
    }
}
