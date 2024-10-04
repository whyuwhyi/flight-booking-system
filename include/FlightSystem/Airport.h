#pragma once
#include <String/String.h>
#include <FlightSystem/Point.h>
#include <iostream>

class Airport {
private:
    String name;
    String country;
    String city;
    Point position;


public:
    Airport();
    Airport( const String& name, const String& country, const String& city, const Point& position = Point());
    const String& getCountry() const;
    const String& getCity() const;
    const String& getName() const;
    const Point& getPosition() const;
    void setCountry(const String& country);
    void setCity(const String& city);
    void setName(const String& name);
    void setPosition(const Point& position);

    friend std::ostream& operator<<(std::ostream& out, const Airport& airport);
    friend std::istream& operator>>(std::istream& in, Airport& airport);
};
