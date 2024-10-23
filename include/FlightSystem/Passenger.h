#pragma once

#include <String/String.h>
#include <iostream>

class Passenger {
private:
    String name;
    String idNumber;

public:
    Passenger();
    Passenger(const String& name, const String& idNumber);
    Passenger(const Passenger& other);

    const String& getName() const;
    const String& getIdNumber() const;
    
    friend std::istream& operator>>(std::istream& is, Passenger& passenger);
    friend std::ostream& operator<<(std::ostream& os, const Passenger& passenger);
};
