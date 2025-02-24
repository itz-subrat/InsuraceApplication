#include "PaymentManagement.h"

// Constructor
PaymentManagement::PaymentManagement(SQLHANDLE connHandle) : sqlConnHandle(connHandle) {}

// Function to print SQL errors
void PaymentManagement::printSQLError(SQLSMALLINT handleType, SQLHANDLE handle) {
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

// Process policy payment
void PaymentManagement::processPayment() {
    int policyId, clientId;
    double monthlyPremium = 0.0, totalPremium = 0.0, receivedAmount = 0.0;
    std::wstring paymentMode;
    SQLHANDLE sqlStmtHandle;

    std::wcout << L"Enter Policy ID to process payment: ";
    std::wcin >> policyId;

    SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);

    // Fetch premium, payment mode, and client ID
    std::wstring query = L"SELECT monthly_premium, payment_mode, client_id FROM policy_proposals WHERE id = ?";
    if (SQLPrepareW(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS) != SQL_SUCCESS) {
        std::wcout << L"Error preparing SQL statement.\n";
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        return;
    }

    SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &policyId, 0, NULL);
    SQLBindCol(sqlStmtHandle, 1, SQL_C_DOUBLE, &monthlyPremium, 0, NULL);

    wchar_t modeBuffer[50];
    SQLBindCol(sqlStmtHandle, 2, SQL_C_WCHAR, modeBuffer, sizeof(modeBuffer), NULL);
    SQLBindCol(sqlStmtHandle, 3, SQL_C_LONG, &clientId, 0, NULL);

    if (SQLExecute(sqlStmtHandle) != SQL_SUCCESS || SQLFetch(sqlStmtHandle) != SQL_SUCCESS) {
        std::wcout << L"Policy not found or error fetching data.\n";
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    paymentMode = modeBuffer;

    // Calculate total premium based on mode
    if (paymentMode == L"Monthly") {
        totalPremium = monthlyPremium;
    }
    else if (paymentMode == L"Yearly") {
        totalPremium = monthlyPremium * 12;
    }
    else if (paymentMode == L"Half-Yearly") {
        totalPremium = monthlyPremium * 6;
    }
    else {
        std::wcout << L"Invalid payment mode.\n";
        return;
    }

    std::wcout << L"Total Premium to be paid: " << totalPremium << L"\n";

    // Loop until exact payment is received
    do {
        std::wcout << L"Enter amount received from customer: ";
        std::wcin >> receivedAmount;

        if (receivedAmount != totalPremium) {
            std::wcout << L"Received amount is incorrect. Please collect the exact amount.\n";
        }
    } while (receivedAmount != totalPremium);

    // Update paid_value in database
    SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);
    query = L"UPDATE policy_proposals SET paid_value = ? WHERE id = ?";

    if (SQLPrepareW(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS) != SQL_SUCCESS) {
        std::wcout << L"Error preparing SQL update.\n";
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        return;
    }

    SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &receivedAmount, 0, NULL);
    SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &policyId, 0, NULL);

    if (SQLExecute(sqlStmtHandle) == SQL_SUCCESS) {
        std::wcout << L"Payment successfully recorded in policy_proposals.\n";
    }
    else {
        std::wcout << L"Error updating payment record in policy_proposals.\n";
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);

    // Insert payment record into payments table
    SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);
    query = L"INSERT INTO payments (client_id, policy_id, payment_amount, payment_date, payment_mode) VALUES (?, ?, ?, GETDATE(), ?)";

    if (SQLPrepareW(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS) != SQL_SUCCESS) {
        std::wcout << L"Error preparing SQL insert.\n";
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        return;
    }

    SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &clientId, 0, NULL);
    SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &policyId, 0, NULL);
    SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &receivedAmount, 0, NULL);
    SQLBindParameter(sqlStmtHandle, 4, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, paymentMode.size(), 0, (SQLWCHAR*)paymentMode.c_str(), 0, NULL);

    if (SQLExecute(sqlStmtHandle) == SQL_SUCCESS) {
        std::wcout << L"Payment successfully recorded in payments table.\n";
    }
    else {
        std::wcout << L"Error inserting payment record into payments table.\n";
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
}
