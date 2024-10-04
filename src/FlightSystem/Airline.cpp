#include <FlightSystem/Airline.h>

Airline::Airline() : name(), departureAirport(), arrivalAirport(), route() {}

Airline::Airline(const String& name, const String& departureAirport, const String& arrivalAirport, const LinkedList<Point>& route)
    : name(name), departureAirport(departureAirport), arrivalAirport(arrivalAirport), route(route) {}

const String& Airline::getName() const {
    return name;
}

const String& Airline::getDepartureAirport() const {
    return departureAirport;
}

const String& Airline::getArrivalAirport() const {
    return arrivalAirport;
}

const LinkedList<Point>& Airline::getRoute() const {
    return route;
}

void Airline::setName(const String& name) {
    this->name = name;
}

void Airline::setDepartureAirport(const String& departureAirport) {
    this->departureAirport = departureAirport;
}

void Airline::setArrivalAirport(const String& arrivalAirport) {
    this->arrivalAirport = arrivalAirport;
}

void Airline::setRoute(const LinkedList<Point>& route) {
    this->route = route;
}

void Airline::addPoint(const Point& point) {
    route.append(point);
}

std::ostream& operator<<(std::ostream& out, const Airline& airline) {
    out << airline.name << "\n";
    out << airline.departureAirport << "\n";
    out << airline.arrivalAirport << "\n";
    out << airline.route;
    return out;
}

std::istream& operator>>(std::istream& in, Airline& airline) {
    in >> airline.name;
    in >> airline.departureAirport;
    in >> airline.arrivalAirport;
    in >> airline.route;
    return in;
}