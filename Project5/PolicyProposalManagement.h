#ifndef POLICY_PROPOSAL_MANAGEMENT_H
#define POLICY_PROPOSAL_MANAGEMENT_H

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include <vector>

class PolicyProposalManagement {
public:
    PolicyProposalManagement(SQLHANDLE connHandle);

    bool addPolicy(int clientId, const std::wstring& insuranceType, double lifeCoverAmount,
        int coverageAge, bool tobaccoConsumption, double monthlyPremium,
        int paymentTenure, const std::wstring& paymentMode, int createdBy,
        const std::wstring& policyNumber, const std::wstring& startDate,
        const std::wstring& endDate);

    std::vector<std::wstring> searchPolicy(int clientId);
    void printSQLError(SQLSMALLINT handleType, SQLHANDLE handle);


private:
    SQLHANDLE sqlConnHandle;
};

#endif // POLICY_PROPOSAL_MANAGEMENT_H
