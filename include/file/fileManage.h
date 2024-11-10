#pragma once

#include <cstring>
#include <Map/Map.h>
#include <User/User.h>
#include <LinkedList/LinkedList.h>
#include <FlightSystem/Airport.h>
#include <FlightSystem/Airline.h>
#include <FlightSystem/AirplaneModel.h>
#include <FlightSystem/Flight.h>
#include <FlightSystem/FlightNetwork.h>
#include <FlightSystem/Passenger.h>
#include <FlightSystem/Order.h>

typedef Map<String, User> UserMap;
typedef Map<String, Airport> AirportMap;
typedef Map<String, Airline> AirlineMap;
typedef Map<String, AirplaneModel> AirplaneModelMap;
typedef Map<String, Flight> FlightMap;
typedef Map<String, Passenger> PassengerMap;
typedef Map<String, Order> OrderMap;
typedef LinkedList<Flight> FlightList;


extern const String DATA_DIR;
extern const String LOCAL_DATA_DIR;

extern const String AIRLINES_PATH;
extern const String MODELS_PATH;
extern const String AIRPORTS_PATH;
extern const String USERS_DIR;
extern const String USERS_PATH;
extern const String FLIGHTS_DIR;
extern const String FLIGHTS_PATH;
extern const String LOCAL_USER_PATH;


extern User current_login_user;
extern UserMap user_map;
extern AirportMap airport_map;
extern AirlineMap airline_map;
extern AirplaneModelMap airplane_model_map;
extern FlightMap flight_map;
extern OrderMap order_map;
extern FlightNetwork flight_network;

bool createDirectory(const char* directoryPath);
bool removeDirectory(const char* directoryPath);
bool createFile(const char* fileName);
bool removeFile(const char* fileName);

bool loadLocalUserFromFile(User &local_user, const char* fileName);
bool writeLocalUserToFile(User &local_user, const char* fileName);

bool loadFlightNetworkFromFile();

bool buyTicket(const Ticket& ticket, const CabinType& cabin, const Passenger& passenger);
bool refundTicket(const Order& ticketInfo);

bool buyMeal(const Order& order, enum Meal meal);
bool chooseSeat(const Order& order, const String& seatNum);

template <typename Key, typename Value>
bool loadMapFromFile(Map<Key, Value> &map, const char* fileName);

template <typename Key, typename Value>
bool writeMapToFile(Map<Key, Value> &map, const char* fileName);

template <typename Key, typename Value>
bool addElementToMap(Map<Key, Value> &map, const Value &element, const char *fileName);

template <typename Key, typename Value>
bool modifyElementInMap(Map<Key, Value> &map, const Value &updateElement, const char* fileName);

template <typename Key, typename Value>
bool deleteElementInMap(Map<Key, Value> &map, const Key &elementKey, const char* fileName);

template <typename Key, typename Value>
bool loadMapFromFile(Map<Key, Value>& map, const char* fileName) {
    std::ifstream inFile(fileName, std::ios::in);
    if (!inFile.is_open()) {
        std::cout << "File open failed!" << std::endl;
        return false;
    }
    if (inFile.peek() == std::ifstream::traits_type::eof()) {
        map.clear();
        
        inFile.close();
        return true;
    }
    inFile >> map;
    return true;
}

template <typename Key, typename Value>
bool writeMapToFile(Map<Key, Value>& map, const char* fileName) {
    std::ofstream outFile(fileName, std::ios::out);
    if (!outFile.is_open()) {
        std::cout << "File open failed" << std::endl;
        return false;
    }
    outFile << map;

    outFile.close();
    return true;
}

template <typename Key, typename Value>
bool addElementToMap(Map<Key, Value>& map, const Value& element, const char* fileName) {
    if (map.insert(element)) {
        return writeMapToFile(map, fileName);
    }
    return false;
}

template <typename Key, typename Value>
bool modifyElementInMap(Map<Key, Value>& map, const Value& updateElement, const char* fileName) {
    Key key = map.getKey(updateElement);
    if (map.erase(key)) {
        return addElementToMap(map, updateElement, fileName);
    }
    return false;
}

template <typename Key, typename Value>
bool deleteElementInMap(Map<Key, Value>& map, const Key& elementKey, const char* fileName) {
    if (map.erase(elementKey)) {
        return writeMapToFile(map, fileName);
    }
    return false;
}

