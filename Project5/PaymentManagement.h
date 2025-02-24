#pragma once
#ifndef PAYMENTMANAGEMENT_H
#define PAYMENTMANAGEMENT_H

#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include <iostream>
#include <string>

class PaymentManagement {
private:
    SQLHANDLE sqlConnHandle;

public:
    PaymentManagement(SQLHANDLE connHandle);
    void processPayment();
    void printSQLError(SQLSMALLINT handleType, SQLHANDLE handle);
};

#endif // PAYMENTMANAGEMENT_H
