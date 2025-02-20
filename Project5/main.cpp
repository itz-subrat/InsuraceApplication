#include <iostream>
#include <string>
#include <cwchar>
#include "db_connection.h"
#include "authentication.h"

int main() {
    bool useWindowsAuth;
    std::cout << "Use Windows Authentication? (1 = Yes, 0 = No): ";
    std::cin >> useWindowsAuth;
    std::cin.ignore();

    SQLHANDLE sqlConnHandle = ConnectToSQLServer(useWindowsAuth);

    if (sqlConnHandle) {
        std::wstring username, password, role;
        std::wcout << L"Enter Username: ";
        std::wcin >> username;
        std::wcout << L"Enter Password: ";
        std::wcin >> password;

        if (AuthenticateUser(sqlConnHandle, username, password, role)) {
            std::wcout << L"Welcome, " << username << L"!\n";
            std::wcout << L"Your role is: " << role << L"\n";

            // Assign functionality based on role
            if (role == L"Admin") {
                std::wcout << L"Admin dashboard functionality\n";
            }
            else if (role == L"User") {
                std::wcout << L"User dashboard functionality\n";
            }
            else {
                std::wcout << L"Unknown role\n";
            }
        }
        else {
            std::wcout << L"Invalid username or password.\n";
        }

        DisconnectFromSQLServer(sqlConnHandle);
    }

    return 0;
}