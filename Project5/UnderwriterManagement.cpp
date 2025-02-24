#include "UnderwriterManagement.h"

UnderwriterManagement::UnderwriterManagement(SQLHANDLE sqlConnHandle)
    : sqlConnHandle(sqlConnHandle), policyMgr(sqlConnHandle) {
}

void UnderwriterManagement::UnderwriterDashboard() {
    int choice;

    do {
        std::wcout << L"\n--- Underwriter Dashboard ---\n";
        std::wcout << L"1. View Pending Policies\n";
        std::wcout << L"2. Search Policy by ID\n";
        std::wcout << L"3. Exit\n";
        std::wcout << L"Enter your choice: ";

        if (!(std::wcin >> choice)) {
            std::wcin.clear();
            std::wcin.ignore(INT_MAX, '\n');
            std::wcout << L"Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
        case 1:
            policyMgr.viewPendingPolicies();
            break;

        case 2: {
            int policyId;
            std::wcout << L"Enter Policy ID to search: ";

            if (!(std::wcin >> policyId)) {
                std::wcin.clear();
                std::wcin.ignore(INT_MAX, '\n');
                std::wcout << L"Invalid input. Please enter a numeric Policy ID.\n";
                break;
            }

            policyMgr.searchPolicyById(policyId); // Ensure this function exists

            int action;
            std::wcout << L"\n1. Approve Policy\n2. Cancel Policy\n3. Go Back\nEnter choice: ";

            if (!(std::wcin >> action)) {
                std::wcin.clear();
                std::wcin.ignore(INT_MAX, '\n');
                std::wcout << L"Invalid input. Returning to menu.\n";
                break;
            }

            if (action == 1) {
                policyMgr.approvePolicy(policyId);
            }
            else if (action == 2) {
                policyMgr.cancelPolicy(policyId);
            }
            break;
        }

        case 3:
            std::wcout << L"Exiting Underwriter Dashboard...\n";
            break;

        default:
            std::wcout << L"Invalid choice. Try again.\n";
            break;
        }
    } while (choice != 3);
}
