#pragma once
#include <String/String.h>

class Airport {
private:
    String name;
    String country;
    String city;


public:
    Airport( const String& name = NULL, const String& country = NULL, const String& city = NULL);
    const String& getCountry() const;
    const String& getCity() const;
    const String& getName() const;
    void setCountry(const String& country);
    void setCity(const String& city);
    void setName(const String& name);
    bool operator==(const Airport& other) const;
};
