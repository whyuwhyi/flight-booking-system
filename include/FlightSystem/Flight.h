#pragma once

#include <Time/Date.h>
#include <Time/Time.h>
#include <String/String.h>
#include <FlightSystem/Airport.h>
#include <FlightSystem/Point.h>
#include <LinkedList/LinkedList.h>

class Flight {
private:
    String airline;
    String flightNumber;
    String airplaneModel;
    String departureAirport;
    String arrivalAirport;
    Time departureTime;
    Time costTime;
    LinkedList<Point> airRoute;

public:
    Flight();
    Flight(const String& airline, const String& flightNumber,
           const String& airplaneModel, const String& departureAirport,
           const String& arrivalAirport, const Time& departureTime = Time(),
           const Time& costTime = Time(), const LinkedList<Point>& airRoute = LinkedList<Point>());

    const String& getAirline() const;
    const String& getFlightNumber() const;
    const String& getAirplaneModel() const;
    const String& getDepartureAirport() const;
    const String& getArrivalAirport() const;
    const Time& getDepartureTime() const;
    const Time& getCostTime() const;
    const LinkedList<Point>& getAirRoute() const;

    void setAirline(const String& airline);
    void setFlightNumber(const String& flightNumber);
    void setAirplaneModel(const String& airplaneModel);
    void setDepartureAirport(const String& departureAirport);
    void setArrivalAirport(const String& arrivalAirport);
    void setDepartureTime(const Time& departureTime);
    void setCostTime(const Time& costTime);
    void addAirRoutePoint(const Point& point);
};



