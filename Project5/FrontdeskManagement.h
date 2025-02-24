#pragma once
#ifndef FRONTDESKMANAGEMENT_H
#define FRONTDESKMANAGEMENT_H

#include <iostream>
#include "ClientManagement.h"

class FrontDeskManagement {
private:
    SQLHANDLE sqlConnHandle;
    ClientManagement clientMgr;

public:
    FrontDeskManagement(SQLHANDLE sqlConnHandle);
    void FrontDeskDashboard();
};

#endif // FRONTDESKMANAGEMENT_H
