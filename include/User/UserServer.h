#pragma once

#include <LinkedList/LinkedList.h>
#include <User/User.h>


User current_login_user;
String user_file("/home/yuyi/cs-learnning/cpp-projects/curriculum-design/flight-booking-system/data/users.txt");

typedef LinkedList<User> UserList;

UserList user_list;

bool loadUserFromFile(UserList& userList, String& fileName);

bool writeUserToFile(UserList& userList, String& fileName);