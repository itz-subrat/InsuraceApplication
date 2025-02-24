#pragma once
#include <windows.h>
#include <sql.h>
#include <sqlext.h>

SQLHANDLE ConnectToSQLServer(bool useWindowsAuth);
void DisconnectFromSQLServer(SQLHANDLE sqlConnHandle);
