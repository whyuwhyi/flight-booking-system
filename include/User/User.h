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

    bool operator==(const User& other) const;

    friend std::ostream& operator<<(std::ostream &out, const User &user);
    friend std::istream& operator>>(std::istream &in, User &user);
};
