#pragma once

#include <String/String.h>
#include <cstdio>

class Passenger {
private:
    String name;     // 乘客姓名
    String idNumber; // 身份证号
    String phoneNumber; // 电话号码

public:
    Passenger(const String& name, const String& idNumber, const String& phoneNumber);
    Passenger(const Passenger& other);

    const String& getName() const;
    const String& getIdNumber() const;
    const String& getPhoneNumber() const;

    void print() const;
};

Passenger::Passenger(const String& name, const String& idNumber, const String& phoneNumber)
    : name(name), idNumber(idNumber), phoneNumber(phoneNumber) {}

Passenger::Passenger(const Passenger& other)
    : name(other.name), idNumber(other.idNumber), phoneNumber(other.phoneNumber) {}

const String& Passenger::getName() const { return name; }
const String& Passenger::getIdNumber() const { return idNumber; }
const String& Passenger::getPhoneNumber() const { return phoneNumber; }

void Passenger::print() const {
    printf("Passenger Name: %s\nID Number: %s\nPhone Number: %s\n", 
        name.c_str(), idNumber.c_str(), phoneNumber.c_str());
}
