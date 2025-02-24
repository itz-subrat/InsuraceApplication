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
    int policyId;
    double monthlyPremium = 0.0, totalPremium = 0.0, receivedAmount = 0.0;
    std::wstring paymentMode;

    std::wcout << L"Enter Policy ID to process payment: ";
    std::wcin >> policyId;

    SQLHANDLE sqlStmtHandle;
    SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);

    // Fetch premium and payment mode
    std::wstring query = L"SELECT monthly_premium, payment_mode FROM policy_proposals WHERE id = ?";
    if (SQLPrepareW(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS) != SQL_SUCCESS) {
        std::wcout << L"Error preparing SQL statement.\n";
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
        return;
    }

    SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &policyId, 0, NULL);
    SQLBindCol(sqlStmtHandle, 1, SQL_C_DOUBLE, &monthlyPremium, 0, NULL);

    wchar_t modeBuffer[50];
    SQLBindCol(sqlStmtHandle, 2, SQL_C_WCHAR, modeBuffer, sizeof(modeBuffer), NULL);

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
        std::wcout << L"Payment successfully recorded.\n";
    }
    else {
        std::wcout << L"Error updating payment record.\n";
        printSQLError(SQL_HANDLE_STMT, sqlStmtHandle);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
}
