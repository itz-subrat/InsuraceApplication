#pragma once
#include <string>

class Client {
private:
    int id;
    std::wstring name;
    std::wstring dob;
    std::wstring gender;
    double income;
    std::wstring city;
    std::wstring occupation;
    std::wstring educationQualification;

public:
    // Constructor Declaration
    Client(int id, const std::wstring& name, const std::wstring& dob, const std::wstring& gender,
        double income, const std::wstring& city, const std::wstring& occupation,
        const std::wstring& educationQualification);

    // Getter Methods
    int getId() const { return id; }
    std::wstring getName() const { return name; }
    std::wstring getDob() const { return dob; }
    std::wstring getGender() const { return gender; }
    double getIncome() const { return income; }
    std::wstring getCity() const { return city; }
    std::wstring getOccupation() const { return occupation; }
    std::wstring getEducationQualification() const { return educationQualification; }
};
