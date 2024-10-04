#include <FlightSystem/Airport.h>

Airport::Airport(){}

// 构造函数
Airport::Airport(const String& name, const String& country, const String& city, const Point& position)
    : name(name), country(country), city(city), position(position) {}

// Getter 方法
const String& Airport::getCountry() const {
    return country;
}

const String& Airport::getCity() const {
    return city;
}

const String& Airport::getName() const {
    return name;
}

const Point& Airport::getPosition() const {
    return position;
}

// Setter 方法
void Airport::setCountry(const String& country) {
    this->country = country;
}

void Airport::setCity(const String& city) {
    this->city = city;
}

void Airport::setName(const String& name) {
    this->name = name;
}

void Airport::setPosition(const Point& position) {
    this->position = position;
}

std::ostream& operator<<(std::ostream& out, const Airport& airport) {
    out << airport.name<< std::endl
        << airport.country << std::endl
        << airport.city << std::endl
        << airport.position << std::endl;
    return out;
}

std::istream& operator>>(std::istream& in, Airport& airport) {
    in >> airport.name >> airport.country >> airport.city >> airport.position;
    return in;
}
