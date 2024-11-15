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

struct CityInfo {
    bool isAbroad;
    int index;
};

class FlightNetwork {
private:
    HashMap<String, CityInfo> cityIndexMap;
    LinkedList<Flight*>** internationalFlight;
    LinkedList<Flight*>** domesticFlight;

public:
    FlightNetwork(int maxCityCount);
    ~FlightNetwork();

    void addCity(const Airport& airport);
    bool cityExists(const String& city) const;
    int getCityIndex(const String& city) const;
    CityInfo getCityInfo(const String& city) const;

    void addFlight(Flight* flight);

    LinkedList<ConnectingTicket> findConnectingFlights(LinkedList<Flight*>** flightNetwork, const String& departureCity, const String& arrivalCity, const Date& date, int maxStops) const;
    LinkedList<ConnectingTicket> findConnectingFlights(const String& departureCity, const String& arrivalCity, const Date& date, int maxStops) const;

    void traverseCities(std::function<void(const String&, const CityInfo&)> func) const;
};