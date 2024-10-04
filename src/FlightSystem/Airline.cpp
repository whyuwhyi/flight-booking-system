#include <FlightSystem/Airline.h>

Airline::Airline() : name(), airport1(), airport2(), length(0.0), route() {}

Airline::Airline(const String& name, const String& airport1, const String& airport2, const LinkedList<Point>& route, double length)
    : name(name), airport1(airport1), airport2(airport2), length(length), route(route) {}

const String& Airline::getName() const {
    return name;
}

const String& Airline::getAirport1() const {
    return airport1;
}

const String& Airline::getAirport2() const {
    return airport2;
}

double Airline::getLength() const {
    return length;
}

const LinkedList<Point>& Airline::getRoute() const {
    return route;
}

void Airline::setName(const String& name) {
    this->name = name;
}

void Airline::setAirport1(const String& airport1) {
    this->airport1 = airport1;
}

void Airline::setAirport2(const String& airport2) {
    this->airport2 = airport2;
}

void Airline::setLength(double length) {
    this->length = length;
}

void Airline::setRoute(const LinkedList<Point>& route) {
    this->route = route;
}

void Airline::addPoint(const Point& point) {
    route.append(point);
}

std::ostream& operator<<(std::ostream& out, const Airline& airline) {
    out << airline.name << "\n";
    out << airline.airport1 << "\n";
    out << airline.airport2 << "\n";
    out << airline.length << "\n";
    out << airline.route;
    return out;
}

std::istream& operator>>(std::istream& in, Airline& airline) {
    in >> airline.name;
    in >> airline.airport1;
    in >> airline.airport2;
    in >> airline.length;
    in >> airline.route;
    return in;
}