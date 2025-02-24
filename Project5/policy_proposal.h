#pragma once
#ifndef POLICY_PROPOSAL_H
#define POLICY_PROPOSAL_H

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <string>

class policymanagement {
private:
    SQLHANDLE sqlConnHandle;

public:
    // Constructor
    policymanagement(SQLHANDLE sqlConnHandle);

    // Create a new policy proposal
    bool create_policy_proposal(int client_id, const std::wstring& insurance_type,
        double life_cover_amount, int coverage_age, bool tobacco_consumption, double monthly_premium,
        const std::wstring& payment_tenure, const std::wstring& payment_mode, int created_by);
};

#endif // POLICY_PROPOSAL_H
