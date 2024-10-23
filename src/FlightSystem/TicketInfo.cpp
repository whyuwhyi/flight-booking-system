#include <FlightSystem/TicketInfo.h>

TicketInfo::TicketInfo() {}

TicketInfo::TicketInfo(const String& flightNumber, const String& bookTicketUser, const Passenger& passenger,
                       CabinType cabin, int price, const Date& date, const String& seatNum, TicketStatus status)
    : flightNumber(flightNumber), bookTicketUser(bookTicketUser), passenger(passenger), cabin(cabin), price(price), date(date), seatNum(seatNum), status(status) {}

const String& TicketInfo::getFlightNumber() const {
    return flightNumber;
}

const String& TicketInfo::getBookTicketUser() const {
    return bookTicketUser;
}

const Passenger& TicketInfo::getPassenger() const {
    return passenger;
}

CabinType TicketInfo::getCabinType() const {
    return cabin;
}

int TicketInfo::getPrice() const{
    return price;
}

const Date& TicketInfo::getDate() const {
    return date;
}

const String& TicketInfo::getSeatNum() const {
    return seatNum;
}

TicketStatus TicketInfo::getStatus() const {
    return status;
}

void TicketInfo::setFlightNumber(const String& flightNumber) {
    this->flightNumber = flightNumber;
}

void TicketInfo::setBookTicketUser(const String& bookTicketUser) {
    this->bookTicketUser = bookTicketUser;
}

void TicketInfo::setPassenger(const Passenger& passenger) {
    this->passenger = passenger;
}

void TicketInfo::setCabinType(CabinType cabin) {
    this->cabin = cabin;
}

void TicketInfo::setPrice(int price) {
    this->price = price;
}

void TicketInfo::setDate(const Date& date) {
    this->date = date;
}

void TicketInfo::setSeatNum(const String& seatNum) {
    this->seatNum = seatNum;
}

void TicketInfo::setStatus(TicketStatus status) {
    this->status = status;
}

void TicketInfo::cancelTicket() {
    status = CANCELED;
}

void TicketInfo::checkIn() {
    status = CHECKED_IN;
}

void TicketInfo::refundTicket() {
    status = REFUNDED;
}


std::istream& operator>>(std::istream& is, TicketInfo& ticket) {
    int cabinInt, statusInt;
    is >> ticket.flightNumber >> ticket.bookTicketUser >> ticket.passenger >> cabinInt >> ticket.price >> ticket.date >> ticket.seatNum >> statusInt;
    ticket.cabin = static_cast<CabinType>(cabinInt);
    ticket.status = static_cast<TicketStatus>(statusInt);
    return is;
}

std::ostream& operator<<(std::ostream& os, const TicketInfo& ticket) {
    os << ticket.flightNumber << "\n" 
       << ticket.bookTicketUser << "\n"
       << ticket.passenger << "\n"
       << static_cast<int>(ticket.cabin) << " " << ticket.price << "\n" 
       << ticket.date << "\n" 
       << ticket.seatNum << "\n" 
       << static_cast<int>(ticket.status);
    return os;
}