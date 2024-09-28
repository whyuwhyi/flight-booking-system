#pragma once

#include <String/String.h>
#include <cstdio>

class Passenger {
private:
    String name;     // 乘客姓名
    String idNumber; // 身份证号

public:
    Passenger(const String& name, const String& idNumber);
    Passenger(const Passenger& other);

    const String& getName() const;
    const String& getIdNumber() const;
};

Passenger::Passenger(const String& name, const String& idNumber)
    : name(name), idNumber(idNumber) {}

Passenger::Passenger(const Passenger& other)
    : name(other.name), idNumber(other.idNumber) {}

const String& Passenger::getName() const { return name; }
const String& Passenger::getIdNumber() const { return idNumber; }
