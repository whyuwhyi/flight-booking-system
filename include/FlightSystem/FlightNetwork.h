#pragma once

#include <FlightSystem/Flight.h>
#include <String/String.h>
#include <Map/Map.h>
#include <Map/HashMap.h>
#include <LinkedList/LinkedList.h>
#include <functional>
#include <stdexcept>
#include <FlightSystem/Time.h>
#include <FlightSystem/Ticket.h>

class FlightNetwork {
private:
    HashMap<String, int> cityIndexMap;
    LinkedList<Flight*>** flightNetwork;

public:
    FlightNetwork(int maxCityCount);
    ~FlightNetwork();

    void addCity(const String& city);
    bool cityExists(const String& city) const;
    int getCityIndex(const String& city) const;

    void addFlight(const String& departureCity, const String& arrivalCity, Flight* flight);

    Map<Ticket, Ticket> findDirectFlights(const String& departureCity, const String& arrivalCity, const Date& date) const;
    Map<ConnectingTicket, ConnectingTicket> findConnectingFlights(const String& departureCity, const String& arrivalCity, const Date& date, int maxStops) const;

    void traverseCities(std::function<void(const String&)> func) const;
    void traverseFlights(std::function<void(const Flight&)> func) const;
};