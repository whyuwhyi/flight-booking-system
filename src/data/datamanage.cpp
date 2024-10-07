#include <data/datamanage.h>
#include <iostream>
#include <fstream>

User current_login_user;
UserMap user_map([] (const User &user) { return user.getPhoneNumber(); });
AirportMap airport_map([] (const Airport &airport) { return airport.getName(); });
AirlineMap airline_map([] (const Airline &airline) { return airline.getName(); });
AirplaneModelMap airplane_model_map([] (const AirplaneModel &model) { return model.getName(); });
FlightMap flight_map([] (const Flight &flight) { return flight.getFlightName(); });

char* myStrcat(const char* str1, const char* str2) {
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    char* result = new char[len1 + len2 + 1];

    strcpy(result, str1);
    strcat(result, str2);

    return result;
}

bool loadUserFromFile(UserMap &user_map) {
    char* filename = myStrcat(DATA_PATH, "user/users.txt");
    std::ifstream inFile(filename, std::ios::in);

    if (!inFile.is_open()) {
        std::cout << "File open failed!" << std::endl;
        delete[] filename;
        return false;
    }

    if (inFile.peek() == std::ifstream::traits_type::eof()) {
        user_map.clear();
        delete[] filename;
        return true;
    }

    inFile >> user_map;
    delete[] filename;
    return true;
}

bool writeUserToFile(const UserMap &user_map) {
    char* filename = myStrcat(DATA_PATH, "user/users.txt");
    std::ofstream outFile(filename, std::ios::out);

    if (!outFile.is_open()) {
        std::cout << "File open failed" << std::endl;
        delete[] filename;
        return false;
    }

    outFile << user_map;
    delete[] filename;
    return true;
}

bool loadLocalUserFromFile(User &local_user) {
    char* filename = myStrcat(LOCAL_DATA_PATH, "local-user.txt");
    std::ifstream inFile(filename, std::ios::in);

    if (!inFile.is_open()) {
        std::cout << "File open failed" << std::endl;
        delete[] filename;
        return false;
    }

    inFile >> local_user;
    delete[] filename;
    return true;
}

bool writeLocalUserToFile(User &local_user) {
    char* filename = myStrcat(LOCAL_DATA_PATH, "local-user.txt");
    std::ofstream outFile(filename, std::ios::out);

    if (!outFile.is_open()) {
        std::cout << "File open failed" << std::endl;
        delete[] filename;
        return false;
    }

    outFile << local_user;
    delete[] filename;
    return true;
}

bool addUser(const User &user) {
    if (user_map.insert(user)) {
        return writeUserToFile(user_map);
    }
    return false;
}

bool modifyUser(const String &username, const User &updatedUser) {
    if (user_map.erase(username)) {
        return addUser(updatedUser);
    }
    return false;
}

bool deleteUser(const String &username) {
    if (user_map.erase(username)) {
        return writeUserToFile(user_map);
    }
    return false;
}

bool loadAirportFromFile(AirportMap &airport_map) {
    char* filename = myStrcat(DATA_PATH, "airport/airports.txt");
    std::ifstream inFile(filename, std::ios::in);

    if (!inFile.is_open()) {
        std::cout << "File open failed!" << std::endl;
        delete[] filename;
        return false;
    }

    if (inFile.peek() == std::ifstream::traits_type::eof()) {
        airport_map.clear();
        delete[] filename;
        return true;
    }

    inFile >> airport_map;
    delete[] filename;
    return true;
}

bool writeAirportToFile(const AirportMap &airport_map) {
    char* filename = myStrcat(DATA_PATH, "airport/airports.txt");
    std::ofstream outFile(filename, std::ios::out);

    if (!outFile.is_open()) {
        std::cout << "File open failed" << std::endl;
        delete[] filename;
        return false;
    }

    outFile << airport_map;
    delete[] filename;
    return true;
}

bool addAirport(const Airport &airport) {
    if (airport_map.insert(airport)) {
        return writeAirportToFile(airport_map);
    }
    return false;
}

bool modifyAirport(const String &airportName, const Airport &updatedAirport) {
    if (airport_map.erase(airportName)) {
        return addAirport(updatedAirport);
    }
    return false;
}

bool deleteAirport(const String &airportName) {
    if (airport_map.erase(airportName)) {
        return writeAirportToFile(airport_map);
    }
    return false;
}

bool loadAirlineFromFile(AirlineMap &airline_map) {
    char* filename = myStrcat(DATA_PATH, "airline/airlines.txt");
    std::ifstream inFile(filename, std::ios::in);

    if (!inFile.is_open()) {
        std::cout << "File open failed!" << std::endl;
        delete[] filename;
        return false;
    }

    if (inFile.peek() == std::ifstream::traits_type::eof()) {
        airline_map.clear();
        delete[] filename;
        return true;
    }

    inFile >> airline_map;
    delete[] filename;
    return true;
}

bool writeAirlineToFile(const AirlineMap &airline_map) {
    char* filename = myStrcat(DATA_PATH, "airline/airlines.txt");
    std::ofstream outFile(filename, std::ios::out);

    if (!outFile.is_open()) {
        std::cout << "File open failed" << std::endl;
        delete[] filename;
        return false;
    }

    outFile << airline_map;
    delete[] filename;
    return true;
}

bool addAirline(const Airline &airline) {
    if (airline_map.insert(airline)) {
        return writeAirlineToFile(airline_map);
    }
    return false;
}

bool modifyAirline(const String &airlineName, const Airline &updatedAirline) {
    if (airline_map.erase(airlineName)) {
        return addAirline(updatedAirline);
    }
    return false;
}

bool deleteAirline(const String &airlineName) {
    if (airline_map.erase(airlineName)) {
        return writeAirlineToFile(airline_map);
    }
    return false;
}

bool loadAirplaneModelFromFile(AirplaneModelMap &airplane_model_map) {
    char* filename = myStrcat(DATA_PATH, "airplanemodel/models.txt");
    std::ifstream inFile(filename, std::ios::in);

    if (!inFile.is_open()) {
        std::cout << "File open failed!" << std::endl;
        delete[] filename;
        return false;
    }

    if (inFile.peek() == std::ifstream::traits_type::eof()) {
        airplane_model_map.clear();
        delete[] filename;
        return true;
    }

    inFile >> airplane_model_map;
    delete[] filename;
    return true;
}

bool writeAirplaneModelToFile(const AirplaneModelMap &airplane_model_map) {
    char* filename = myStrcat(DATA_PATH, "airplanemodel/models.txt");
    std::ofstream outFile(filename, std::ios::out);

    if (!outFile.is_open()) {
        std::cout << "File open failed" << std::endl;
        delete[] filename;
        return false;
    }

    outFile << airplane_model_map;
    delete[] filename;
    return true;
}

bool addAirplaneModel(const AirplaneModel &airplane_model) {
    if (airplane_model_map.insert(airplane_model)) {
        return writeAirplaneModelToFile(airplane_model_map);
    }
    return false;
}

bool modifyAirplaneModel(const String &modelName, const AirplaneModel &updatedAirplaneModel) {
    if (airplane_model_map.erase(modelName)) {
        return addAirplaneModel(updatedAirplaneModel);
    }
    return false;
}

bool deleteAirplaneModel(const String &modelName) {
    if (airplane_model_map.erase(modelName)) {
        return writeAirplaneModelToFile(airplane_model_map);
    }
    return false;
}

bool loadFlightFromFile(FlightMap &flight_map) {
    char* filename = myStrcat(DATA_PATH, "flight/flights.txt");
    std::ifstream inFile(filename, std::ios::in);

    if (!inFile.is_open()) {
        std::cout << "File open failed!" << std::endl;
        delete[] filename;
        return false;
    }

    if (inFile.peek() == std::ifstream::traits_type::eof()) {
        flight_map.clear();
        delete[] filename;
        return true;
    }

    inFile >> flight_map;
    delete[] filename;
    return true;
}

bool writeFlightToFile(const FlightMap &flight_map) {
    char* filename = myStrcat(DATA_PATH, "flight/flights.txt");
    std::ofstream outFile(filename, std::ios::out);

    if (!outFile.is_open()) {
        std::cout << "File open failed" << std::endl;
        delete[] filename;
        return false;
    }

    outFile << flight_map;
    delete[] filename;
    return true;
}

bool addFlight(const Flight &flight) {
    if (flight_map.insert(flight)) {
        return writeFlightToFile(flight_map);
    }
    return false;
}

bool modifyFlight(const String &flightName, const Flight &updatedFlight) {
    if (flight_map.erase(flightName)) {
        return addFlight(updatedFlight);
    }
    return false;
}

bool deleteFlight(const String &flightName) {
    if (flight_map.erase(flightName)) {
        return writeFlightToFile(flight_map);
    }
    return false;
}