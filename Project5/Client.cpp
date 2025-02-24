#include "Client.h"

// Constructor Definition
Client::Client(int id, const std::wstring& name, const std::wstring& dob, const std::wstring& gender,
    double income, const std::wstring& city, const std::wstring& occupation,
    const std::wstring& educationQualification)
    : id(id), name(name), dob(dob), gender(gender), income(income),
    city(city), occupation(occupation), educationQualification(educationQualification) {
}
