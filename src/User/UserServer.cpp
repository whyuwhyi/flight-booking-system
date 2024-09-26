#include <User/UserServer.h>
#include <iostream>

User current_login_user;
String user_file("/home/yuyi/cs-learnning/cpp-projects/curriculum-design/flight-booking-system/data/users.txt");
String local_user_file("/home/yuyi/.flight-booking/local-user.txt");
UserList user_list;


bool loadUserFromFile(UserList& userList, String& fileName) {
    std::ifstream inFile(fileName.c_str(), std::ios::in);
    
    if (!inFile.is_open()) {
        return false;
    }

    inFile >> userList;
    return true;
}

bool writeUserToFile(UserList& userList, String& fileName) {
    std::ofstream outFile(fileName.c_str(), std::ios::out);
    
    if (!outFile.is_open()) {
        return false;
    }

    outFile << userList;
    return true;
}

bool loadLocalUserFromFile(User& user, String& fieName) {
    std::ifstream inFile(fieName.c_str(), std::ios::in);

    if (!inFile.is_open()) {
        return false;
    }
    inFile >> user;
    return true;
}

bool writeLocalUserToFile(User& user,String& fileName) {
    std::ofstream outFile(fileName.c_str(), std::ios::out);
    
    if (!outFile.is_open()) {
        return false;
    }

    outFile << user;
    return true;
}