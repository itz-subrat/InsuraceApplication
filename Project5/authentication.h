#pragma once
#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <string>

bool AuthenticateUser(SQLHANDLE sqlConnHandle, const std::wstring& username, const std::wstring& password, std::wstring& role);

#endif // AUTHENTICATION_H