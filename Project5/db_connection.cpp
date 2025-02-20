#include "db_connection.h"
#include <iostream>
#include <cwchar>

SQLHANDLE ConnectToSQLServer(bool useWindowsAuth) {
    SQLHANDLE sqlEnvHandle, sqlConnHandle;
    SQLRETURN retCode;
    SQLWCHAR connectionString[512];

    // Connection string
    if (useWindowsAuth) {
        swprintf(connectionString, sizeof(connectionString) / sizeof(SQLWCHAR),
            L"DRIVER={ODBC Driver 17 for SQL Server};SERVER=localhost;"
            L"DATABASE=master;Trusted_Connection=Yes;");
    }
    else {
        swprintf(connectionString, sizeof(connectionString) / sizeof(SQLWCHAR),
            L"DRIVER={ODBC Driver 17 for SQL Server};SERVER=INLT3178\\SQLEXPRESS;"
            L"DATABASE=SQL_TRAINING;UID=sa;PWD=YourStrongPassword;");
    }

    // Allocate environment handle
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle)) {
        std::cerr << "Error allocating SQL environment handle!\n";
        return NULL;
    }

    // Set the ODBC version environment attribute
    SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

    // Allocate connection handle
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle)) {
        std::cerr << "Error allocating SQL connection handle!\n";
        return NULL;
    }

    // Connect to SQL Server
    retCode = SQLDriverConnectW(sqlConnHandle, NULL, connectionString, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);

    if (SQL_SUCCESS == retCode || SQL_SUCCESS_WITH_INFO == retCode) {
        std::cout << "Connected to SQL Server successfully!\n";
        return sqlConnHandle;
    }
    else {
        std::cerr << "Failed to connect to SQL Server.\n";
        SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
        return NULL;
    }
}

void DisconnectFromSQLServer(SQLHANDLE sqlConnHandle) {
    SQLDisconnect(sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
    std::cout << "Disconnected from SQL Server.\n";
}