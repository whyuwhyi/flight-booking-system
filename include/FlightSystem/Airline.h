#pragma once
#include <String/String.h>
#include <FlightSystem/Point.h>
#include <LinkedList/LinkedList.h>

class Airline {
private:
    String name;
    String airport1;
    String airport2;
    double length;
    LinkedList<Point> route;

public:
    Airline();
    Airline(const String& name, const String& airport1, const String& airport2, const LinkedList<Point>& route = LinkedList<Point>(), double length = 0.0);

    const String& getName() const;
    const String& getAirport1() const;
    const String& getAirport2() const;
    double getLength() const;
    const LinkedList<Point>& getRoute() const;

    void setName(const String& name);
    void setAirport1(const String& airport1);
    void setAirport2(const String& airport2);
    void setLength(double length);
    void setRoute(const LinkedList<Point>& route);

    void addPoint(const Point& point);

    friend std::ostream& operator<<(std::ostream& out, const Airline& airline);
    friend std::istream& operator>>(std::istream& in, Airline& airline);
};
