#pragma once

#include <FlightSystem/Flight.h>
#include <FlightSystem/Passenger.h>

class Ticket {
private:
    Flight flight;           // 航班信息
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

Ticket::Ticket(const Flight& flight, const Passenger& passenger, const String& seatNumber, double price)
    : flight(flight), passenger(passenger), seatNumber(seatNumber), price(price) {}

Ticket::Ticket(const Ticket& other)
    : flight(other.flight), passenger(other.passenger), seatNumber(other.seatNumber), price(other.price) {}

const Flight& Ticket::getFlight() const { return flight; }
const Passenger& Ticket::getPassenger() const { return passenger; }
const String& Ticket::getSeatNumber() const { return seatNumber; }
double Ticket::getPrice() const { return price; }

