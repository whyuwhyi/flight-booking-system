#include <FlightSystem/Airport.h>

Airport::Airport(const String& name, const String& country, const String& city)
    : name(name), country(country), city(city){}

const String& Airport::getName() const {
    return name;
}

const String& Airport::getCountry() const {
    return country;
}

const String& Airport::getCity() const {
    return city;
}

void Airport::setName(const String& name) {
    this->name = name;
}

void Airport::setCountry(const String& country) {
    this->country = country;
}

void Airport::setCity(const String& city) {
    this->city = city;
}

bool Airport::operator==(const Airport& other) const {
    return country == other.country && city == other.city && name == other.name;
}
