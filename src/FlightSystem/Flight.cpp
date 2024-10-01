#include <FlightSystem/Flight.h>

Flight::Flight(const String& airline, const String& flightNumber,
               const String& airplaneModel, const String& departureAirport,
               const String& arrivalAirport, const Time& departureTime,
               const Time& costTime, const LinkedList<Point>& airRoute)
    : airline(airline), flightNumber(flightNumber), airplaneModel(airplaneModel),
      departureAirport(departureAirport), arrivalAirport(arrivalAirport),
      departureTime(departureTime), costTime(costTime), airRoute(airRoute) {}

const String& Flight::getAirline() const {
    return airline;
}

const String& Flight::getFlightNumber() const {
    return flightNumber;
}

const String& Flight::getAirplaneModel() const {
    return airplaneModel;
}

const String& Flight::getDepartureAirport() const {
    return departureAirport;
}

const String& Flight::getArrivalAirport() const {
    return arrivalAirport;
}

const Time& Flight::getDepartureTime() const {
    return departureTime;
}

const Time& Flight::getCostTime() const {
    return costTime;
}

const LinkedList<Point>& Flight::getAirRoute() const {
    return airRoute;
}

void Flight::setAirline(const String& airline) {
    this->airline = airline;
}

void Flight::setFlightNumber(const String& flightNumber) {
    this->flightNumber = flightNumber;
}

void Flight::setAirplaneModel(const String& airplaneModel) {
    this->airplaneModel = airplaneModel;
}

void Flight::setDepartureAirport(const String& departureAirport) {
    this->departureAirport = departureAirport;
}

void Flight::setArrivalAirport(const String& arrivalAirport) {
    this->arrivalAirport = arrivalAirport;
}

void Flight::setDepartureTime(const Time& departureTime) {
    this->departureTime = departureTime;
}

void Flight::setCostTime(const Time& costTime) {
    this->costTime = costTime;
}

void Flight::addAirRoutePoint(const Point& point) {
    airRoute.append(point);
}
