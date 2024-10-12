#pragma once

#include <String/String.h>
#include <FlightSystem/Point.h>
#include <LinkedList/LinkedList.h>
#include <FlightSystem/Time.h>
#include <FlightSystem/AirplaneModel.h>
#include <Map/Map.h>
#include <FlightSystem/Airport.h>
#include <iostream>

class FlightTicketDetail;

typedef Map<Date, FlightTicketDetail> FlightScheduleMap;

class FlightTicketDetail {
public:
private:
    double firstClassPrice;
    double businessClassPrice;
    double economyClassPrice;
    int firstClassTickets;
    int businessClassTickets;
    int economyClassTickets;
    Date flightDate;

public:
    FlightTicketDetail();
    FlightTicketDetail(double firstClassPrice, double businessClassPrice, double economyClassPrice,
                   int firstClassTickets, int businessClassTickets, int economyClassTickets, const Date& flightDate);

    double getCabinPrice(CabinType type) const;
    void setCabinPrice(CabinType type, double price);

    int getRemainingTickets(CabinType type) const;
    void setRemainingTickets(CabinType type, int tickets);

    const Date& getFlightDate() const;
    void setFlightDate(const Date& date);

    friend std::ostream& operator<<(std::ostream& out, const FlightTicketDetail& ticketInfo);

    friend std::istream& operator>>(std::istream& in, FlightTicketDetail& ticketInfo);
};

class Flight {
private:
    String flightName;
    String airline;
    String airplaneModel;
    Airport departureAirport;
    Airport arrivalAirport;
    String flightRouteName;
    Time departureTime;
    Time costTime;
    Cabin firstClassCabin;
    Cabin businessClassCabin;
    Cabin economyClassCabin;
    double initialFirstClassPrice;
    double initialBusinessClassPrice;
    double initialEconomyClassPrice;
    FlightScheduleMap flightScheduleMap;

public:
    Flight();
    Flight(const String& flightName, const String& airline, const String& airplaneModel,
           const Airport& departureAirport, const Airport& arrivalAirport, const String& flightRouteName,
           const Time& departureTime = Time(), const Time& costTime = Time(),
           double initialFirstClassPrice = 0.0, double initialBusinessClassPrice = 0.0, double initialEconomyClassPrice = 0.0);

    const String& getFlightName() const;
    void setFlightName(const String& flightName);
    const String& getAirline() const;
    void setAirline(const String& airline);
    const String& getAirplaneModel() const;
    void setAirplaneModel(const String& airplaneModel);
    const Airport& getDepartureAirport() const;
    void setDepartureAirport(const Airport& departureAirport);
    const Airport& getArrivalAirport() const;
    void setArrivalAirport(const Airport& arrivalAirport);
    const String& getFlightRouteName() const;
    void setFlightRouteName(const String& flightRouteName);
    const Time& getDepartureTime() const;
    void setDepartureTime(const Time& departureTime);
    const Time& getCostTime() const;
    void setCostTime(const Time& costTime);
    double getInitialPrice(CabinType type) const;
    void setInitialPrice(CabinType type, double price);
    Cabin& getCabin(CabinType type);
    const Cabin& getCabin(CabinType type) const;
    void setCabin(CabinType type, const Cabin& cabin);
    bool hasFlightOnDate(const Date& date) const;
    bool addFlightSchedule(const FlightTicketDetail& ticketInfo);
    bool removeFlightSchedule(const Date& date);
    const FlightScheduleMap& getFlightSchedule() const;

    friend std::ostream& operator<<(std::ostream& out, const Flight& flight);
    friend std::istream& operator>>(std::istream& in, Flight& flight);
};