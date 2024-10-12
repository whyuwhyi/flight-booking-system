#include <FlightSystem/Ticket.h>
#include <LinkedList/LinkedList.h>
#include <stdexcept>

Ticket::Ticket() : flight(nullptr), flightTicketDetail(nullptr) {}

Ticket::Ticket(Flight* flight, FlightTicketDetail* flightTicketDetail) : flight(flight), flightTicketDetail(flightTicketDetail) {}

Ticket::Ticket(const Ticket& other) : flight(other.flight), flightTicketDetail(other.flightTicketDetail) {}

Ticket::~Ticket() {}

Ticket& Ticket::operator=(const Ticket& other) {
    if (this != &other) {
        flight = other.flight;
        flightTicketDetail = other.flightTicketDetail;
    }
    return *this;
}

Flight* Ticket::getFlight() const {
    return flight;
}

void Ticket::setFlight(Flight* flight) {
    this->flight = flight;
}

FlightTicketDetail* Ticket::getFlightTicketDetail() const {
    return flightTicketDetail;
}

void Ticket::setFlightTicketDetail(FlightTicketDetail* flightTicketDetail) {
    this->flightTicketDetail = flightTicketDetail;
}

DateTime Ticket::getDepartureDateTime() const {
    return DateTime(flightTicketDetail->getFlightDate(), flight->getDepartureTime());
}

DateTime Ticket::getArrivalDateTime() const {
    return getDepartureDateTime() + flight->getCostTime();
}

bool Ticket::operator<(const Ticket& other) const {
    return flightTicketDetail->getFlightDate() < other.flightTicketDetail->getFlightDate();
}

ConnectingTicket::ConnectingTicket() : tickets() {}

ConnectingTicket::ConnectingTicket(const ConnectingTicket& other) : tickets(other.tickets) {}

ConnectingTicket::~ConnectingTicket() {}

ConnectingTicket& ConnectingTicket::operator=(const ConnectingTicket& other) {
    if (this != &other) {
        tickets = other.tickets;
    }
    return *this;
}

void ConnectingTicket::addTicket(const Ticket& ticket) {
    if (!tickets.isEmpty()) {
        Link<Ticket>* last = tickets.getLast();
        DateTime lastArrival = last->getElement().getArrivalDateTime();
        DateTime currentDeparture = ticket.getDepartureDateTime();
        if (!(last->getElement().getFlight()->getArrivalAirport().getCity() == ticket.getFlight()->getDepartureAirport().getCity()) ||
            !(lastArrival.getDate() == currentDeparture.getDate() || lastArrival.getDate() + 1 == currentDeparture.getDate())) {
            throw std::invalid_argument("联程航班的出发城市与上一个航班的到达城市不匹配，或者出发时间不符合要求");
        }
    }
    tickets.append(ticket);
}

void ConnectingTicket::removeLastTicket() {
    tickets.removeLast();
}

int ConnectingTicket::getNumberOfTickets() const {
    return tickets.size();
}

LinkedList<Ticket> ConnectingTicket::getTickets() const {
    return tickets;
}

DateTime ConnectingTicket::getDepartureDateTime() const {
    if (tickets.isEmpty()) return DateTime();
    return tickets.getHead()->getElement().getDepartureDateTime();
}

DateTime ConnectingTicket::getArrivalDateTime() const {
    if (tickets.isEmpty()) return DateTime();
    Link<Ticket>* current = tickets.getLast();
    return current->getElement().getArrivalDateTime();
}

double ConnectingTicket::getTotalPrice(CabinType cabinType) const {
    double total = 0.0;
    Link<Ticket>* current = tickets.getHead();
    while (current != nullptr) {
        total += current->getElement().getFlightTicketDetail()->getCabinPrice(cabinType);
        current = current->getNext();
    }
    return total;
}

Time ConnectingTicket::getTotalDuration() const {
    Time total;
    Link<Ticket>* current = tickets.getHead();
    while (current != nullptr) {
        total = total + current->getElement().getFlight()->getCostTime();
        current = current->getNext();
    }
    return total;
}

bool ConnectingTicket::isValid() const {
    if (tickets.size() <= 1) return false;
    Link<Ticket>* current = tickets.getHead();
    Ticket* previousTicket = nullptr;
    while (current != nullptr) {
        Ticket* ticket = &current->getElement();
        if (previousTicket != nullptr) {
            DateTime lastArrival = previousTicket->getArrivalDateTime();
            DateTime currentDeparture = ticket->getDepartureDateTime();
            if (!(previousTicket->getFlight()->getArrivalAirport().getCity() == ticket->getFlight()->getDepartureAirport().getCity()) ||
                !(lastArrival.getDate() == currentDeparture.getDate() || lastArrival.getDate() + 1 == currentDeparture.getDate())) {
                return false;
            }
        }
        previousTicket = ticket;
        current = current->getNext();
    }
    return true;
}

bool ConnectingTicket::operator<(const ConnectingTicket& other) const {
    return getTotalDuration() < other.getTotalDuration();
}