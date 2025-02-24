#pragma once
#include "db_connection.h"
#include "Client.h"
#include <vector>
#include "PolicyProposalManagement.h"

class ClientManagement {
private:
    SQLHANDLE sqlConnHandle;
public:
    ClientManagement(SQLHANDLE connHandle);
    bool addClient(const std::wstring& name, const std::wstring& dob, const std::wstring& gender, double income,
        const std::wstring& city, const std::wstring& occupation, const std::wstring& educationQualification);
    std::vector<Client> searchClient(const std::wstring& query);

    bool addPolicyProposal(int clientId, const std::wstring& insuranceType, double lifeCoverAmount,
        int coverageAge, const std::wstring& tobaccoConsumption, double monthlyPremium,
        int paymentTenure, const std::wstring& paymentMode, int createdBy, const std::wstring& policyNumber,
        const std::wstring& startDate, const std::wstring& endDate);
};
