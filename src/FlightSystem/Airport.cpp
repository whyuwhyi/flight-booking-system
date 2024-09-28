#include <FlightSystem/Airport.h>

Airport::Airport(const String& country, const String& city, const String& name)
    : country(country), city(city), name(name) {}

String Airport::getCountry() const {
    return country;
}

String Airport::getCity() const {
    return city;
}

String Airport::getName() const {
    return name;
}

void Airport::setCountry(const String& country) {
    this->country = country;
}

void Airport::setCity(const String& city) {
    this->city = city;
}

void Airport::setName(const String& name) {
    this->name = name;
}

bool Airport::operator==(const Airport& other) const {
    return country == other.country && city == other.city && name == other.name;
}
