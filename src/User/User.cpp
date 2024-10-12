#include <User/User.h>



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

bool User::operator==(const User& other) const {
    return (phoneNumber == other.phoneNumber&&password == other.password);
}

std::ostream& operator<<(std::ostream &out, const User &user) {
    out << user.phoneNumber << " " << user.password << "\n";
    return out;
}

std::istream& operator>>(std::istream &in, User &user) {
    in >> user.phoneNumber >> user.password;
    return in;
}

