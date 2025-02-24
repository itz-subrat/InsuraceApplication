#include "policy_proposal.h"
#include <iostream>

// Constructor
policymanagement::policymanagement(SQLHANDLE sqlConnHandle) {
    this->sqlConnHandle = sqlConnHandle;
}

// Function to create a policy proposal
bool policymanagement::create_policy_proposal(
    int client_id,
    const std::wstring& insurance_type,
    double life_cover_amount,
    int coverage_age,
    bool tobacco_consumption,
    double monthly_premium,
    const std::wstring& payment_tenure,
    const std::wstring& payment_mode,
    int created_by,
    const std::wstring& status
) {
    SQLHANDLE sqlStmtHandle;
    SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);

    // Parameterized query to prevent SQL injection
    std::wstring query =
        L"INSERT INTO policy_proposal (client_id, insurance_type, life_cover_amount, "
        L"coverage_age, tobacco_consumption, monthly_premium, payment_tenure, payment_mode, "
        L"created_by, status, created_at) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, GETDATE())";

    if (SQLPrepareW(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS) != SQL_SUCCESS) {
        std::cerr << "Error preparing SQL statement for policy proposal!\n";
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }

    // Binding parameters
    SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &client_id, 0, NULL);
    SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 255, 0, (SQLWCHAR*)insurance_type.c_str(), 0, NULL);
    SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DECIMAL, 10, 2, &life_cover_amount, 0, NULL);
    SQLBindParameter(sqlStmtHandle, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &coverage_age, 0, NULL);
    SQLBindParameter(sqlStmtHandle, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tobacco_consumption, 0, NULL);
    SQLBindParameter(sqlStmtHandle, 6, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DECIMAL, 10, 2, &monthly_premium, 0, NULL);
    SQLBindParameter(sqlStmtHandle, 7, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLWCHAR*)payment_tenure.c_str(), 0, NULL);
    SQLBindParameter(sqlStmtHandle, 8, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLWCHAR*)payment_mode.c_str(), 0, NULL);
    SQLBindParameter(sqlStmtHandle, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &created_by, 0, NULL);
    SQLBindParameter(sqlStmtHandle, 10, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLWCHAR*)status.c_str(), 0, NULL);

    // Execute query
    if (SQLExecute(sqlStmtHandle) != SQL_SUCCESS) {
        std::cerr << "Error executing policy proposal query!\n";
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    std::wcout << L"Policy proposal created successfully with status '" << status << L"'.\n";
    return true;
}

// Function to search policy proposals by ID
void policymanagement::search_policy_proposal(int proposal_id) {
    SQLHANDLE sqlStmtHandle;
    SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);

    std::wstring query = L"SELECT * FROM policy_proposal WHERE id = ?";

    if (SQLPrepareW(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS) != SQL_SUCCESS) {
        std::cerr << "Error preparing SQL search query for policy proposal!\n";
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return;
    }

    SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &proposal_id, 0, NULL);

    if (SQLExecute(sqlStmtHandle) != SQL_SUCCESS) {
        std::cerr << "Error executing search query for policy proposal!\n";
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return;
    }

    SQLINTEGER id, client_id, coverage_age, created_by;
    SQLWCHAR insurance_type[255], payment_tenure[50], payment_mode[50], status[50];
    SQLDOUBLE life_cover_amount, monthly_premium;
    SQLSMALLINT tobacco_consumption;

    SQLBindCol(sqlStmtHandle, 1, SQL_C_LONG, &id, 0, NULL);
    SQLBindCol(sqlStmtHandle, 2, SQL_C_LONG, &client_id, 0, NULL);
    SQLBindCol(sqlStmtHandle, 3, SQL_C_WCHAR, insurance_type, sizeof(insurance_type), NULL);
    SQLBindCol(sqlStmtHandle, 4, SQL_C_DOUBLE, &life_cover_amount, 0, NULL);
    SQLBindCol(sqlStmtHandle, 5, SQL_C_LONG, &coverage_age, 0, NULL);
    SQLBindCol(sqlStmtHandle, 6, SQL_C_TINYINT, &tobacco_consumption, 0, NULL);
    SQLBindCol(sqlStmtHandle, 7, SQL_C_DOUBLE, &monthly_premium, 0, NULL);
    SQLBindCol(sqlStmtHandle, 8, SQL_C_WCHAR, payment_tenure, sizeof(payment_tenure), NULL);
    SQLBindCol(sqlStmtHandle, 9, SQL_C_WCHAR, payment_mode, sizeof(payment_mode), NULL);
    SQLBindCol(sqlStmtHandle, 10, SQL_C_LONG, &created_by, 0, NULL);
    SQLBindCol(sqlStmtHandle, 11, SQL_C_WCHAR, status, sizeof(status), NULL);

    while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
        std::wcout << L"\nPolicy Proposal ID: " << id;
        std::wcout << L"\nClient ID: " << client_id;
        std::wcout << L"\nInsurance Type: " << insurance_type;
        std::wcout << L"\nLife Cover Amount: " << life_cover_amount;
        std::wcout << L"\nCoverage Age: " << coverage_age;
        std::wcout << L"\nTobacco Consumption: " << (tobacco_consumption ? L"Yes" : L"No");
        std::wcout << L"\nMonthly Premium: " << monthly_premium;
        std::wcout << L"\nPayment Tenure: " << payment_tenure;
        std::wcout << L"\nPayment Mode: " << payment_mode;
        std::wcout << L"\nCreated By: " << created_by;
        std::wcout << L"\nStatus: " << status << L"\n";
    }

    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
}
