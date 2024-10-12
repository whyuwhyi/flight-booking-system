#include <FlightSystem/FlightNetwork.h>
#include <iostream>

FlightNetwork::FlightNetwork(int maxCityCount) {
    cityIndexMap = HashMap<String, int>(maxCityCount * 1.5);
    flightNetwork = new LinkedList<Flight*>*[maxCityCount];
    for (int i = 0; i < maxCityCount; ++i) {
        flightNetwork[i] = new LinkedList<Flight*>[maxCityCount];
    }
}

FlightNetwork::~FlightNetwork() {
    for (int i = 0; i < cityIndexMap.getSize(); ++i) {
        delete[] flightNetwork[i];
    }
    delete[] flightNetwork;
}

void FlightNetwork::addCity(const String& city) {
    if (cityIndexMap.contains(city)) {
        throw std::invalid_argument("City already exists: " );
    }
    int index = cityIndexMap.getSize();
    cityIndexMap.insert(city, index);
}

bool FlightNetwork::cityExists(const String& city) const {
    return cityIndexMap.contains(city);
}

int FlightNetwork::getCityIndex(const String& city) const {
    auto cityIndexOpt = cityIndexMap.get(city);
    if (!cityIndexOpt.has_value()) {
        throw std::invalid_argument("City not found: ");
    }
    return cityIndexOpt.value();
}

void FlightNetwork::addFlight(const String& departureCity, const String& arrivalCity, Flight* flight) {
    if (!cityExists(departureCity)) {
        addCity(departureCity);
    }
    if (!cityExists(arrivalCity)) {
        addCity(arrivalCity);
    }

    int departureIndex = getCityIndex(departureCity);
    int arrivalIndex = getCityIndex(arrivalCity);
    flightNetwork[departureIndex][arrivalIndex].append(flight);
}

Map<Ticket, Ticket> FlightNetwork::findDirectFlights(const String& departureCity, const String& arrivalCity, const Date& date) const {
    Map<Ticket, Ticket> directFlights([](const Ticket& ticket) -> Ticket { return ticket; });

    if (!cityExists(departureCity) || !cityExists(arrivalCity)) {
        return directFlights;
    }

    int departureIndex = getCityIndex(departureCity);
    int arrivalIndex = getCityIndex(arrivalCity);
    flightNetwork[departureIndex][arrivalIndex].traverse([&](Flight* flight) {
        FlightTicketDetail* flightDetail = flight->getFlightSchedule().find(date);
        if (flightDetail) {
            Ticket ticket(flight, flightDetail);
            directFlights.insert(ticket);
        }
    });

    return directFlights;
}

Map<ConnectingTicket, ConnectingTicket> FlightNetwork::findConnectingFlights(const String& departureCity, const String& arrivalCity, const Date& date, int maxStops) const {
    Map<ConnectingTicket, ConnectingTicket> connectingFlights([](const ConnectingTicket& ticket) -> ConnectingTicket { return ticket; });

    if (!cityExists(departureCity) || !cityExists(arrivalCity)) {
        return connectingFlights;
    }

    int departureIndex = getCityIndex(departureCity);
    int arrivalIndex = getCityIndex(arrivalCity);

    struct FlightPath {
        LinkedList<Ticket> tickets;
        int currentCityIndex;
        int stops;
        DateTime totalArrivalTime;
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
                stack.append({tickets, getCityIndex(flight->getArrivalAirport().getCity()), 0, arrivalTime});
            }
        });
    }

    while (stack.size() > 0) {
        FlightPath currentPath = stack.removeLast();
        Ticket lastTicket = currentPath.tickets.getLast()->getElement();
        String currentCity = lastTicket.getFlight()->getArrivalAirport().getCity();
        DateTime lastArrivalTime = currentPath.totalArrivalTime;

        if (currentCity == arrivalCity && currentPath.stops > 0) {
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
            flightNetwork[currentPath.currentCityIndex][i].traverse([&](Flight* nextFlight) {
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
                stack.append({newTickets, getCityIndex(nextFlight->getArrivalAirport().getCity()), currentPath.stops + 1, nextTicket.getArrivalDateTime()});
            });
        }
    }

    return connectingFlights;
}

void FlightNetwork::traverseCities(std::function<void(const String&)> func) const {
    cityIndexMap.traverse([&](const String& city, int index) {
        func(city);
    });
}

void FlightNetwork::traverseFlights(std::function<void(const Flight&)> func) const {
    for (size_t i = 0; i < cityIndexMap.getSize(); ++i) {
        for (size_t j = 0; j < cityIndexMap.getSize(); ++j) {
            flightNetwork[i][j].traverse([&](Flight* flight) {
                func(*flight);
            });
        }
    }
}