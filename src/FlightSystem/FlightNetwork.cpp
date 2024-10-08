#include <FlightSystem/FlightNetwork.h>
#include <iostream>

FlightNetwork::FlightNetwork(int initialCityCount) {
    allocateNetwork(initialCityCount);
}

FlightNetwork::~FlightNetwork() {
    deallocateNetwork();
}

void FlightNetwork::allocateNetwork(int cityCount) {
    flightNetwork = new Map<Flight*, Flight*>*[cityCount];
    for (int i = 0; i < cityCount; ++i) {
        flightNetwork[i] = nullptr;
    }
}

void FlightNetwork::deallocateNetwork() {
    for (size_t i = 0; i < cityIndexMap.getSize(); ++i) {
        delete flightNetwork[i];
    }
    delete[] flightNetwork;
}

void FlightNetwork::addCity(const String& city) {
    if (cityIndexMap.contains(city)) {
        throw std::invalid_argument("City already exists: ");
    }
    int index = cityIndexMap.getSize();
    cityIndexMap.insert(city, index);
    // Expand network size dynamically
    Map<Flight*, Flight*>** newFlightNetwork = new Map<Flight*, Flight*>*[index + 1];
    for (int i = 0; i < index; ++i) {
        newFlightNetwork[i] = flightNetwork[i];
    }
    newFlightNetwork[index] = nullptr;
    delete[] flightNetwork;
    flightNetwork = newFlightNetwork;
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
    int departureIndex = getCityIndex(departureCity);
    int arrivalIndex = getCityIndex(arrivalCity);

    if (flightNetwork[departureIndex] == nullptr) {
        auto getKeyLambda = [](const Flight* flight) -> Flight* { return const_cast<Flight*>(flight); };
        auto compareLambda = [](const Flight* a, const Flight* b) -> bool { return a < b; };
        Map<Flight*, Flight*> flightMap(getKeyLambda, compareLambda);
    }

    flightNetwork[departureIndex]->insert(flight);
}

Map<Flight*, Flight*>* FlightNetwork::getFlightsBetween(const String& departureCity, const String& arrivalCity) {
    int departureIndex = getCityIndex(departureCity);
    if (flightNetwork[departureIndex] == nullptr) {
        throw std::invalid_argument("No flights from the specified city: ");
    }
    return flightNetwork[departureIndex];
}

void FlightNetwork::traverseCities(std::function<void(const String&)> func) const {
    cityIndexMap.traverse([&](const String& city, int index) {
        func(city);
    });
}

void FlightNetwork::traverseFlights(std::function<void(const Flight&)> func) const {
    for (size_t i = 0; i < cityIndexMap.getSize(); ++i) {
        if (flightNetwork[i] != nullptr) {
            flightNetwork[i]->traverse([&](Flight* flight) {
                func(*flight);
            });
        }
    }
}