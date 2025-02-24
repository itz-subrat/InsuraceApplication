#pragma once
#ifndef CANCELLATION_MANAGEMENT_H
#define CANCELLATION_MANAGEMENT_H

#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include <string>

class CancellationManagement {
private:
    SQLHANDLE sqlConnHandle;

    void printSQLError(SQLSMALLINT handleType, SQLHANDLE handle);

public:
    explicit CancellationManagement(SQLHANDLE connHandle);
    bool requestCancellation(int policyId, int requestedBy, const std::wstring& cancellationReason, const std::wstring& initiatedBy);
    bool updateCancellationStatus(int cancellationId, int approvedBy, const std::wstring& status);
    // View pending cancellation requests
    void viewPendingCancellations();

    // Approve or reject a cancellation request
    bool processCancellation(int cancellationId, int underwriterId, bool approve);

};

#endif
