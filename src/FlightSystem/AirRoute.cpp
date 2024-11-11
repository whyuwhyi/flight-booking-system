#include <FlightSystem/AirRoute.h>

AirRoute::AirRoute() : name(), airport1(), airport2(), length(0.0), route() {}

AirRoute::AirRoute(const String& name, const String& airport1, const String& airport2, const LinkedList<Point>& route, double length)
    : name(name), airport1(airport1), airport2(airport2), length(length), route(route) {}

const String& AirRoute::getName() const {
    return name;
}

const String& AirRoute::getAirport1() const {
    return airport1;
}

const String& AirRoute::getAirport2() const {
    return airport2;
}

double AirRoute::getLength() const {
    return length;
}

const LinkedList<Point>& AirRoute::getRoute() const {
    return route;
}

void AirRoute::setName(const String& name) {
    this->name = name;
}

void AirRoute::setAirport1(const String& airport1) {
    this->airport1 = airport1;
}

void AirRoute::setAirport2(const String& airport2) {
    this->airport2 = airport2;
}

void AirRoute::setLength(double length) {
    this->length = length;
}

void AirRoute::setRoute(const LinkedList<Point>& route) {
    this->route = route;
}

void AirRoute::addPoint(const Point& point) {
    route.append(point);
}

std::ostream& operator<<(std::ostream& out, const AirRoute& airline) {
    out << airline.name << "\n";
    out << airline.airport1 << "\n";
    out << airline.airport2 << "\n";
    out << airline.length << "\n";
    out << airline.route;
    return out;
}

std::istream& operator>>(std::istream& in, AirRoute& airline) {
    in >> airline.name;
    in >> airline.airport1;
    in >> airline.airport2;
    in >> airline.length;
    in >> airline.route;
    return in;
}