#ifndef RIDER_MANAGEMENT_H
#define RIDER_MANAGEMENT_H

#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include <string>

class RiderManagement {
private:
    SQLHANDLE sqlConnHandle;

    // Function to print SQL errors
    void printSQLError(SQLSMALLINT handleType, SQLHANDLE handle);

public:
    // Constructor
    RiderManagement(SQLHANDLE connHandle);

    // Function to add a rider
    bool addRider(int policyId, const std::wstring& addonType, double coverageAmount,
        double additionalPremium, int managerId, int underwriterId);
};

#endif // RIDER_MANAGEMENT_H
