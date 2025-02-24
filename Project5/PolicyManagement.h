#ifndef POLICYMANAGEMENT_H
#define POLICYMANAGEMENT_H

#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <string>

class PolicyManagement {
public:
    PolicyManagement(SQLHANDLE sqlConnHandle);
    void viewPendingPolicies();
    std::wstring searchPolicyById(int policyId);
    void approvePolicy(int policyId);
    void cancelPolicy(int policyId);
    void viewPendingPolicyByManager(); // Added method for Manager to search and approve/reject policies
    void approvePolicyByManager(int policyId);

private:
    SQLHANDLE sqlConnHandle;
    void printSQLError(SQLSMALLINT handleType, SQLHANDLE handle);
};

#endif // POLICYMANAGEMENT_H
