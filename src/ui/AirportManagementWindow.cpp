#include <data/datamanagement.h>
#include <iostream>
#include <fstream>

User current_login_user;
UserList user_list;
AirportList airport_list;

char* myStrcat(const char* str1, const char* str2) {
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    char* result = new char[len1 + len2 + 1];
    
    strcpy(result, str1);
    strcat(result, str2);
    
    return result;
}

// User management functions
bool loadUserFromFile(UserList &user_list) {
    char* filename = myStrcat(DATA_PATH, "user/users.txt");
    std::ifstream inFile(filename, std::ios::in);
    
    if (!inFile.is_open()) {
        std::cout << "File open failed!" << std::endl;
        delete[] filename;
        return false;
    }

    inFile >> user_list; // Uses overloaded operator>> for LinkedList
    delete[] filename;
    return true;
}

bool writeUserToFile(const UserList &user_list) {
    char* filename = myStrcat(DATA_PATH, "user/users.txt");
    std::ofstream outFile(filename, std::ios::out);
    
    if (!outFile.is_open()) {
        std::cout << "File open failed" << std::endl;
        delete[] filename;
        return false;
    }

    outFile << user_list; // Uses overloaded operator<< for LinkedList
    delete[] filename;
    return true;
}

// Similar changes for loadLocalUserFromFile and writeLocalUserToFile...

// Airport management functions
bool loadAirportFromFile(AirportList &airport_list) {
    char* filename = myStrcat(DATA_PATH, "airport/airports.txt");
    std::ifstream inFile(filename, std::ios::in);

    if (!inFile.is_open()) {
        std::cout << "File open failed!" << std::endl;
        delete[] filename;
        return false;
    }

    inFile >> airport_list; // Uses overloaded operator>> for LinkedList
    delete[] filename;
    return true;
}

bool writeAirportToFile(const AirportList &airport_list) {
    char* filename = myStrcat(DATA_PATH, "airport/airports.txt");
    std::ofstream outFile(filename, std::ios::out);
    
    if (!outFile.is_open()) {
        std::cout << "File open failed" << std::endl;
        delete[] filename;
        return false;
    }

    outFile << airport_list; // Uses overloaded operator<< for LinkedList
    delete[] filename;
    return true;
}

// Add, modify, and delete functions for Airport
bool addAirport(const Airport& airport) {
    airport_list.append(airport); // Assuming append method exists
    return writeAirportToFile(airport_list);
}

bool modifyAirport(const String& airportName, const Airport& updatedAirport) {
    // Assuming LinkedList has a way to iterate
    for (Link<Airport>* current = airport_list.getHead(); current != nullptr; current = current->next) {
        if (current->data.getName() == airportName) {
            current->data = updatedAirport; // Update the airport
            return writeAirportToFile(airport_list);
        }
    }
    return false; // Not found
}

bool deleteAirport(const String& airportName) {
    // Iterate to find and delete
    for (Link<Airport>* current = airport_list.getHead(), *prev = nullptr; current != nullptr; prev = current, current = current->next) {
        if (current->data.getName() == airportName) {
            // Handle deletion logic (assuming removeAt is a method in LinkedList)
            if (prev) {
                prev->next = current->next; // Bypass current
            } else {
                airport_list.setHead(current->next); // Update head if deleting the first element
            }
            delete current; // Free memory
            return writeAirportToFile(airport_list);
        }
    }
    return false; // Not found
}
