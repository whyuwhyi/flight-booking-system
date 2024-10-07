#pragma once

#include <String/String.h>
#include <cstdio>

class Passenger {
private:
    String name;
    String idNumber;

public:
    Passenger(const String& name, const String& idNumber);
    Passenger(const Passenger& other);

    const String& getName() const;
    const String& getIdNumber() const;
};

Passenger::Passenger(const String& name = nullptr, const String& idNumber = nullptr)
    : name(name), idNumber(idNumber) {}

Passenger::Passenger(const Passenger& other)
    : name(other.name), idNumber(other.idNumber) {}

const String& Passenger::getName() const { return name; }
const String& Passenger::getIdNumber() const { return idNumber; }
