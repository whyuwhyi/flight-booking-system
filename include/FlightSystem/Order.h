#pragma once

#include <iostream>
#include <String/String.h> 
#include <FlightSystem/Time.h>
#include <FlightSystem/Flight.h>
#include <FlightSystem/Ticket.h>
#include <FlightSystem/Passenger.h>
#include <LinkedList/LinkedList.h>

enum TicketStatus {
    BOOKED,
    CANCELED,
    CHECKED_IN,
    REFUNDED
};

class OrderInfo {
private:
    String flightNumber;
    String airRoute;
    String airplaneModel;
    String airline;
    TimeSlot departureTimeSlot;
    CabinType cabin;
    double price;
    Date date;
    Ticket ticket;
    Passenger passenger;

public:
    OrderInfo();
    OrderInfo(const String& flightNumber, const String& airRoute, const String& airplaneModel, const String& airline, 
              TimeSlot departureTimeSlot, CabinType cabin, double price, const Date& date, const Ticket& ticket,
              const Passenger& passenger);

    const String& getFlightNumber() const;
    const String& getAirRoute() const;
    const String& getAirplaneModel() const;
    const String& getAirline() const;
    TimeSlot getDepartureTimeSlot() const;
    CabinType getCabinType() const;
    int getPrice() const;
    const Date& getDate() const;
    const Ticket& getTicket() const;
    const Passenger& getPassenger() const;
    const String& getSeatNum() const;
    Meal getMeal() const;

    void setFlightNumber(const String& flightNumber);
    void setAirRoute(const String& airRoute);
    void setAirplaneModel(const String& airplaneModel);
    void setAirline(const String& airline);
    void setDepartureTimeSlot(TimeSlot departureTimeSlot);
    void setCabinType(CabinType cabin);
    void setPrice(int price);
    void setDate(const Date& date);
    void setTicket(const Ticket& ticket);
    void setPassenger(const Passenger& passenger);
    void setSeatNum(const String& seatNum);
    void setMeal(Meal meal);

    friend std::istream& operator>>(std::istream& is, OrderInfo& orderInfo);
    friend std::ostream& operator<<(std::ostream& os, const OrderInfo& orderInfo);
};

class Order {
private:
    String orderNumber;
    String bookTicketUser;
    TicketStatus status;
    LinkedList<OrderInfo> orderInfos;

    String generateOrderNumber();

public:
    Order();
    Order(const String& bookTicketUser, TicketStatus status = BOOKED);
    Order(const Order& order);

    const String& getOrderNumber() const;
    const String& getBookTicketUser() const;
    TicketStatus getStatus() const;
    const LinkedList<OrderInfo>& getOrderInfos() const;
    const String& getFlightNumber(int segmentIndex) const;
    const Ticket& getTicket(int segmentIndex) const;
    const Passenger& getPassenger(int segmentIndex) const;
    const String& getSeatNum(int segmentIndex) const;
    Meal getMeal(int segmentIndex) const;

    void setBookTicketUser(const String& bookTicketUser);
    void setStatus(TicketStatus status);
    void setOrderInfos(LinkedList<OrderInfo>&& orderInfos);
    void setPassenger(int segmentIndex, const Passenger& passenger);
    void setSeatNum(int segmentIndex, const String& seatNum);
    void setMeal(int segmentIndex, Meal meal);

    int getNumberOfSegments() const;

    friend std::istream& operator>>(std::istream& is, Order& order);
    friend std::ostream& operator<<(std::ostream& os, const Order& order);
};
