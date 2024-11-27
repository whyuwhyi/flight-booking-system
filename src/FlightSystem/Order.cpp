#include <FlightSystem/Order.h>
#include <file/fileManage.h>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <random>

// OrderInfo Implementation
OrderInfo::OrderInfo()
    : flightNumber(""), airRoute(""), airplaneModel(""), airline(""),
      departureTimeSlot(MORNING), cabin(EconomyClass), price(0), date(Date()),
      ticket(Ticket()), passenger(Passenger()) {}

OrderInfo::OrderInfo(const String& flightNumber, const String& airRoute, const String& airplaneModel,
                     const String& airline, TimeSlot departureTimeSlot, CabinType cabin,
                     double price, const Date& date, const Ticket& ticket,
                     const Passenger& passenger)
    : flightNumber(flightNumber), airRoute(airRoute), airplaneModel(airplaneModel),
      airline(airline), departureTimeSlot(departureTimeSlot), cabin(cabin),
      price(price), date(date), ticket(ticket), passenger(passenger) {}

const String& OrderInfo::getFlightNumber() const {
    return flightNumber;
}

const String& OrderInfo::getAirRoute() const {
    return airRoute;
}

const String& OrderInfo::getAirplaneModel() const {
    return airplaneModel;
}

const String& OrderInfo::getAirline() const {
    return airline;
}

TimeSlot OrderInfo::getDepartureTimeSlot() const {
    return departureTimeSlot;
}

CabinType OrderInfo::getCabinType() const {
    return cabin;
}

int OrderInfo::getPrice() const {
    return price;
}

const Date& OrderInfo::getDate() const {
    return date;
}

const Ticket& OrderInfo::getTicket() const {
    return ticket;
}

const Passenger& OrderInfo::getPassenger() const {
    return passenger;
}

const String& OrderInfo::getSeatNum() const {
    return passenger.getSeatNum();
}

Meal OrderInfo::getMeal() const {
    return passenger.getMeal();
}

void OrderInfo::setFlightNumber(const String& flightNumber) {
    this->flightNumber = flightNumber;
}

void OrderInfo::setAirRoute(const String& airRoute) {
    this->airRoute = airRoute;
}

void OrderInfo::setAirplaneModel(const String& airplaneModel) {
    this->airplaneModel = airplaneModel;
}

void OrderInfo::setAirline(const String& airline) {
    this->airline = airline;
}

void OrderInfo::setDepartureTimeSlot(TimeSlot departureTimeSlot) {
    this->departureTimeSlot = departureTimeSlot;
}

void OrderInfo::setCabinType(CabinType cabin) {
    this->cabin = cabin;
}

void OrderInfo::setPrice(int price) {
    this->price = price;
}

void OrderInfo::setDate(const Date& date) {
    this->date = date;
}

void OrderInfo::setTicket(const Ticket& ticket) {
    this->ticket = ticket;
}

void OrderInfo::setPassenger(const Passenger& passenger) {
    this->passenger = passenger;
}

void OrderInfo::setSeatNum(const String& seatNum) {
    this->passenger.setSeatNum(seatNum);
}

void OrderInfo::setMeal(Meal meal) {
    this->passenger.setMeal(meal);
}

std::istream& operator>>(std::istream& is, OrderInfo& orderInfo) {
    int cabinInt, timeSlotInt;
    is >> orderInfo.flightNumber >> orderInfo.airRoute >> orderInfo.airplaneModel >> orderInfo.airline
       >> timeSlotInt >> cabinInt >> orderInfo.price >> orderInfo.date >> orderInfo.passenger;
    orderInfo.cabin = static_cast<CabinType>(cabinInt);
    orderInfo.departureTimeSlot = static_cast<TimeSlot>(timeSlotInt);

    Flight* flight = flight_map.find(orderInfo.flightNumber);
    FlightTicketDetail* ticketDetail = flight->getFlightTicketDetail(orderInfo.date);

    orderInfo.ticket = Ticket(flight, ticketDetail);

    return is;
}

std::ostream& operator<<(std::ostream& os, const OrderInfo& orderInfo) {
    os << orderInfo.flightNumber << "\n" << orderInfo.airRoute << "\n"
       << orderInfo.airplaneModel << "\n" << orderInfo.airline << "\n"
       << static_cast<int>(orderInfo.departureTimeSlot) << "\n" << static_cast<int>(orderInfo.cabin) << "\n"
       << orderInfo.price << "\n" << orderInfo.date << "\n" << orderInfo.passenger << "\n";
    return os;
}

// Order Implementation
Order::Order() : orderNumber(""), bookTicketUser(""), status(BOOKED) {
    orderNumber = generateOrderNumber();
}

Order::Order(const String& bookTicketUser, TicketStatus status)
    : bookTicketUser(bookTicketUser), status(status) {
    orderNumber = generateOrderNumber();
}

Order::Order(const Order& order)
    : orderNumber(order.orderNumber), bookTicketUser(order.bookTicketUser),
      status(order.status), orderInfos(order.orderInfos) {}

String Order::generateOrderNumber() {
    auto now = std::chrono::system_clock::now();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    std::ostringstream oss;
    oss << millis;
    String timestampStr = oss.str().c_str();

    if (timestampStr.size() < 14) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 9);

        while (timestampStr.size() < 14) {
            timestampStr = timestampStr + std::to_string(dis(gen)).c_str();
        }
    }
    return timestampStr;
}

const String& Order::getOrderNumber() const {
    return orderNumber;
}

const String& Order::getBookTicketUser() const {
    return bookTicketUser;
}

TicketStatus Order::getStatus() const {
    return status;
}

const LinkedList<OrderInfo>& Order::getOrderInfos() const {
    return orderInfos;
}

const String& Order::getFlightNumber(int segmentIndex) const {
    return orderInfos.getElementAt(segmentIndex).getFlightNumber();
}

const Ticket& Order::getTicket(int segmentIndex) const {
    return orderInfos.getElementAt(segmentIndex).getTicket();
}

const Passenger& Order::getPassenger(int segmentIndex) const {
    return orderInfos.getElementAt(segmentIndex).getPassenger();
}

const String& Order::getSeatNum(int segmentIndex) const {
    return orderInfos.getElementAt(segmentIndex).getSeatNum();
}

Meal Order::getMeal(int segmentIndex) const {
    return orderInfos.getElementAt(segmentIndex).getMeal();
}

void Order::setBookTicketUser(const String& bookTicketUser) {
    this->bookTicketUser = bookTicketUser;
}

void Order::setStatus(TicketStatus status) {
    this->status = status;
}

void Order::setOrderInfos(LinkedList<OrderInfo>&& orderInfos) {
    this->orderInfos = std::move(orderInfos);
}

void Order::setPassenger(int segmentIndex, const Passenger& passenger) {
    if (segmentIndex >= 0 && segmentIndex < orderInfos.size()) {
        OrderInfo newOrderInfo = orderInfos.getElementAt(segmentIndex);
        newOrderInfo.setPassenger(passenger);
        orderInfos.setElementAt(segmentIndex, newOrderInfo);
    }
}

void Order::setSeatNum(int segmentIndex, const String& seatNum) {
    if (segmentIndex >= 0 && segmentIndex < orderInfos.size()) {
        OrderInfo newOrderInfo = orderInfos.getElementAt(segmentIndex);
        newOrderInfo.setSeatNum(seatNum);
        orderInfos.setElementAt(segmentIndex, newOrderInfo);
    }
}

void Order::setMeal(int segmentIndex, Meal meal) {
    if (segmentIndex >= 0 && segmentIndex < orderInfos.size()) {
        OrderInfo newOrderInfo = orderInfos.getElementAt(segmentIndex);
        newOrderInfo.setMeal(meal);
        orderInfos.setElementAt(segmentIndex, newOrderInfo);
    }
}

int Order::getNumberOfSegments() const {
    return orderInfos.size();
}

std::istream& operator>>(std::istream& is, Order& order) {
    int statusInt;
    is >> order.orderNumber >> order.bookTicketUser >> statusInt >> order.orderInfos;
    order.status = static_cast<TicketStatus>(statusInt);
    return is;
}

std::ostream& operator<<(std::ostream& os, const Order& order) {
    os << order.orderNumber << "\n" << order.bookTicketUser << "\n"
       << static_cast<int>(order.status) << "\n";
    os << order.orderInfos;
    return os;
}
