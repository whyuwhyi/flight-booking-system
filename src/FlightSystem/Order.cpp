#include <FlightSystem/Order.h>

Order::Order() : seatNum("NULL"){}

Order::Order(const String& flightNumber, const String& bookTicketUser, const Passenger& passenger,
                       CabinType cabin, int price, const Date& date, const String& seatNum, TicketStatus status)
    : flightNumber(flightNumber), bookTicketUser(bookTicketUser), passenger(passenger), cabin(cabin), price(price),
      date(date), seatNum(seatNum), meal(NO_MEAL), status(status) {
        if (seatNum == nullptr)
            this->seatNum = "NULL";
        this->orderNumber =date.toString() + flightNumber + passenger.getIdNumber();
}

Order::Order(const Order& order) {
    orderNumber = order.orderNumber;
    flightNumber = order.flightNumber;
    bookTicketUser = order.bookTicketUser;
    passenger = order.passenger;
    cabin = order.cabin;
    price = order.price;
    date = order.date;
    seatNum = order.seatNum;
    meal = order.meal;
    status = order.status;
}

const String& Order::getOrderNumber() const {
    return orderNumber;
}

const String& Order::getFlightNumber() const {
    return flightNumber;
}

const String& Order::getBookTicketUser() const {
    return bookTicketUser;
}

const Passenger& Order::getPassenger() const {
    return passenger;
}

CabinType Order::getCabinType() const {
    return cabin;
}

int Order::getPrice() const{
    return price;
}

const Date& Order::getDate() const {
    return date;
}

const String& Order::getSeatNum() const {
    return seatNum;
}

TicketStatus Order::getStatus() const {
    return status;
}

enum Meal Order::getMeal() const {
    return meal;
}

void Order::setFlightNumber(const String& flightNumber) {
    this->flightNumber = flightNumber;
}

void Order::setBookTicketUser(const String& bookTicketUser) {
    this->bookTicketUser = bookTicketUser;
}

void Order::setPassenger(const Passenger& passenger) {
    this->passenger = passenger;
}

void Order::setCabinType(CabinType cabin) {
    this->cabin = cabin;
}

void Order::setPrice(int price) {
    this->price = price;
}

void Order::setDate(const Date& date) {
    this->date = date;
}

void Order::setSeatNum(const String& seatNum) {
    this->seatNum = seatNum;
}

void Order::setMeal(Meal meal) {
    this->meal = meal;
}

void Order::setStatus(TicketStatus status) {
    this->status = status;
}

std::istream& operator>>(std::istream& is, Order& order) {
    int cabinInt, statusInt, mealInt;
    is >> order.orderNumber >> order.flightNumber >> order.bookTicketUser >> order.passenger >> 
          cabinInt >> order.price >> order.date >> order.seatNum >> mealInt >> statusInt;
    order.cabin = static_cast<CabinType>(cabinInt);
    order.status = static_cast<TicketStatus>(statusInt);
    order.meal = static_cast<Meal>(mealInt);
    return is;
}

std::ostream& operator<<(std::ostream& os, const Order& order) {
    os << order.orderNumber << "\n"
       << order.flightNumber << "\n" 
       << order.bookTicketUser << "\n"
       << order.passenger << "\n"
       << static_cast<int>(order.cabin) << " " << order.price << "\n" 
       << order.date << "\n" 
       << order.seatNum << "\n" 
       << static_cast<int>(order.meal) << "\n"
       << static_cast<int>(order.status) << "\n";
    return os;
}