#include <User/UserServer.h>
#include <iostream>

User current_login_user;
String data_path("/home/yuyi/cs-learnning/cpp-projects/curriculum-design/flight-booking-system/data/");
String local_path("/home/yuyi/.flight-booking/");
UserList user_list;


bool loadUserFromFile(UserList& userList, const char* fileName) {
    std::ifstream inFile(fileName, std::ios::in);
    
    if (!inFile.is_open()) {
        std::cout << "File open filed!" << std::endl;
        return false;
    }

    inFile >> userList;
    return true;
}

bool writeUserToFile(UserList& userList, const char* fileName) {
    std::ofstream outFile(fileName, std::ios::out);
    
    if (!outFile.is_open()) {
        std::cout << "File open filed" << std::endl;
        return false;
    }

    outFile << userList;
    return true;
}

bool loadLocalUserFromFile(User& user, const char* fieName) {
    std::ifstream inFile(fieName, std::ios::in);

    if (!inFile.is_open()) {
        return false;
    }
    inFile >> user;
    return true;
}

bool writeLocalUserToFile(User& user, const char* fileName) {
    std::ofstream outFile(fileName, std::ios::out);
    
    if (!outFile.is_open()) {
        return false;
    }

    outFile << user;
    return true;
}