#pragma once
#ifndef DB_CONNECTION_H
#define DB_CONNECTION_H

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

SQLHANDLE ConnectToSQLServer(bool useWindowsAuth);
void DisconnectFromSQLServer(SQLHANDLE sqlConnHandle);

#endif // DB_CONNECTION_H