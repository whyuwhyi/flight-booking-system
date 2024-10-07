// #include <FlightSystem/FlightNetwork.h>
// #include <iostream>

// FlightNetwork::FlightNetwork(int maxCities)
//     : cityCount(0) {
//     cityIndexMap = HashMap<City*, int>(maxCities);

//     flightNetwork = new Map<Flight*, Flight*>*[maxCities];
//     for (int i = 0; i < maxCities; ++i) {
//         flightNetwork[i] = new Map<Flight*, Flight*>([](const Flight* &flight) {return flight;})[maxCities];
//     }
// }

// bool FlightNetwork::addCity(City* city) {
//     if (cityIndexMap.contains(city)) {
//         return false;
//     }
//     cityIndexMap.insert(city, cityCount++);
//     return true;
// }

// bool FlightNetwork::addFlight(City* departure, City* arrival, Flight* flight) {
//     auto depIndex = cityIndexMap.get(departure);
//     auto arrIndex = cityIndexMap.get(arrival);
//     if (!depIndex || !arrIndex) {
//         return false;
//     }

//     flightNetwork[*depIndex][*arrIndex].insert(flight);
//     return true;
// }

// Map<Flight*, Flight*>* FlightNetwork::findFlights(City* departure, City* arrival) const {
//     auto depIndex = cityIndexMap.get(departure);
//     auto arrIndex = cityIndexMap.get(arrival);
//     if (!depIndex || !arrIndex) {
//         return nullptr;
//     }
//     return &flightNetwork[*depIndex][*arrIndex];
// }

// bool FlightNetwork::removeFlight(City* departure, City* arrival, Flight* flight) {
//     auto depIndex = cityIndexMap.get(departure);
//     auto arrIndex = cityIndexMap.get(arrival);
//     if (!depIndex || !arrIndex) {
//         return false;
//     }

//     auto flightMap = &flightNetwork[*depIndex][*arrIndex];
//     if (flightMap->erase(flight)) {
//         return true;
//     }
//     return false;
// }

// void FlightNetwork::traverseFlights(City* departure, City* arrival, std::function<void(Flight*)> func) const {
//     auto depIndex = cityIndexMap.get(departure);
//     auto arrIndex = cityIndexMap.get(arrival);
//     if (depIndex && arrIndex) {
//         flightNetwork[*depIndex][*arrIndex].traverse(func);
//     }
// }

// FlightNetwork::~FlightNetwork() {
//     for (int i = 0; i < cityCount; ++i) {
//         delete[] flightNetwork[i];
//     }
//     delete[] flightNetwork;
// }
