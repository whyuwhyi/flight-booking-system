#pragma once

#include <cstring>
#include <LinkedList/LinkedList.h>
#include <User/User.h>

#ifndef DATA_PATH
#define DATA_PATH  "/home/yuyi/cs-learnning/cpp-projects/curriculum-design/flight-booking-system/data/"
#endif

#ifndef LOCAL_DATA_PATH
#define LOCAL_DATA_PATH "/home/yuyi/.flight-booking/"
#endif

typedef LinkedList<User> UserList;

extern User current_login_user;
extern UserList user_list;

char* myStrcat(const char* str1, const char* str2);
bool loadUserFromFile();
bool writeUserToFile();
bool loadLocalUserFromFile(User &local_user);
bool writeLocalUserToFile(User &local_user);
