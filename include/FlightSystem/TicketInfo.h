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

class TicketInfo {
private:
    String flightNumber;
    String bookTicketUser;
    Passenger passenger;
    CabinType cabin;
    int price;
    Date date;
    String seatNum;
    TicketStatus status;

public:
    // Constructors
    TicketInfo();
    TicketInfo(const String& flightNumber, const String& bookTicketUser, const Passenger& passenger,
               CabinType cabin, int price, const Date& date, const String& seatNum = String(), TicketStatus status = BOOKED);

    // Getters
    const String& getFlightNumber() const;
    const String& getBookTicketUser() const;
    const Passenger& getPassenger() const;
    CabinType getCabinType() const;
    int getPrice() const;
    const Date& getDate() const;
    const String& getSeatNum() const;
    TicketStatus getStatus() const;

    // Setters
    void setFlightNumber(const String& flightNumber);
    void setBookTicketUser(const String& bookTicketUser);
    void setPassenger(const Passenger& passenger);
    void setCabinType(CabinType cabin);
    void setPrice(int price);
    void setDate(const Date& date);
    void setSeatNum(const String& seatNum);
    void setStatus(TicketStatus status);

    // Utility functions
    void cancelTicket();
    void checkIn();
    void refundTicket();

    
    friend std::istream& operator>>(std::istream& is, TicketInfo& ticket);
    friend std::ostream& operator<<(std::ostream& os, const TicketInfo& ticket);
};
