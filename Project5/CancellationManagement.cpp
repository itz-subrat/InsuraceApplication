#include "CancellationManagement.h"
#include <iostream>

// Constructor
CancellationManagement::CancellationManagement(SQLHANDLE connHandle) : sqlConnHandle(connHandle) {}

// Function to print SQL errors
void CancellationManagement::printSQLError(SQLSMALLINT handleType, SQLHANDLE handle) {
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

// Function to request a cancellation
bool CancellationManagement::requestCancellation(int policyId, int requestedBy, const std::wstring& cancellationReason, const std::wstring& initiatedBy) {
    SQLHANDLE sqlStmtHandle;
    SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);

    std::wstring query = L"INSERT INTO cancellations (policy_id, requested_by, cancellation_date, cancellation_reason, initiated_by, status) "
        L"VALUES (?, ?, GETDATE(), ?, ?, 'pending')";

    if (SQLPrepareW(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS) != SQL_SUCCESS) {
        std::wcout << L"Error preparing SQL statement.\n";
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }

    SQLRETURN ret;
    ret = SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &policyId, 0, NULL);
    if (ret != SQL_SUCCESS) goto ERROR_HANDLER;

    ret = SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &requestedBy, 0, NULL);
    if (ret != SQL_SUCCESS) goto ERROR_HANDLER;

    ret = SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, cancellationReason.size(), 0, (SQLWCHAR*)cancellationReason.c_str(), cancellationReason.size() * sizeof(SQLWCHAR), NULL);
    if (ret != SQL_SUCCESS) goto ERROR_HANDLER;

    ret = SQLBindParameter(sqlStmtHandle, 4, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, initiatedBy.size(), 0, (SQLWCHAR*)initiatedBy.c_str(), initiatedBy.size() * sizeof(SQLWCHAR), NULL);
    if (ret != SQL_SUCCESS) goto ERROR_HANDLER;

    if (SQLExecute(sqlStmtHandle) == SQL_SUCCESS) {
        std::wcout << L"Cancellation request submitted successfully.\n";
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return true;
    }

ERROR_HANDLER:
    std::wcout << L"Error executing SQL statement.\n";
    printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return false;
}

// Function to update cancellation status (Manager Approval)
bool CancellationManagement::updateCancellationStatus(int cancellationId, int approvedBy, const std::wstring& status) {
    SQLHANDLE sqlStmtHandle;
    SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);

    std::wstring query = L"UPDATE cancellations SET status = ?, approved_by = ?, cancellation_date = GETDATE() WHERE id = ?";

    if (SQLPrepareW(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS) != SQL_SUCCESS) {
        std::wcout << L"Error preparing SQL statement.\n";
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }

    SQLRETURN ret;
    ret = SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, status.size(), 0, (SQLWCHAR*)status.c_str(), status.size() * sizeof(SQLWCHAR), NULL);
    if (ret != SQL_SUCCESS) goto ERROR_HANDLER;

    ret = SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &approvedBy, 0, NULL);
    if (ret != SQL_SUCCESS) goto ERROR_HANDLER;

    ret = SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &cancellationId, 0, NULL);
    if (ret != SQL_SUCCESS) goto ERROR_HANDLER;

    if (SQLExecute(sqlStmtHandle) == SQL_SUCCESS) {
        std::wcout << L"Cancellation status updated successfully.\n";
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return true;
    }

ERROR_HANDLER:
    std::wcout << L"Error executing SQL statement.\n";
    printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return false;
}

void CancellationManagement::viewPendingCancellations() {
    SQLHANDLE sqlStmtHandle;
    SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);

    std::wstring query = L"SELECT id, policy_id, requested_by, cancellation_reason, initiated_by FROM cancellations WHERE status = 'pending'";

    if (SQLExecDirectW(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS) != SQL_SUCCESS) {
        std::wcout << L"Error retrieving pending cancellations.\n";
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return;
    }

    int id, policyId, requestedBy;
    SQLWCHAR reason[256], initiatedBy[50];
    SQLLEN len;

    std::wcout << L"\nPending Cancellation Requests:\n";
    std::wcout << L"-------------------------------------------------\n";
    std::wcout << L"ID | Policy ID | Requested By | Reason | Initiated By\n";
    std::wcout << L"-------------------------------------------------\n";

    while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
        SQLGetData(sqlStmtHandle, 1, SQL_C_LONG, &id, 0, &len);
        SQLGetData(sqlStmtHandle, 2, SQL_C_LONG, &policyId, 0, &len);
        SQLGetData(sqlStmtHandle, 3, SQL_C_LONG, &requestedBy, 0, &len);
        SQLGetData(sqlStmtHandle, 4, SQL_C_WCHAR, reason, sizeof(reason), &len);
        SQLGetData(sqlStmtHandle, 5, SQL_C_WCHAR, initiatedBy, sizeof(initiatedBy), &len);

        std::wcout << id << L" | " << policyId << L" | " << requestedBy << L" | " << reason << L" | " << initiatedBy << L"\n";
    }

    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
}

// Function to approve or reject a cancellation request
bool CancellationManagement::processCancellation(int cancellationId, int underwriterId, bool approve) {
    SQLHANDLE sqlStmtHandle;
    SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);

    std::wstring query = L"UPDATE cancellations SET status = ?, approved_by = ?, cancellation_date = GETDATE() WHERE id = ?";

    if (SQLPrepareW(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS) != SQL_SUCCESS) {
        std::wcout << L"Error preparing SQL statement.\n";
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }

    std::wstring status = approve ? L"approved" : L"rejected";

    SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, status.size(), 0, (SQLWCHAR*)status.c_str(), status.size() * sizeof(SQLWCHAR), NULL);
    SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &underwriterId, 0, NULL);
    SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &cancellationId, 0, NULL);

    if (SQLExecute(sqlStmtHandle) == SQL_SUCCESS) {
        std::wcout << L"Cancellation request " << (approve ? L"approved" : L"rejected") << L" successfully.\n";
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return true;
    }

    std::wcout << L"Error updating cancellation request.\n";
    printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return false;
}