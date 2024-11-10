#pragma once

#include <iostream>
#include <String/String.h> 
#include <FlightSystem/Time.h>
#include <FlightSystem/Flight.h>
#include <FlightSystem/Passenger.h>

enum TicketStatus {
    BOOKED,
    CANCELED,
    CHECKED_IN,
    REFUNDED
};

enum Meal {
    NO_MEAL,
    WESTERN,
    CHINESE,
    VEGETARIAN
};

class Order {
private:
    String orderNumber;
    String flightNumber;
    String bookTicketUser;
    Passenger passenger;
    CabinType cabin;
    int price;
    Date date;
    String seatNum;
    enum Meal meal;
    TicketStatus status;

public:
    // Constructors
    Order();
    Order(const String& flightNumber, const String& bookTicketUser, const Passenger& passenger,
               CabinType cabin, int price, const Date& date, const String& seatNum = String(), TicketStatus status = BOOKED);
    Order(const Order& order);

    // Getters
    const String& getOrderNumber() const;
    const String& getFlightNumber() const;
    const String& getBookTicketUser() const;
    const Passenger& getPassenger() const;
    CabinType getCabinType() const;
    int getPrice() const;
    const Date& getDate() const;
    const String& getSeatNum() const;
    TicketStatus getStatus() const;
    enum Meal getMeal() const;

    // Setters
    void setFlightNumber(const String& flightNumber);
    void setBookTicketUser(const String& bookTicketUser);
    void setPassenger(const Passenger& passenger);
    void setCabinType(CabinType cabin);
    void setPrice(int price);
    void setDate(const Date& date);
    void setSeatNum(const String& seatNum);
    void setMeal(enum Meal meal);
    void setStatus(TicketStatus status);

    friend std::istream& operator>>(std::istream& is, Order& ticket);
    friend std::ostream& operator<<(std::ostream& os, const Order& ticket);
};
