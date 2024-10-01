#include <FlightSystem/Airport.h>
#include "Airport.h"

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

// 比较运算符重载
bool Airport::operator==(const Airport& other) const {
    return name == other.name && country == other.country && city == other.city && position == other.position;
}

// 流操作符重载
std::ostream& operator<<(std::ostream& out, const Airport& airport) {
    out << airport.getName() << '\n'
        << airport.getCountry() << '\n'
        << airport.getCity() << '\n'
        << airport.getPosition(); // 假设 Point 类也实现了流操作符重载
    return out;
}

std::istream& operator>>(std::istream& in, Airport& airport) {
    String name, country, city;
    Point position; // 假设 Point 类也实现了流操作符重载
    in >> name >> country >> city >> position;

    airport.setName(name);
    airport.setCountry(country);
    airport.setCity(city);
    airport.setPosition(position);
    return in;
}
