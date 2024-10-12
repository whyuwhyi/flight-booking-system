// Tickets.h
#pragma once

#include <FlightSystem/Flight.h>
#include <FlightSystem/Time.h>
#include <Map/Map.h>
#include <Map/HashMap.h>
#include <functional>
#include <stdexcept>
#include <LinkedList/LinkedList.h>

class Ticket {
private:
    Flight* flight;
    FlightTicketDetail* flightTicketDetail;

public:
    Ticket();
    Ticket(Flight* flight, FlightTicketDetail* flightTicketDetail);
    Ticket(const Ticket& other);
    ~Ticket();

    Ticket& operator=(const Ticket& other);

    Flight* getFlight() const;
    void setFlight(Flight* flight);

    FlightTicketDetail* getFlightTicketDetail() const;
    void setFlightTicketDetail(FlightTicketDetail* flightTicketDetail);

    DateTime getDepartureDateTime() const;
    DateTime getArrivalDateTime() const;

    bool operator<(const Ticket& other) const;
};

class ConnectingTicket {
private:
    LinkedList<Ticket> tickets;

public:
    ConnectingTicket();
    ConnectingTicket(const ConnectingTicket& other);
    ~ConnectingTicket();

    ConnectingTicket& operator=(const ConnectingTicket& other);

    void addTicket(const Ticket& ticket);
    void removeLastTicket();
    int getNumberOfTickets() const;
    LinkedList<Ticket> getTickets() const;

    double getTotalPrice(CabinType cabinType) const;
    Time getTotalDuration() const;
    DateTime getDepartureDateTime() const;
    DateTime getArrivalDateTime() const;

    bool isValid() const;

    bool operator<(const ConnectingTicket& other) const;
};