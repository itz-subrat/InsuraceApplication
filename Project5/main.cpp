#include <iostream>
#include <string>
#include "db_connection.h"
#include "Authentication.h"
#include "ClientManagement.h"



//void addClientFlow(SQLHANDLE sqlConnHandle) {
//    std::wstring name, dob, gender, city, occupation, educationQualification;
//    double income;
//
//    std::wcin.ignore();
//
//    std::wcout << L"Enter Name: ";
//    std::getline(std::wcin, name);
//
//    std::wcout << L"Enter Date of Birth (YYYY-MM-DD): ";
//    std::getline(std::wcin, dob);
//
//    std::wcout << L"Enter Gender: ";
//    std::getline(std::wcin, gender);
//
//    std::wcout << L"Enter Income: ";
//    std::wcin >> income;
//    std::wcin.ignore();
//
//    std::wcout << L"Enter City: ";
//    std::getline(std::wcin, city);
//
//    std::wcout << L"Enter Occupation: ";
//    std::getline(std::wcin, occupation);
//
//    std::wcout << L"Enter Education Qualification: ";
//    std::getline(std::wcin, educationQualification);
//
//    ClientManagement clientMgr(sqlConnHandle);
//    if (clientMgr.addClient(name, dob, gender, income, city, occupation, educationQualification)) {
//        std::wcout << L"Client added successfully.\n";
//    }
//    else {
//        std::wcout << L"Failed to add client.\n";
//    }
//}
//
//void searchClientFlow(SQLHANDLE sqlConnHandle) {
//    std::wstring searchQuery;
//    std::wcout << L"Enter client name or ID to search: ";
//    std::wcin.ignore();
//    std::getline(std::wcin, searchQuery);
//
//    ClientManagement clientMgr(sqlConnHandle);
//    clientMgr.searchClient(searchQuery);
//}

int main() {
    bool useWindowsAuth;
    std::cout << "Use Windows Authentication? (1 = Yes, 0 = No): ";
    std::cin >> useWindowsAuth;

    SQLHANDLE sqlConnHandle = ConnectToSQLServer(useWindowsAuth);
    if (!sqlConnHandle) {
        std::cerr << "Database connection failed. Exiting...\n";
        return 1;
    }

    int choice;
    while (true) {
        std::wcout << L"\n1. Sign In\n2. Exit\nChoose an option: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            SignIn(sqlConnHandle);
            break;
        case 2:
            DisconnectFromSQLServer(sqlConnHandle);
            return 0;
        default:
            std::wcout << L"Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
