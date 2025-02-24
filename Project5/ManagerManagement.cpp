#include "ManagerManagement.h"
#include <iostream>

// Constructor: Initialize both CancellationManagement and PolicyManagement
ManagerManagement::ManagerManagement(SQLHANDLE connHandle)
    : CancellationManagement(connHandle), PolicyManagement(connHandle) {
}

// Manager Dashboard - Now it has access to CancellationManagement & PolicyManagement methods
void ManagerManagement::ManagerDashboard() {
    int choice;

    do {
        std::wcout << L"\nManager Dashboard:\n";
        std::wcout << L"1. View Pending Cancellations\n";
        std::wcout << L"2. Approve/Reject Cancellation Request\n";
        std::wcout << L"3. View Pending Policies\n";
        std::wcout << L"4. Approve/Reject Policy\n";
        std::wcout << L"5. Exit\n";
        std::wcout << L"Enter your choice: ";
        std::wcin >> choice;

        if (choice == 1) {
            viewPendingCancellations();  // Inherited from CancellationManagement
        }
        else if (choice == 2) {
            int cancellationId, managerId, decision;

            std::wcout << L"Enter Cancellation Request ID: ";
            std::wcin >> cancellationId;

            std::wcout << L"Enter Your User ID (Manager ID): ";
            std::wcin >> managerId;

            std::wcout << L"Approve (1) or Reject (0)? ";
            std::wcin >> decision;

            processCancellation(cancellationId, managerId, decision);  // Inherited from CancellationManagement
        }
        else if (choice == 3) {
            viewPendingPolicyByManager();  // Inherited from PolicyManagement
        }
        else if (choice == 4) {
            int policyId, decision;

            std::wcout << L"Enter Policy ID: ";
            std::wcin >> policyId;

            std::wcout << L"Approve (1) or Reject (0)? ";
            std::wcin >> decision;

            if (decision == 1) {
                approvePolicyByManager(policyId);  // Inherited from PolicyManagement
            }
            else {
               // rejectPolicyByManager(policyId);  // Inherited from PolicyManagement
            }
        }
    } while (choice != 5);
}
