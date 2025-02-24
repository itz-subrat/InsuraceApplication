#include "FrontdeskManagement.h"
#include "Authentication.h"
#include "ClientManagement.h"
#include "RiderManagement.h"
#include "CancellationManagement.h"
#include "PaymentManagement.h"

FrontDeskManagement::FrontDeskManagement(SQLHANDLE sqlConnHandle)
    : sqlConnHandle(sqlConnHandle), clientMgr(sqlConnHandle) {
}


void addClientFlow(SQLHANDLE sqlConnHandle) {
    std::wstring name, dob, gender, city, occupation, educationQualification;
    double income;

    std::wcin.ignore();

    std::wcout << L"Enter Name: ";
    std::getline(std::wcin, name);

    std::wcout << L"Enter Date of Birth (YYYY-MM-DD): ";
    std::getline(std::wcin, dob);

    std::wcout << L"Enter Gender: ";
    std::getline(std::wcin, gender);

    std::wcout << L"Enter Income: ";
    std::wcin >> income;
    std::wcin.ignore();

    std::wcout << L"Enter City: ";
    std::getline(std::wcin, city);

    std::wcout << L"Enter Occupation: ";
    std::getline(std::wcin, occupation);

    std::wcout << L"Enter Education Qualification: ";
    std::getline(std::wcin, educationQualification);

    ClientManagement clientMgr(sqlConnHandle);
    if (clientMgr.addClient(name, dob, gender, income, city, occupation, educationQualification)) {
        std::wcout << L"Client added successfully.\n";
    }
    else {
        std::wcout << L"Failed to add client.\n";
    }
}

void searchClientFlow(SQLHANDLE sqlConnHandle) {
    std::wstring searchQuery;
    std::wcout << L"Enter client name or ID to search: ";
    std::wcin.ignore();
    std::getline(std::wcin, searchQuery);

    ClientManagement clientMgr(sqlConnHandle);
    clientMgr.searchClient(searchQuery);
}


void FrontDeskManagement::FrontDeskDashboard() {
    int choice;

    do {
        std::wcout << L"\n--- FrontDesk Dashboard ---\n";
        std::wcout << L"1. Add Client\n";
        std::wcout << L"2. Search Client\n";
        std::wcout << L"3. Add Rider to Policy\n";
        std::wcout << L"4. View Riders for a Policy\n";
		std::wcout << L"5. Cancel a Policy\n";
        std::wcout << L"6. Payment Processing...\n";
        std::wcout << L"7. Exit\n";
        std::wcout << L"Enter your choice: ";;

        if (!(std::wcin >> choice)) {
            std::wcin.clear();
            std::wcin.ignore(INT_MAX, '\n');
            std::wcout << L"Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
        case 1:
            addClientFlow(sqlConnHandle);
            break;
        case 2:
            searchClientFlow(sqlConnHandle);
            break;

        case 3: {
            RiderManagement riderMgr(sqlConnHandle);
            int policyId, managerId, underwriterId;
            std::wstring addonType;
            double coverageAmount, additionalPremium;

            std::wcout << L"Enter Policy ID: ";
            std::wcin >> policyId;
            std::wcin.ignore();

            std::wcout << L"Enter Addon Type: ";
            std::getline(std::wcin, addonType);

            std::wcout << L"Enter Coverage Amount: ";
            std::wcin >> coverageAmount;

            std::wcout << L"Enter Additional Premium: ";
            std::wcin >> additionalPremium;

            std::wcout << L"Enter Manager ID: ";
            std::wcin >> managerId;

            std::wcout << L"Enter Underwriter ID: ";
            std::wcin >> underwriterId;

            if (riderMgr.addRider(policyId, addonType, coverageAmount, additionalPremium, managerId, underwriterId)) {
                std::wcout << L"Rider added successfully.\n";
            }
            break;
        }
        case 4: {
            RiderManagement riderMgr(sqlConnHandle);
            int policyId;
            std::wcout << L"Enter Policy ID to view riders: ";
            std::wcin >> policyId;
            //riderMgr.viewRiders(policyId);
            break;
        }
        case 5: {
            CancellationManagement cancelMgr(sqlConnHandle);
            int policyId, requestedBy;
            std::wstring cancellationReason, initiatedBy;

            std::wcout << L"Enter Policy ID to request cancellation: ";
            std::wcin >> policyId;

            std::wcout << L"Enter Your User ID: ";
            std::wcin >> requestedBy;

            std::wcout << L"Enter Cancellation Reason: ";
            std::wcin.ignore(); // Clear newline from input buffer
            std::getline(std::wcin, cancellationReason);

            std::wcout << L"Enter Initiator Name (e.g., FrontDesk): ";
            std::getline(std::wcin, initiatedBy);

            if (cancelMgr.requestCancellation(policyId, requestedBy, cancellationReason, initiatedBy)) {
                std::wcout << L"Cancellation request submitted successfully.\n";
            }
            else {
                std::wcout << L"Failed to submit cancellation request.\n";
            }
            return;
        }

        case 6:
        {
            PaymentManagement paymentMgr(sqlConnHandle);
            paymentMgr.processPayment();
            break;
        }

        
        case 7:
            std::wcout << L"Exiting FrontDesk Dashboard...\n";
            return;
        default:
            std::wcout << L"Invalid choice. Try again.\n";
            break;
        }
    } while (choice != 8);
}