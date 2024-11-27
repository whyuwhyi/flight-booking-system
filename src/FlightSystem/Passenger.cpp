#include <FlightSystem/Passenger.h>

Passenger::Passenger() : seatNum("NULL"), meal(NO_MEAL) {}

Passenger::Passenger(const String& name, const String& idNumber, const String& seatNum, Meal meal)
    : name(name), idNumber(idNumber), seatNum(seatNum), meal(meal) {}

Passenger::Passenger(const Passenger& other)
    : name(other.name), idNumber(other.idNumber), seatNum(other.seatNum), meal(other.meal) {}

const String& Passenger::getName() const {
    return name;
}

const String& Passenger::getIdNumber() const {
    return idNumber;
}

const String& Passenger::getSeatNum() const {
    return seatNum;
}

Meal Passenger::getMeal() const {
    return meal;
}

void Passenger::setName(const String& name) {
    this->name = name;
}

void Passenger::setIdNumber(const String& idNumber) {
    this->idNumber = idNumber;
}

void Passenger::setSeatNum(const String& seatNum) {
    this->seatNum = seatNum;
}

void Passenger::setMeal(Meal meal) {
    this->meal = meal;
}

std::istream& operator>>(std::istream& is, Passenger& passenger) {
    int mealInt;
    is >> passenger.name >> passenger.idNumber >> passenger.seatNum >> mealInt;
    passenger.meal = static_cast<Meal>(mealInt);
    return is;
}

std::ostream& operator<<(std::ostream& os, const Passenger& passenger) {
    os << passenger.name << " " << passenger.idNumber << " " << passenger.seatNum << " " << static_cast<int>(passenger.meal);
    return os;
}
