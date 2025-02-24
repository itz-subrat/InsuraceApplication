#pragma once
#ifndef MANAGERMANAGEMENT_H
#define MANAGERMANAGEMENT_H

#include <windows.h>
#include "PolicyManagement.h"
#include "CancellationManagement.h"

class ManagerManagement : public CancellationManagement, public PolicyManagement {  // Inheriting both
public:
    ManagerManagement(SQLHANDLE sqlConnHandle);

    void ManagerDashboard();
};

#endif
