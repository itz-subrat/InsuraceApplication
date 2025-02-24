#include "PolicyProposalManagement.h"
#include <iostream>
#include <sstream>

// Constructor to initialize database connection
PolicyProposalManagement::PolicyProposalManagement(SQLHANDLE connHandle) : sqlConnHandle(connHandle) {}

// Function to print SQL errors
void PolicyProposalManagement::printSQLError(SQLSMALLINT handleType, SQLHANDLE handle) {
    SQLWCHAR sqlState[6], message[256];
    SQLINTEGER nativeError;
    SQLSMALLINT messageLength;
    SQLRETURN ret;
    int i = 1;

    while ((ret = SQLGetDiagRecW(handleType, handle, i, sqlState, &nativeError, message, sizeof(message), &messageLength)) == SQL_SUCCESS) {
        std::wcout << L"SQL Error [" << sqlState << L"]: " << message << L" (Error Code: " << nativeError << L")" << std::endl;
        i++;
    }
}

// Function to add a policy
bool PolicyProposalManagement::addPolicy(int clientId, const std::wstring& insuranceType, double lifeCoverAmount,
    int coverageAge, bool tobaccoConsumption, double monthlyPremium,
    int paymentTenure, const std::wstring& paymentMode, int createdBy,
    const std::wstring& policyNumber, const std::wstring& startDate,
    const std::wstring& endDate) {

    SQLHANDLE sqlStmtHandle;
    SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);

    std::wstring query = L"INSERT INTO policy_proposals (client_id, insurance_type, life_cover_amount, coverage_age, "
        L"tobacco_consumption, monthly_premium, payment_tenure, payment_mode, created_by, policy_number, "
        L"start_date, end_date, status,paid_value,uw_status) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, 'pending',0,'pending')";

    if (SQLPrepareW(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS) != SQL_SUCCESS) {
        std::wcout << L"Error preparing SQL statement.\n";
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }

    SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &clientId, 0, NULL);
    SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 100, 0, (SQLWCHAR*)insuranceType.c_str(), 0, NULL);
    SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_FLOAT, 0, 0, &lifeCoverAmount, 0, NULL);
    SQLBindParameter(sqlStmtHandle, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &coverageAge, 0, NULL);
    SQLBindParameter(sqlStmtHandle, 5, SQL_PARAM_INPUT, SQL_C_BIT, SQL_BIT, 0, 0, &tobaccoConsumption, 0, NULL);
    SQLBindParameter(sqlStmtHandle, 6, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_FLOAT, 0, 0, &monthlyPremium, 0, NULL);
    SQLBindParameter(sqlStmtHandle, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &paymentTenure, 0, NULL);
    SQLBindParameter(sqlStmtHandle, 8, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLWCHAR*)paymentMode.c_str(), 0, NULL);
    SQLBindParameter(sqlStmtHandle, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &createdBy, 0, NULL);
    SQLBindParameter(sqlStmtHandle, 10, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLWCHAR*)policyNumber.c_str(), 0, NULL);
    SQLBindParameter(sqlStmtHandle, 11, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 20, 0, (SQLWCHAR*)startDate.c_str(), 0, NULL);
    SQLBindParameter(sqlStmtHandle, 12, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 20, 0, (SQLWCHAR*)endDate.c_str(), 0, NULL);

    if (SQLExecute(sqlStmtHandle) == SQL_SUCCESS) {
        std::wcout << L"Policy added successfully.\n";
    }
    else {
        std::wcout << L"Error inserting policy.\n";
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return true;
}

// Function to search for policies by client ID
std::vector<std::wstring> PolicyProposalManagement::searchPolicy(int clientId) {
    std::vector<std::wstring> policies;
    SQLHANDLE sqlStmtHandle;
    SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);

    std::wcout << L"Searching for policies for client ID: " << clientId << std::endl;

    std::wstring query = L"SELECT id, policy_number, insurance_type, life_cover_amount, coverage_age, tobacco_consumption, "
        L"monthly_premium, payment_tenure, start_date, end_date, status "
        L"FROM policy_proposals WHERE client_id = ?";

    if (SQLPrepareW(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS) != SQL_SUCCESS) {
        std::wcout << L"Error preparing SQL statement." << std::endl;
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return policies;
    }

    if (SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &clientId, 0, NULL) != SQL_SUCCESS) {
        std::wcout << L"Error binding SQL parameter." << std::endl;
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return policies;
    }

    if (SQLExecute(sqlStmtHandle) != SQL_SUCCESS) {
        std::wcout << L"Error executing SQL query." << std::endl;
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return policies;
    }

    wchar_t policyNumber[50], insuranceType[100], startDate[20], endDate[20], status[20];
    double lifeCoverAmount, monthlyPremium;
    int id, coverageAge, tobaccoConsumption, paymentTenure;
    SQLLEN indicator;

    // Binding Columns
    SQLBindCol(sqlStmtHandle, 1, SQL_C_LONG, &id, 0, &indicator);
    SQLBindCol(sqlStmtHandle, 2, SQL_C_WCHAR, policyNumber, sizeof(policyNumber), &indicator);
    SQLBindCol(sqlStmtHandle, 3, SQL_C_WCHAR, insuranceType, sizeof(insuranceType), &indicator);
    SQLBindCol(sqlStmtHandle, 4, SQL_C_DOUBLE, &lifeCoverAmount, 0, &indicator);
    SQLBindCol(sqlStmtHandle, 5, SQL_C_LONG, &coverageAge, 0, &indicator);
    SQLBindCol(sqlStmtHandle, 6, SQL_C_LONG, &tobaccoConsumption, 0, &indicator);
    SQLBindCol(sqlStmtHandle, 7, SQL_C_DOUBLE, &monthlyPremium, 0, &indicator);
    SQLBindCol(sqlStmtHandle, 8, SQL_C_LONG, &paymentTenure, 0, &indicator);
    SQLBindCol(sqlStmtHandle, 9, SQL_C_WCHAR, startDate, sizeof(startDate), &indicator);
    SQLBindCol(sqlStmtHandle, 10, SQL_C_WCHAR, endDate, sizeof(endDate), &indicator);
    SQLBindCol(sqlStmtHandle, 11, SQL_C_WCHAR, status, sizeof(status), &indicator);
    SQLBindCol(sqlStmtHandle, 12, SQL_C_LONG, &id, 0, &indicator);

    // Fetching results
    bool hasResults = false;
    while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
        hasResults = true;
        std::wstringstream ss;
        ss << L"Policy Id: " << id << L", Policy Number: " << policyNumber << L", Type: " << insuranceType
            << L", Cover Amount: " << lifeCoverAmount << L", Coverage Age: " << coverageAge
            << L", Tobacco: " << (tobaccoConsumption ? L"Yes" : L"No")
            << L", Monthly Premium: " << monthlyPremium << L", Payment Tenure: " << paymentTenure
            << L", Start Date: " << startDate << L", End Date: " << endDate
            << L", Status: " << status;

        std::wcout << ss.str() << std::endl;
        policies.push_back(ss.str());
    }

    if (!hasResults) {
        std::wcout << L"No policies found for this client." << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return policies;
}


