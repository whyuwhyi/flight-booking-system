#pragma once

#include <cstring>
#include <LinkedList/LinkedList.h>
#include <User/User.h>
#include <FlightSystem/Airport.h>

#ifndef DATA_PATH
#define DATA_PATH  "/home/yuyi/cs-learnning/cpp-projects/curriculum-design/flight-booking-system/data/"
#endif

#ifndef LOCAL_DATA_PATH
#define LOCAL_DATA_PATH "/home/yuyi/.flight-booking/"
#endif

typedef LinkedList<User> UserList;
typedef LinkedList<Airport> AirportList;

extern User current_login_user;
extern UserList user_list;
extern AirportList airport_list;

char* myStrcat(const char* str1, const char* str2);

// User management functions
bool loadUserFromFile(UserList &user_list);
bool writeUserToFile(const UserList& user_list);
bool loadLocalUserFromFile(User &local_user);
bool writeLocalUserToFile(User &local_user);
bool addUser(const User& user);
bool modifyUser(const String& username, const User& updatedUser);
bool deleteUser(const String& username);


// Airport management functions
bool loadAirportFromFile(AirportList &airport_list);
bool writeAirportToFile(const AirportList& airport_list);
void displayAirports(const AirportList& airport_list);
bool addAirport(const Airport& airport);
bool modifyAirport(const String& airportName, const Airport& updatedAirport);
bool deleteAirport(const String& airportName);
