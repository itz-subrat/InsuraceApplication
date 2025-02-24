#include "ClientManagement.h"
#include "PolicyProposalManagement.h"
#include <iostream>

ClientManagement::ClientManagement(SQLHANDLE connHandle) : sqlConnHandle(connHandle) {}

bool ClientManagement::addClient(const std::wstring& name, const std::wstring& dob, const std::wstring& gender,
    double income, const std::wstring& city, const std::wstring& occupation,
    const std::wstring& educationQualification) {

    SQLHANDLE sqlStmtHandle;
    SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);

    std::wstring query = L"INSERT INTO Clients (name, dob, gender, income, city, occupation, educational_qualification) "
        L"VALUES (?, ?, ?, ?, ?, ?, ?)";

    SQLPrepareW(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS);
    SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 100, 0, (SQLWCHAR*)name.c_str(), 0, NULL);
    SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 10, 0, (SQLWCHAR*)dob.c_str(), 0, NULL);
    SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 10, 0, (SQLWCHAR*)gender.c_str(), 0, NULL);
    SQLBindParameter(sqlStmtHandle, 4, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_FLOAT, 0, 0, &income, 0, NULL);
    SQLBindParameter(sqlStmtHandle, 5, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 100, 0, (SQLWCHAR*)city.c_str(), 0, NULL);
    SQLBindParameter(sqlStmtHandle, 6, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 100, 0, (SQLWCHAR*)occupation.c_str(), 0, NULL);
    SQLBindParameter(sqlStmtHandle, 7, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 100, 0, (SQLWCHAR*)educationQualification.c_str(), 0, NULL);

    bool success = (SQLExecute(sqlStmtHandle) == SQL_SUCCESS);
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return success;
}

std::vector<Client> ClientManagement::searchClient(const std::wstring& query) {
    std::vector<Client> clients;
    SQLHANDLE sqlStmtHandle;
    SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);

    std::wstring sqlQuery = L"SELECT id, name, dob, gender, income, city, occupation, educational_qualification, created_at "
        L"FROM Clients WHERE name LIKE ? OR id = ?";

    SQLPrepareW(sqlStmtHandle, (SQLWCHAR*)sqlQuery.c_str(), SQL_NTS);
    std::wstring likeQuery = L"%" + query + L"%";
    int id = 0;
    try {
        id = std::stoi(query, nullptr, 10);  // Try parsing the query as an ID (integer)
    }
    catch (const std::invalid_argument&) {
        id = -1;  // If it's not a valid integer, set ID to -1
    }

    SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 100, 0, (SQLWCHAR*)likeQuery.c_str(), 0, NULL);
    SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &id, 0, NULL);

    // Execute the query
    if (SQLExecute(sqlStmtHandle) == SQL_SUCCESS) {
        int fetchedId;
        wchar_t name[100], dob[11], gender[10], city[100], occupation[100], educationQualification[100], createdAt[20];
        double income;
        SQLLEN indicator;

        // Bind columns to fetch the results
        SQLBindCol(sqlStmtHandle, 1, SQL_C_LONG, &fetchedId, 0, &indicator);
        SQLBindCol(sqlStmtHandle, 2, SQL_C_WCHAR, name, sizeof(name), &indicator);
        SQLBindCol(sqlStmtHandle, 3, SQL_C_WCHAR, dob, sizeof(dob), &indicator);
        SQLBindCol(sqlStmtHandle, 4, SQL_C_WCHAR, gender, sizeof(gender), &indicator);
        SQLBindCol(sqlStmtHandle, 5, SQL_C_DOUBLE, &income, 0, &indicator);
        SQLBindCol(sqlStmtHandle, 6, SQL_C_WCHAR, city, sizeof(city), &indicator);
        SQLBindCol(sqlStmtHandle, 7, SQL_C_WCHAR, occupation, sizeof(occupation), &indicator);
        SQLBindCol(sqlStmtHandle, 8, SQL_C_WCHAR, educationQualification, sizeof(educationQualification), &indicator);
        SQLBindCol(sqlStmtHandle, 9, SQL_C_WCHAR, createdAt, sizeof(createdAt), &indicator);
        std::wcout << L"-------------------------------" << std::endl;

        // Fetch the results and print
        if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS_WITH_INFO) {
            std::wcout << L"ID: " << fetchedId << std::endl;
            std::wcout << L"Name: " << name << std::endl;
            std::wcout << L"DOB: " << dob << std::endl;
            std::wcout << L"Gender: " << gender << std::endl;
            std::wcout << L"Income: " << income << std::endl;
            std::wcout << L"City: " << city << std::endl;
            std::wcout << L"Occupation: " << occupation << std::endl;
            std::wcout << L"Education: " << educationQualification << std::endl;
            std::wcout << L"Created At: " << createdAt << std::endl;
            std::wcout << L"-------------------------------" << std::endl;

            // Store the client into the vector
            clients.emplace_back(fetchedId, name, dob, gender, income, city, occupation, educationQualification);
            if (true) {
                int choice = 0;
                std::wcout << L"\n1. Add Policy To the client\n2. Show Policies of the client\n ";
                std::cin >> choice;

                if (choice == 1) {
                    std::wstring insuranceType, paymentMode, policyNumber, startDate, endDate;
                    double lifeCoverAmount = 0.0, monthlyPremium = 0.0;
                    int coverageAge = 0, tobaccoConsumption = 0, paymentTenure = 0, createdBy = 0;

                    std::wcin.ignore();  // Ignore leftover input from previous operations

                    std::wcout << L"Enter Insurance Type: ";
                    std::getline(std::wcin, insuranceType);

                    std::wcout << L"Enter Life Cover Amount: ";
                    while (!(std::wcin >> lifeCoverAmount)) {  // Validate numeric input
                        std::wcin.clear();
                        std::wcin.ignore(10000, '\n');
                        std::wcout << L"Invalid input. Enter a valid Life Cover Amount: ";
                    }

                    std::wcout << L"Enter Coverage Age: ";
                    while (!(std::wcin >> coverageAge)) {
                        std::wcin.clear();
                        std::wcin.ignore(10000, '\n');
                        std::wcout << L"Invalid input. Enter a valid Coverage Age: ";
                    }

                    std::wcout << L"Does the client consume tobacco? (1 = Yes, 0 = No): ";
                    while (!(std::wcin >> tobaccoConsumption) || (tobaccoConsumption != 0 && tobaccoConsumption != 1)) {
                        std::wcin.clear();
                        std::wcin.ignore(10000, '\n');
                        std::wcout << L"Invalid input. Enter 1 for Yes, 0 for No: ";
                    }


                    std::wcout << L"Enter Payment Tenure (in years): ";
                    while (!(std::wcin >> paymentTenure)) {
                        std::wcin.clear();
                        std::wcin.ignore(10000, '\n');
                        std::wcout << L"Invalid input. Enter a valid Payment Tenure: ";
                    }

                    std::wcin.ignore();  // Clear the newline character left in buffer

                    double riskFactor = (tobaccoConsumption == 1) ? 1.2 : 1.0;

                    // Calculate Monthly Premium
                    monthlyPremium = (lifeCoverAmount / (paymentTenure * 12)) * riskFactor;

                    std::wcout << L"Calculated Monthly Premium: " << monthlyPremium << L"\n";

                    std::wcin.ignore();

                    int paymentChoice;

                    std::wcout << L"Select Payment Mode:\n";
                    std::wcout << L"1. Monthly\n";
                    std::wcout << L"2. Yearly\n";
                    std::wcout << L"3. Half-Yearly\n";
                    std::wcout << L"Enter your choice (1/2/3): ";

                    while (!(std::wcin >> paymentChoice) || (paymentChoice < 1 || paymentChoice > 3)) {
                        std::wcin.clear();
                        std::wcin.ignore(10000, '\n');
                        std::wcout << L"Invalid input. Enter 1 for Monthly, 2 for Yearly, or 3 for Half-Yearly: ";
                    }

                    // Assign the selected payment mode
                    switch (paymentChoice) {
                    case 1:
                        paymentMode = L"Monthly";
                        break;
                    case 2:
                        paymentMode = L"Yearly";
                        break;
                    case 3:
                        paymentMode = L"Half-Yearly";
                        break;
                    }

                    // Display selected mode
                    std::wcout << L"Selected Payment Mode: " << paymentMode << L"\n";


                    std::wcout << L"Enter Policy Number: ";
                    std::getline(std::wcin, policyNumber);

                    std::wcout << L"Enter Start Date (YYYY-MM-DD): ";
                    std::getline(std::wcin, startDate);

                    std::wcout << L"Enter End Date (YYYY-MM-DD): ";
                    std::getline(std::wcin, endDate);

                    std::wcout << L"Enter Created By (User ID of creator): ";
                    while (!(std::wcin >> createdBy)) {
                        std::wcin.clear();
                        std::wcin.ignore(10000, '\n');
                        std::wcout << L"Invalid input. Enter a valid User ID: ";
                    }

                    // Output for verification
                    std::wcout << L"\nPolicy Details Entered:\n";
                    std::wcout << L"Insurance Type: " << insuranceType << std::endl;
                    std::wcout << L"Life Cover Amount: " << lifeCoverAmount << std::endl;
                    std::wcout << L"Coverage Age: " << coverageAge << std::endl;
                    std::wcout << L"Tobacco Consumption: " << (tobaccoConsumption ? L"Yes" : L"No") << std::endl;
                    std::wcout << L"Monthly Premium: " << monthlyPremium << std::endl;
                    std::wcout << L"Payment Tenure: " << paymentTenure << L" years" << std::endl;
                    std::wcout << L"Payment Mode: " << paymentMode << std::endl;
                    std::wcout << L"Policy Number: " << policyNumber << std::endl;
                    std::wcout << L"Start Date: " << startDate << std::endl;
                    std::wcout << L"End Date: " << endDate << std::endl;
                    std::wcout << L"Created By (User ID): " << createdBy << std::endl;

                    PolicyProposalManagement policyMgr(sqlConnHandle);
                    bool success = policyMgr.addPolicy(fetchedId, insuranceType, lifeCoverAmount, coverageAge,
                        tobaccoConsumption, monthlyPremium, paymentTenure, paymentMode, createdBy,
                        policyNumber, startDate, endDate);
                    if (success) {
                        std::wcout << L"Policy added successfully.\n";
                    }
                    else {
                        std::wcout << L"Failed to add policy.\n";
                    }
                }
                else if (choice == 2) {
                    // Show Policies for the client
                    PolicyProposalManagement policyMgr(sqlConnHandle);
                    policyMgr.searchPolicy(fetchedId);
                }
            }
        }
    }
    else {
        std::wcerr << L"SQL query execution failed." << std::endl;
    }

    // Free the SQL statement handle
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return clients;
}
