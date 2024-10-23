#include <FlightSystem/Passenger.h>

Passenger::Passenger() {}

Passenger::Passenger(const String& name, const String& idNumber)
    : name(name), idNumber(idNumber) {}

Passenger::Passenger(const Passenger& other)
    : name(other.name), idNumber(other.idNumber) {}

const String& Passenger::getName() const {
    return name;
}

const String& Passenger::getIdNumber() const {
    return idNumber;
}

std::istream& operator>>(std::istream& is, Passenger& passenger) {
    is >> passenger.name >> passenger.idNumber;
    return is;
}

std::ostream& operator<<(std::ostream& os, const Passenger& passenger) {
    os << passenger.name << " "<< passenger.idNumber;
    return os;
}
