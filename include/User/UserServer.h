#pragma once

#include <LinkedList/LinkedList.h>
#include <User/User.h>


typedef LinkedList<User> UserList;

extern User current_login_user;
extern UserList user_list;
extern String data_path;
extern String local_path;

bool loadUserFromFile(UserList& userList, const char* fileName);
bool writeUserToFile(UserList& userList, const char* fileName);

bool loadLocalUserFromFile(User& user, const char* fieName);
bool writeLocalUserToFile(User& user, const char* fileName);