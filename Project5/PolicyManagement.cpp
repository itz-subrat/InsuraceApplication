#include "PolicyManagement.h"
#include <iostream>
#include <sstream>


PolicyManagement::PolicyManagement(SQLHANDLE sqlConnHandle) : sqlConnHandle(sqlConnHandle) {}

void PolicyManagement::printSQLError(SQLSMALLINT handleType, SQLHANDLE handle) {
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

void PolicyManagement::viewPendingPolicies() {
    SQLHANDLE sqlStmtHandle;
    SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);

    std::wcout << L"\nFetching pending policies...\n";

    std::wstring query = L"SELECT id, policy_number, insurance_type, life_cover_amount, coverage_age, "
        L"tobacco_consumption, monthly_premium, payment_tenure, start_date, end_date "
        L"FROM policy_proposals WHERE uw_status = 'Pending' and paid_value > 0";

    if (SQLPrepareW(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS) != SQL_SUCCESS) {
        std::wcout << L"Error preparing SQL statement." << std::endl;
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return;
    }

    if (SQLExecute(sqlStmtHandle) != SQL_SUCCESS) {
        std::wcout << L"Error executing SQL query." << std::endl;
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return;
    }

    // Variables to hold query results
    int id, coverageAge, tobaccoConsumption, paymentTenure;
    wchar_t policyNumber[50], insuranceType[100], startDate[20], endDate[20];
    double lifeCoverAmount, monthlyPremium;
    SQLLEN indicator;

    // Binding columns
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

    // Fetching and displaying results
    bool hasResults = false;
    while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
        hasResults = true;
        std::wcout << L"\nPolicy ID: " << id
            << L"\nPolicy Number: " << policyNumber
            << L"\nInsurance Type: " << insuranceType
            << L"\nLife Cover Amount: " << lifeCoverAmount
            << L"\nCoverage Age: " << coverageAge
            << L"\nTobacco Consumption: " << (tobaccoConsumption ? L"Yes" : L"No")
            << L"\nMonthly Premium: " << monthlyPremium
            << L"\nPayment Tenure: " << paymentTenure << " years"
            << L"\nStart Date: " << startDate
            << L"\nEnd Date: " << endDate
            << L"\n--------------------------" << std::endl;
    }

    if (!hasResults) {
        std::wcout << L"No pending policies found." << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
}


std::wstring PolicyManagement::searchPolicyById(int policyId) {
    SQLHANDLE sqlStmtHandle;
    SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);

    std::wcout << L"Searching for policy ID: " << policyId << std::endl;

    std::wstring query = L"SELECT policy_number, insurance_type, life_cover_amount, coverage_age, "
        L"tobacco_consumption, monthly_premium, payment_tenure, start_date, end_date, status "
        L"FROM policy_proposals WHERE id = ? ";

    if (SQLPrepareW(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS) != SQL_SUCCESS) {
        std::wcout << L"Error preparing SQL statement." << std::endl;
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return L"";
    }

    if (SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &policyId, 0, NULL) != SQL_SUCCESS) {
        std::wcout << L"Error binding SQL parameter." << std::endl;
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return L"";
    }

    if (SQLExecute(sqlStmtHandle) != SQL_SUCCESS) {
        std::wcout << L"Error executing SQL query." << std::endl;
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return L"";
    }

    wchar_t policyNumber[50], insuranceType[100], startDate[20], endDate[20], status[20];
    double lifeCoverAmount, monthlyPremium;
    int coverageAge, tobaccoConsumption, paymentTenure;
    SQLLEN indicator;

    // Binding Columns
    SQLBindCol(sqlStmtHandle, 1, SQL_C_WCHAR, policyNumber, sizeof(policyNumber), &indicator);
    SQLBindCol(sqlStmtHandle, 2, SQL_C_WCHAR, insuranceType, sizeof(insuranceType), &indicator);
    SQLBindCol(sqlStmtHandle, 3, SQL_C_DOUBLE, &lifeCoverAmount, 0, &indicator);
    SQLBindCol(sqlStmtHandle, 4, SQL_C_LONG, &coverageAge, 0, &indicator);
    SQLBindCol(sqlStmtHandle, 5, SQL_C_LONG, &tobaccoConsumption, 0, &indicator);
    SQLBindCol(sqlStmtHandle, 6, SQL_C_DOUBLE, &monthlyPremium, 0, &indicator);
    SQLBindCol(sqlStmtHandle, 7, SQL_C_LONG, &paymentTenure, 0, &indicator);
    SQLBindCol(sqlStmtHandle, 8, SQL_C_WCHAR, startDate, sizeof(startDate), &indicator);
    SQLBindCol(sqlStmtHandle, 9, SQL_C_WCHAR, endDate, sizeof(endDate), &indicator);
    SQLBindCol(sqlStmtHandle, 10, SQL_C_WCHAR, status, sizeof(status), &indicator);

    std::wstring result;
    if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
        std::wstringstream ss;
        ss << L"Policy Number: " << policyNumber << L", Type: " << insuranceType
            << L", Cover Amount: " << lifeCoverAmount << L", Coverage Age: " << coverageAge
            << L", Tobacco: " << (tobaccoConsumption ? L"Yes" : L"No")
            << L", Monthly Premium: " << monthlyPremium << L", Payment Tenure: " << paymentTenure
            << L", Start Date: " << startDate << L", End Date: " << endDate
            << L", Status: " << status;

        result = ss.str();
        std::wcout << result << std::endl;
    }
    else {
        std::wcout << L"No policy found with this ID." << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return result;
}


void PolicyManagement::approvePolicy(int policyId) {
    SQLHANDLE sqlStmtHandle;
    SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);

    std::wcout << L"Approving policy ID: " << policyId << std::endl;

    std::wstring query = L"UPDATE policy_proposals SET uw_status = 'Approved' WHERE id = ?";

    if (SQLPrepareW(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS) != SQL_SUCCESS) {
        std::wcout << L"Error preparing SQL statement." << std::endl;
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return;
    }

    if (SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &policyId, 0, NULL) != SQL_SUCCESS) {
        std::wcout << L"Error binding SQL parameter." << std::endl;
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return;
    }

    if (SQLExecute(sqlStmtHandle) != SQL_SUCCESS) {
        std::wcout << L"Error executing SQL update query." << std::endl;
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return;
    }

    std::wcout << L"Policy approved successfully." << std::endl;

    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
}


void PolicyManagement::cancelPolicy(int policyId) {
    SQLHANDLE sqlStmtHandle;
    SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);

    std::wcout << L"Canceling policy ID: " << policyId << std::endl;

    std::wstring query = L"UPDATE policy_proposals SET uw_status = 'Canceled' WHERE id = ?";

    if (SQLPrepareW(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS) != SQL_SUCCESS) {
        std::wcout << L"Error preparing SQL statement." << std::endl;
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return;
    }

    if (SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &policyId, 0, NULL) != SQL_SUCCESS) {
        std::wcout << L"Error binding SQL parameter." << std::endl;
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return;
    }

    if (SQLExecute(sqlStmtHandle) != SQL_SUCCESS) {
        std::wcout << L"Error executing SQL update query." << std::endl;
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return;
    }

    std::wcout << L"Policy canceled successfully." << std::endl;

    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
}

void PolicyManagement::viewPendingPolicyByManager() {
    SQLHANDLE sqlStmtHandle;
    SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);

    std::wcout << L"\nFetching manager-approvable policies...\n";

    std::wstring query = L"SELECT id, policy_number, insurance_type, life_cover_amount, monthly_premium FROM policy_proposals "
        L"WHERE status = 'Pending' AND uw_status = 'Approved'";

    if (SQLPrepareW(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS) != SQL_SUCCESS) {
        std::wcout << L"Error preparing SQL statement." << std::endl;
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return;
    }

    if (SQLExecute(sqlStmtHandle) != SQL_SUCCESS) {
        std::wcout << L"Error executing SQL query." << std::endl;
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return;
    }

    int id;
    wchar_t policyNumber[50], insuranceType[100];
    double lifeCoverAmount, monthlyPremium;
    SQLLEN indicator;

    SQLBindCol(sqlStmtHandle, 1, SQL_C_LONG, &id, 0, &indicator);
    SQLBindCol(sqlStmtHandle, 2, SQL_C_WCHAR, policyNumber, sizeof(policyNumber), &indicator);
    SQLBindCol(sqlStmtHandle, 3, SQL_C_WCHAR, insuranceType, sizeof(insuranceType), &indicator);
    SQLBindCol(sqlStmtHandle, 4, SQL_C_DOUBLE, &lifeCoverAmount, 0, &indicator);
    SQLBindCol(sqlStmtHandle, 5, SQL_C_DOUBLE, &monthlyPremium, 0, &indicator);

    bool hasResults = false;
    while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
        hasResults = true;
        std::wcout << L"\nPolicy ID: " << id
            << L"\nPolicy Number: " << policyNumber
            << L"\nInsurance Type: " << insuranceType
            << L"\nLife Cover Amount: " << lifeCoverAmount
            << L"\nMonthly Premium: " << monthlyPremium
            << L"\n--------------------------" << std::endl;
    }

    if (!hasResults) {
        std::wcout << L"No pending policies found for manager approval." << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
}

void PolicyManagement::approvePolicyByManager(int policyId) {
    SQLHANDLE sqlStmtHandle;
    SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);

    std::wcout << L"Approving policy ID: " << policyId << std::endl;
    std::wstring policyNumber = L"PAKUYTHHG" + std::to_wstring(policyId);

    std::wstring query = L"UPDATE policy_proposals SET status = 'Approved', policy_number = ? WHERE id = ?";

    if (SQLPrepareW(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS) != SQL_SUCCESS) {
        std::wcout << L"Error preparing SQL statement." << std::endl;
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return;
    }

    if (SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &policyId, 0, NULL) != SQL_SUCCESS) {
        std::wcout << L"Error binding SQL parameter." << std::endl;
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return;
    }

    if (SQLExecute(sqlStmtHandle) != SQL_SUCCESS) {
        std::wcout << L"Error executing SQL update query." << std::endl;
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return;
    }

    std::wcout << L"Policy approved successfully by Manager." << std::endl;

    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
}
