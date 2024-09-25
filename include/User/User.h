#pragma once

#include <String/String.h>
#include <fstream>

class User {
private:
    String phoneNumber;
    String password;

public:
    User();
    User(const String& phoneNumber, const String& password);
    User(const User& other);

    const String& getPhoneNumber() const;
    const String& getPassword() const;

    friend std::ostream& operator<<(std::ostream &out, const User &user);
    friend std::istream& operator>>(std::istream &in, User &user);
};

User::User(){
    this->phoneNumber = NULL;
    this->password = NULL;
}

User::User(const String& phoneNumber, const String& password) {
    this->phoneNumber = phoneNumber;
    this->password = password;
}

User::User(const User& other) {
    this->phoneNumber = other.getPhoneNumber();
    this->password = other.getPassword();
}

const String& User::getPhoneNumber() const {
    return phoneNumber;
}

const String& User::getPassword() const {
    return password;
}

std::ostream& operator<<(std::ostream &out, const User &user) {
    out << user.phoneNumber << " " << user.password;
    return out;
}

std::istream& operator>>(std::istream &in, User &user) {
    in >> user.phoneNumber >> user.password;
    return in;
}

