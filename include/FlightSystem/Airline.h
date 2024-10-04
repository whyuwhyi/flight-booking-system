#pragma once
#include <String/String.h>
#include <FlightSystem/Point.h>
#include <LinkedList/LinkedList.h>

class Airline {
private:
    String name;
    String departureAirport;
    String arrivalAirport;
    LinkedList<Point> route;

public:
    Airline();
    Airline(const String& name, const String& departureAirport, const String& arrivalAirport, const LinkedList<Point>& route = LinkedList<Point>());

    const String& getName() const;
    const String& getDepartureAirport() const;
    const String& getArrivalAirport() const;
    const LinkedList<Point>& getRoute() const;

    void setName(const String& name);
    void setDepartureAirport(const String& departureAirport);
    void setArrivalAirport(const String& arrivalAirport);
    void setRoute(const LinkedList<Point>& route);

    void addPoint(const Point& point);

    friend std::ostream& operator<<(std::ostream& out, const Airline& airline);
    friend std::istream& operator>>(std::istream& in, Airline& airline);
};

