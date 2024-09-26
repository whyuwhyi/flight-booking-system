#pragma once

#include <LinkedList/LinkedList.h>
#include <User/User.h>


typedef LinkedList<User> UserList;

extern User current_login_user;
extern UserList user_list;
extern String user_file;
extern String local_user_file;

bool loadUserFromFile(UserList& userList, String& fileName);
bool writeUserToFile(UserList& userList, String& fileName);

bool loadLocalUserFromFile(User& user, String& fieName);
bool writeLocalUserToFile(User& user,String& fileName);