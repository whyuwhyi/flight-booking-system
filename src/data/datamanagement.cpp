#include <data/datamanagement.h>
#include <iostream>
#include <fstream>

User current_login_user;
UserList user_list;

char* myStrcat(const char* str1, const char* str2) {
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    char* result = new char[len1 + len2 + 1];
    
    strcpy(result, str1);
    strcat(result, str2);
    
    return result;
}

bool loadUserFromFile() {
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

bool writeUserToFile() {
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
