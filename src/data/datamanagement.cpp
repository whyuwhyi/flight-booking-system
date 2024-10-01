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

    inFile >> user_list;
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

    outFile << user_list;
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

// User management functions
bool addUser(const User& user) {
    user_list.add(user); // Assuming add method exists
    return writeUserToFile(user_list);
}

bool modifyUser(const String& username, const User& updatedUser) {
    for (int i = 0; i < user_list.size(); ++i) {
        if (user_list[i].getUsername() == username) { // Assuming getUsername() is defined
            user_list[i] = updatedUser; // Assuming assignment operator is defined
            return writeUserToFile(user_list);
        }
    }
    return false; // Not found
}

bool deleteUser(const String& username) {
    for (int i = 0; i < user_list.size(); ++i) {
        if (user_list[i].getUsername() == username) {
            user_list.removeAt(i); // Assuming removeAt method exists
            return writeUserToFile(user_list);
        }
    }
    return false; // Not found
}


// Airport management functions
bool loadAirportFromFile(AirportList &airport_list) {
    char* filename = myStrcat(DATA_PATH, "airport/airports.txt");
    std::ifstream inFile(filename, std::ios::in);

    if (!inFile.is_open()) {
        std::cout << "File open failed!" << std::endl;
        delete[] filename;
        return false;
    }

    Airport airport;
    while (inFile >> airport) {
        airport_list.add(airport); // Assuming add method exists
    }

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

    for (const auto& airport : airport_list) {
        outFile << airport; // Assuming operator<< is overloaded
    }

    delete[] filename;
    return true;
}

bool addAirport(const Airport& airport) {
    airport_list.add(airport); // Assuming add method exists
    return writeAirportToFile(airport_list);
}

bool modifyAirport(const String& airportName, const Airport& updatedAirport) {
    for (int i = 0; i < airport_list.size(); ++i) {
        if (airport_list[i].getName() == airportName) {
            airport_list[i] = updatedAirport; // Assuming assignment operator is defined
            return writeAirportToFile(airport_list);
        }
    }
    return false; // Not found
}

bool deleteAirport(const String& airportName) {
    for (int i = 0; i < airport_list.size(); ++i) {
        if (airport_list[i].getName() == airportName) {
            airport_list.removeAt(i); // Assuming removeAt method exists
            return writeAirportToFile(airport_list);
        }
    }
    return false; // Not found
}
