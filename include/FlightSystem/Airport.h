#pragma once
#include <String/String.h>
#include <FlightSystem/Point.h>

class Airport {
private:
    String name;
    String country;
    String city;
    Point position;


public:
    Airport( const String& name = nullptr, const String& country = nullptr, const String& city = nullptr);
    const String& getCountry() const;
    const String& getCity() const;
    const String& getName() const;
    void setCountry(const String& country);
    void setCity(const String& city);
    void setName(const String& name);
    bool operator==(const Airport& other) const;
};
