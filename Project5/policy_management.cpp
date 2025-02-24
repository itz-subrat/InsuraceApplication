#include "policy_management.h"
#include <iostream>

// Constructor
policymanagement::policymanagement(SQLHANDLE sqlConnHandle) {
    this->sqlConnHandle = sqlConnHandle;
}

// Create a policy proposal
bool policymanagement::create_policy_proposal(int client_id, const std::wstring& insurance_type,
    double life_cover_amount, int coverage_age, bool tobacco_consumption, double monthly_premium,
    const std::wstring& payment_tenure, const std::wstring& payment_mode, int created_by)
{
    SQLHANDLE sqlStmtHandle;
    SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);

    // Insert query using parameterized SQL statement
    std::wstring query = L"INSERT INTO policy_proposals (client_id, insurance_type, life_cover_amount, "
        L"coverage_age, tobacco_consumption, monthly_premium, payment_tenure, payment_mode, "
        L"created_by, status, created_at) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, 'Pending', GETDATE())";

    if (SQLPrepareW(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS) != SQL_SUCCESS) {
        std::cerr << "Error preparing SQL statement for policy proposal!\n";
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }

    SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &client_id, 0, NULL);
    SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 255, 0, (SQLWCHAR*)insurance_type.c_str(), 0, NULL);
    SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DECIMAL, 10, 2, (SQLPOINTER)&life_cover_amount, 0, NULL);
    SQLBindParameter(sqlStmtHandle, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &coverage_age, 0, NULL);
    SQLBindParameter(sqlStmtHandle, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tobacco_consumption, 0, NULL);
    SQLBindParameter(sqlStmtHandle, 6, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DECIMAL, 10, 2, (SQLPOINTER)&monthly_premium, 0, NULL);
    SQLBindParameter(sqlStmtHandle, 7, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLWCHAR*)payment_tenure.c_str(), 0, NULL);
    SQLBindParameter(sqlStmtHandle, 8, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLWCHAR*)payment_mode.c_str(), 0, NULL);
    SQLBindParameter(sqlStmtHandle, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &created_by, 0, NULL);

    if (SQLExecute(sqlStmtHandle) != SQL_SUCCESS) {
        std::cerr << "Error executing policy proposal query!\n";
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    std::wcout << L"Policy proposal created successfully with status 'Pending'.\n";
    return true;
}
