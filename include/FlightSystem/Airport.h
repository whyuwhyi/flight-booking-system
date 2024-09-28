#pragma once
#include <String/String.h>

class Airport {
private:
    String country;
    String city;
    String name;

public:
    Airport(const String& country = NULL, const String& city = NULL, const String& name = NULL);
    String getCountry() const;
    String getCity() const;
    String getName() const;
    void setCountry(const String& country);
    void setCity(const String& city);
    void setName(const String& name);
    bool operator==(const Airport& other) const;
};
