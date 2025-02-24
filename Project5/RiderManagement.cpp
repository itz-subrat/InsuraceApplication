#include "RiderManagement.h"
#include <iostream>

// Constructor
RiderManagement::RiderManagement(SQLHANDLE connHandle) : sqlConnHandle(connHandle) {}

// Function to print SQL errors
void RiderManagement::printSQLError(SQLSMALLINT handleType, SQLHANDLE handle) {
    SQLWCHAR sqlState[6], message[256];
    SQLINTEGER nativeError;
    SQLSMALLINT messageLength;
    SQLRETURN ret;
    int i = 1;

    while ((ret = SQLGetDiagRecW(handleType, handle, i, sqlState, &nativeError, message,
        sizeof(message) / sizeof(SQLWCHAR), &messageLength)) == SQL_SUCCESS) {
        std::wcout << L"SQL Error [" << sqlState << L"]: " << message << L" (Error Code: " << nativeError << L")" << std::endl;
        i++;
    }
}

// Function to add a rider
bool RiderManagement::addRider(int policyId, const std::wstring& addonType, double coverageAmount,
    double additionalPremium, int managerId, int underwriterId) {

    SQLHANDLE sqlStmtHandle;
    SQLRETURN ret;

    // Allocate Statement Handle
    if (SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle) != SQL_SUCCESS) {
        std::wcout << L"Error allocating SQL statement handle.\n";
        return false;
    }

    // SQL Query
    std::wstring query = L"INSERT INTO policy_addons (policy_id, addon_type, coverage_amount, additional_premium, status, manager_id, underwriter_id) "
        L"VALUES (?, ?, ?, ?, 'pending', ?, ?)";

    // Prepare SQL Statement
    ret = SQLPrepareW(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::wcout << L"Error preparing SQL statement.\n";
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }

    // Bind Parameters
    ret = SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &policyId, 0, NULL);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) goto ERROR_HANDLER;

    ret = SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, addonType.size(), 0,
        (SQLWCHAR*)addonType.c_str(), addonType.size() * sizeof(SQLWCHAR), NULL);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) goto ERROR_HANDLER;

    ret = SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &coverageAmount, 0, NULL);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) goto ERROR_HANDLER;

    ret = SQLBindParameter(sqlStmtHandle, 4, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &additionalPremium, 0, NULL);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) goto ERROR_HANDLER;

    ret = SQLBindParameter(sqlStmtHandle, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &managerId, 0, NULL);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) goto ERROR_HANDLER;

    ret = SQLBindParameter(sqlStmtHandle, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &underwriterId, 0, NULL);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) goto ERROR_HANDLER;

    // Execute SQL Statement
    ret = SQLExecute(sqlStmtHandle);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        std::wcout << L"Rider added successfully.\n";
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return true;
    }

ERROR_HANDLER:
    std::wcout << L"Error executing SQL statement.\n";
    printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return false;
}
