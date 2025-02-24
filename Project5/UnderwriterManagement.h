#ifndef UNDERWRITER_MANAGEMENT_H
#define UNDERWRITER_MANAGEMENT_H

#include <iostream>
#include "PolicyManagement.h"

class UnderwriterManagement {
private:
    SQLHANDLE sqlConnHandle;
    PolicyManagement policyMgr;

public:
    UnderwriterManagement(SQLHANDLE sqlConnHandle);
    void UnderwriterDashboard();
};

#endif
