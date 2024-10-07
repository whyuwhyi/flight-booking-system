// #pragma once

// #include <FlightSystem/Flight.h>
// #include <String/String.h>
// #include <Map/Map.h>
// #include <Map/HashMap.h>
// #include <functional>

// typedef String City;

// class FlightNetwork {
// private:
//     HashMap<City*, int> cityIndexMap;
    
//     Map<Flight*, Flight*>** flightNetwork;
//     int cityCount;

// public:
//     FlightNetwork(int maxCities);

//     bool addCity(City* city);

//     bool addFlight(City* departure, City* arrival, Flight* flight);

//     Map<Flight*, Flight*>* findFlights(City* departure, City* arrival) const;

//     bool removeFlight(City* departure, City* arrival, Flight* flight);

//     void traverseFlights(City* departure, City* arrival, std::function<void(Flight*)> func) const;

//     ~FlightNetwork();
// };
