#pragma once
#ifndef POLICY_MANAGEMENT_H
#define POLICY_MANAGEMENT_H

#include "db_connection.h"
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <string>

class PolicyManagement {
public:
    PolicyManagement(dbconnection& dbConn);
    ~PolicyManagement();

    bool create_policy_proposal(int client_id, const std::wstring& insurance_type,
        double life_cover_amount, int coverage_age, bool tobacco_consumption,
        double monthly_premium, const std::wstring& payment_tenure,
        const std::wstring& payment_mode, int created_by);

private:
    SQLHANDLE sqlConnHandle;
};

#endif // POLICY_MANAGEMENT_H
