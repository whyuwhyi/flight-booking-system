#pragma once

#include <FlightSystem/Flight.h>
#include <FlightSystem/Passenger.h>

class Ticket {
private:
    String flight;           // 航班信息
    Passenger passenger;     // 乘客信息
    String seatNumber;       // 座位号
    double price;            // 票价

public:
    Ticket(const Flight& flight, const Passenger& passenger, const String& seatNumber, double price);

    Ticket(const Ticket& other);

    const Flight& getFlight() const;
    const Passenger& getPassenger() const;
    const String& getSeatNumber() const;
    double getPrice() const;
};


