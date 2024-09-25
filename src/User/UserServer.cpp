#include <User/UserServer.h>
#include <iostream>


bool loadUserFromFile(UserList& userList, String& fileName) {
    std::ifstream inFile(fileName.c_str(), std::ios::in);
    
    if(!inFile.is_open()){
        return false;
    }

    inFile >> userList;
    return true;
}

bool writeUserToFile(UserList& userList, String& fileName) {
    std::ofstream outFile(fileName.c_str(), std::ios::out);
    
    if(!outFile.is_open()){
        return false;
    }

    outFile << userList;
    return true;
}