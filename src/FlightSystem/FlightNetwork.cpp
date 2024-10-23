#include <FlightSystem/FlightNetwork.h>
#include <iostream>

FlightNetwork::FlightNetwork(int maxCityCount) {
    cityIndexMap = HashMap<String, CityInfo>(maxCityCount * 1.5);
    internationalFlight = new LinkedList<Flight*>*[maxCityCount];
    for (int i = 0; i < maxCityCount; ++i) {
        internationalFlight[i] = new LinkedList<Flight*>[maxCityCount];
    }
}

FlightNetwork::~FlightNetwork() {
    for (int i = 0; i < cityIndexMap.getSize(); ++i) {
        delete[] internationalFlight[i];
    }
    delete[] internationalFlight;
}

void FlightNetwork::addCity(const Airport& airport) {
    cityIndexMap.insert(airport.getCity(), {!(airport.getCity() == "中国") ,(int)cityIndexMap.getSize()});
}

bool FlightNetwork::cityExists(const String& city) const {
    return cityIndexMap.contains(city);
}

int FlightNetwork::getCityIndex(const String& city) const {
    auto cityIndexOpt = cityIndexMap.get(city);
    if (!cityIndexOpt.has_value()) {
        throw std::invalid_argument("City not found: ");
    }
    return cityIndexOpt.value().index;
}

CityInfo FlightNetwork::getCityInfo(const String& city) const {
    auto cityIndexOpt = cityIndexMap.get(city);
    if (!cityIndexOpt.has_value()) {
        throw std::invalid_argument("City not found: ");
    }
    return cityIndexOpt.value();
}

void FlightNetwork::addFlight(Flight* flight) {
    if (!cityExists(flight->getDepartureAirport().getCity())) {
        addCity(flight->getDepartureAirport());
    }
    if (!cityExists(flight->getArrivalAirport().getCity())) {
        addCity(flight->getArrivalAirport());
    }

    CityInfo departureCityInfo = getCityInfo(flight->getDepartureAirport().getCity());
    CityInfo arrivalCityInfo = getCityInfo(flight->getArrivalAirport().getCity());

    internationalFlight[departureCityInfo.index][arrivalCityInfo.index].append(flight);

    if (!departureCityInfo.isAbroad && !arrivalCityInfo.isAbroad) {
        domesticFlight[departureCityInfo.index][arrivalCityInfo.index].append(flight);
    }

}

Map<Ticket, Ticket> FlightNetwork::findDirectFlights(const String& departureCity, const String& arrivalCity, const Date& date) const {
    Map<Ticket, Ticket> directFlights([](const Ticket& ticket) -> Ticket { return ticket; });

    if (!cityExists(departureCity) || !cityExists(arrivalCity)) {
        return directFlights;
    }

    int departureIndex = getCityIndex(departureCity);
    int arrivalIndex = getCityIndex(arrivalCity);
    internationalFlight[departureIndex][arrivalIndex].traverse([&](Flight* flight) {
        FlightTicketDetail* flightDetail = flight->getFlightSchedule().find(date);
        if (flightDetail) {
            Ticket ticket(flight, flightDetail);
            directFlights.insert(ticket);
        }
    });

    return directFlights;
}



Map<ConnectingTicket, ConnectingTicket> FlightNetwork::findConnectingFlights(LinkedList<Flight*>** flightNetwork, const String& departureCity, const String& arrivalCity, const Date& date, int maxStops) const{
   Map<ConnectingTicket, ConnectingTicket> connectingFlights([](const ConnectingTicket& ticket) -> ConnectingTicket { return ticket; });
    int departureIndex = getCityIndex(departureCity);
    int arrivalIndex = getCityIndex(arrivalCity);

    struct FlightPath {
        LinkedList<Ticket> tickets;
        int currentCityIndex;
        int stops;
        DateTime totalArrivalTime;
        Map<int, int> visitedCities;
    };

    LinkedList<FlightPath> stack;

    for (int i = 0; i < cityIndexMap.getSize(); ++i) {
        flightNetwork[departureIndex][i].traverse([&](Flight* flight) {
            FlightTicketDetail* flightDetail = flight->getFlightSchedule().find(date);
            if (flightDetail) {
                Ticket ticket(flight, flightDetail);
                LinkedList<Ticket> tickets;
                tickets.append(ticket);
                DateTime arrivalTime = ticket.getArrivalDateTime();

                Map<int, int> visitedCities([](const int& value) { return value; });
                visitedCities.insert(departureIndex);
                int arrivalCityIndex = getCityIndex(flight->getArrivalAirport().getCity());
                visitedCities.insert(arrivalCityIndex);

                stack.append({tickets, arrivalCityIndex, 0, arrivalTime, visitedCities});
            }
        });
    }

    while (stack.size() > 0) {
        FlightPath currentPath = stack.removeLast();
        Ticket lastTicket = currentPath.tickets.getLast()->getElement();
        int currentCityIndex = currentPath.currentCityIndex;
        DateTime lastArrivalTime = currentPath.totalArrivalTime;

        if (currentCityIndex == arrivalIndex && currentPath.stops > 0) {
            ConnectingTicket connectingTicket;
            currentPath.tickets.traverse([&](const Ticket& ticket) {
                connectingTicket.addTicket(ticket);
            });
            connectingFlights.insert(connectingTicket);
            continue;
        }

        if (currentPath.stops >= maxStops) {
            continue;
        }

        for (int i = 0; i < cityIndexMap.getSize(); ++i) {
            flightNetwork[currentCityIndex][i].traverse([&](Flight* nextFlight) {
                int nextCityIndex = getCityIndex(nextFlight->getArrivalAirport().getCity());

                if (currentPath.visitedCities.find(nextCityIndex)) {
                    return;
                }

                DateTime nextDepartureTime = lastArrivalTime + Time(24, 0, 0);
                Date nextFlightDate = nextDepartureTime.getDate();
                FlightTicketDetail* nextFlightDetail = nextFlight->getFlightSchedule().find(nextFlightDate);
                if (!nextFlightDetail) {
                    return;
                }

                DateTime nextArrivalTime = DateTime(nextFlightDate, nextFlight->getDepartureTime()) + nextFlight->getCostTime();
                if (nextArrivalTime - Time(24, 0, 0) > lastArrivalTime) {
                    return;
                }

                Ticket nextTicket(nextFlight, nextFlightDetail);
                LinkedList<Ticket> newTickets = currentPath.tickets;
                newTickets.append(nextTicket);

                Map<int, int> newVisitedCities = currentPath.visitedCities;
                newVisitedCities.insert(nextCityIndex);

                stack.append({newTickets, nextCityIndex, currentPath.stops + 1, nextTicket.getArrivalDateTime(), newVisitedCities});
            });
        }
    }

    return connectingFlights;
}

Map<ConnectingTicket, ConnectingTicket> FlightNetwork::findConnectingFlights(const String& departureCity, const String& arrivalCity, const Date& date, int maxStops) const{
    Map<ConnectingTicket, ConnectingTicket> connectingFlights([](const ConnectingTicket& ticket) -> ConnectingTicket { return ticket; });
    if (!cityExists(departureCity) || !cityExists(arrivalCity)) {
        return connectingFlights;
    }

    CityInfo departureCityInfo = getCityInfo(departureCity);
    CityInfo arrivalCityInfo = getCityInfo(arrivalCity);

    if (!departureCityInfo.isAbroad && !arrivalCityInfo.isAbroad) {
        return std::move(findConnectingFlights(domesticFlight, departureCity, arrivalCity, date, maxStops));
    }
    return std::move(findConnectingFlights(internationalFlight, departureCity, arrivalCity, date, maxStops));
}


void FlightNetwork::traverseCities(std::function<void(const String&)> func) const {
    cityIndexMap.traverse([&](const String& city, CityInfo cityInfo) {
        func(city);
    });
}
