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

#ifndef DATA_PATH
#define DATA_PATH  "/home/yuyi/cs-learnning/cpp-projects/curriculum-design/flight-booking-system/data/"
#endif

#ifndef LOCAL_DATA_PATH
#define LOCAL_DATA_PATH "/home/yuyi/.flight-booking/"
#endif

typedef Map<String, User> UserMap;
typedef Map<String, Airport> AirportMap;
typedef Map<String, Airline> AirlineMap;
typedef Map<String, AirplaneModel> AirplaneModelMap;
typedef Map<String, Flight> FlightMap;
typedef LinkedList<Flight> FlightList;

extern User current_login_user;
extern UserMap user_map;
extern AirportMap airport_map;
extern AirlineMap airline_map;
extern AirplaneModelMap airplane_model_map;
extern FlightMap flight_map;
extern FlightNetwork flight_network;
extern FlightList flight_list;

char* myStrcat(const char* str1, const char* str2);

// User management functions
bool loadUserFromFile(UserMap &user_map);
bool writeUserToFile(const UserMap &user_map);
bool loadLocalUserFromFile(User &local_user);
bool writeLocalUserToFile(User &local_user);
bool addUser(const User &user);
bool modifyUser(const String &username, const User &updatedUser);
bool deleteUser(const String &username);

// Airport management functions
bool loadAirportFromFile(AirportMap &airport_map);
bool writeAirportToFile(const AirportMap &airport_map);
bool addAirport(const Airport &airport);
bool modifyAirport(const String &airportName, const Airport &updatedAirport);
bool deleteAirport(const String &airportName);

// Airline management functions
bool loadAirlineFromFile(AirlineMap &airline_map);
bool writeAirlineToFile(const AirlineMap &airline_map);
bool addAirline(const Airline &airline);
bool modifyAirline(const String &airlineName, const Airline &updatedAirline);
bool deleteAirline(const String &airlineName);

// AirplaneModel management functions
bool loadAirplaneModelFromFile(AirplaneModelMap &airplane_model_map);
bool writeAirplaneModelToFile(const AirplaneModelMap &airplane_model_map);
bool addAirplaneModel(const AirplaneModel &airplane_model);
bool modifyAirplaneModel(const String &modelName, const AirplaneModel &updatedAirplaneModel);
bool deleteAirplaneModel(const String &modelName);

// Flight management functions
bool loadFlightFromFile(FlightMap &flight_map);
bool writeFlightToFile(const FlightMap &flight_map);
bool addFlight(const Flight &flight);
bool modifyFlight(const String &flightName, const Flight &updatedFlight);
bool deleteFlight(const String &flightName);


bool loadFlightFromFile(FlightList &flight_list);
bool loadFlightNetworkFromFile(FlightNetwork &flight_network);