#include <FlightSystem/Flight.h>
#include <file/fileManage.h>

// FlightTicketDetail class implementation
FlightTicketDetail::FlightTicketDetail() : firstClassPrice(0.0), businessClassPrice(0.0), economyClassPrice(0.0),
                                       firstClassTickets(0), businessClassTickets(0), economyClassTickets(0), flightDate() {}

FlightTicketDetail::FlightTicketDetail(double firstClassPrice, double businessClassPrice, double economyClassPrice,
                                 int firstClassTickets, int businessClassTickets, int economyClassTickets, const Date& flightDate)
    : firstClassPrice(firstClassPrice), businessClassPrice(businessClassPrice), economyClassPrice(economyClassPrice),
      firstClassTickets(firstClassTickets), businessClassTickets(businessClassTickets), economyClassTickets(economyClassTickets), flightDate(flightDate) {}

double FlightTicketDetail::getCabinPrice(CabinType type) const {
    switch (type) {
        case FirstClass: return firstClassPrice;
        case BusinessClass: return businessClassPrice;
        case EconomyClass: return economyClassPrice;
        default: throw std::invalid_argument("Invalid cabin type");
    }
}

void FlightTicketDetail::setCabinPrice(CabinType type, double price) {
    switch (type) {
        case FirstClass: firstClassPrice = price; break;
        case BusinessClass: businessClassPrice = price; break;
        case EconomyClass: economyClassPrice = price; break;
        default: throw std::invalid_argument("Invalid cabin type");
    }
}

int FlightTicketDetail::getRemainingTickets(CabinType type) const {
    switch (type) {
        case FirstClass: return firstClassTickets;
        case BusinessClass: return businessClassTickets;
        case EconomyClass: return economyClassTickets;
        default: throw std::invalid_argument("Invalid cabin type");
    }
}

void FlightTicketDetail::setRemainingTickets(CabinType type, int tickets) {
    switch (type) {
        case FirstClass: firstClassTickets = tickets; break;
        case BusinessClass: businessClassTickets = tickets; break;
        case EconomyClass: economyClassTickets = tickets; break;
        default: throw std::invalid_argument("Invalid cabin type");
    }
}

const Date& FlightTicketDetail::getFlightDate() const {
    return flightDate;
}

void FlightTicketDetail::setFlightDate(const Date& date) {
    flightDate = date;
}

std::ostream& operator<<(std::ostream& out, const FlightTicketDetail& ticketInfo) {
    out << ticketInfo.firstClassPrice << " " << ticketInfo.businessClassPrice << " " << ticketInfo.economyClassPrice << "\n"
        << ticketInfo.firstClassTickets << " " << ticketInfo.businessClassTickets << " " << ticketInfo.economyClassTickets << "\n"
        << ticketInfo.flightDate;
    return out;
}

std::istream& operator>>(std::istream& in, FlightTicketDetail& ticketInfo) {
    in >> ticketInfo.firstClassPrice >> ticketInfo.businessClassPrice >> ticketInfo.economyClassPrice
       >> ticketInfo.firstClassTickets >> ticketInfo.businessClassTickets >> ticketInfo.economyClassTickets >> ticketInfo.flightDate;
    return in;
}

// Flight class implementation
Flight::Flight() : flightNumber(), airline(), airplaneModel(), departureAirport(), arrivalAirport(),
                   airRoute(), departureTime(), costTime(), firstClassCabin(),
                   businessClassCabin(), economyClassCabin(),
                   initialFirstClassPrice(0.0), initialBusinessClassPrice(0.0), initialEconomyClassPrice(0.0),
                   flightScheduleMap([] (const FlightTicketDetail& ticketInfo) { return ticketInfo.getFlightDate(); }) {}

Flight::Flight(const String& flightNumber, const String& airline, const String& airplaneModel,
               const Airport& departureAirport, const Airport& arrivalAirport, const String& airRoute,
               const Time& departureTime, const Time& costTime, double initialFirstClassPrice,
               double initialBusinessClassPrice, double initialEconomyClassPrice)
    : flightNumber(flightNumber), airline(airline), airplaneModel(airplaneModel), departureAirport(departureAirport),
      arrivalAirport(arrivalAirport), airRoute(airRoute), departureTime(departureTime),
      costTime(costTime), firstClassCabin(), businessClassCabin(), economyClassCabin(),
      initialFirstClassPrice(initialFirstClassPrice), initialBusinessClassPrice(initialBusinessClassPrice),
      initialEconomyClassPrice(initialEconomyClassPrice),
      flightScheduleMap([] (const FlightTicketDetail& ticketInfo) { return ticketInfo.getFlightDate(); }) {}

const String& Flight::getFlightName() const {
    return flightNumber;
}

void Flight::setFlightName(const String& flightNumber) {
    this->flightNumber = flightNumber;
}

const String& Flight::getAirline() const {
    return airline;
}

void Flight::setAirline(const String& airline) {
    this->airline = airline;
}

const String& Flight::getAirplaneModel() const {
    return airplaneModel;
}

void Flight::setAirplaneModel(const String& airplaneModel) {
    this->airplaneModel = airplaneModel;
}

const Airport& Flight::getDepartureAirport() const {
    return departureAirport;
}

void Flight::setDepartureAirport(const Airport& departureAirport) {
    this->departureAirport = departureAirport;
}

const Airport& Flight::getArrivalAirport() const {
    return arrivalAirport;
}

void Flight::setArrivalAirport(const Airport& arrivalAirport) {
    this->arrivalAirport = arrivalAirport;
}

const String& Flight::getairRoute() const {
    return airRoute;
}

void Flight::setairRoute(const String& airRoute) {
    this->airRoute = airRoute;
}

const Time& Flight::getDepartureTime() const {
    return departureTime;
}

void Flight::setDepartureTime(const Time& departureTime) {
    this->departureTime = departureTime;
}

const Time& Flight::getCostTime() const {
    return costTime;
}

void Flight::setCostTime(const Time& costTime) {
    this->costTime = costTime;
}

Cabin& Flight::getCabin(CabinType type) {
    switch (type) {
        case FirstClass: return firstClassCabin;
        case BusinessClass: return businessClassCabin;
        case EconomyClass: return economyClassCabin;
        default: throw std::invalid_argument("Invalid cabin type");
    }
}

const Cabin& Flight::getCabin(CabinType type) const {
    switch (type) {
        case FirstClass: return firstClassCabin;
        case BusinessClass: return businessClassCabin;
        case EconomyClass: return economyClassCabin;
        default: throw std::invalid_argument("Invalid cabin type");
    }
}

void Flight::setCabin(CabinType type, const Cabin& cabin) {
    switch (type) {
        case FirstClass: firstClassCabin = cabin; break;
        case BusinessClass: businessClassCabin = cabin; break;
        case EconomyClass: economyClassCabin = cabin; break;
        default: throw std::invalid_argument("Invalid cabin type");
    }
}

bool Flight::hasFlightOnDate(const Date& date) const {
    return flightScheduleMap.find(date) != nullptr;
}

bool Flight::addFlightSchedule(const FlightTicketDetail& ticketInfo) {
    if (flightScheduleMap.insert(ticketInfo)) {
        String fileName = FLIGHTS_DIR + flightNumber + "/" + ticketInfo.getFlightDate().toString() + ".txt";
        return writeMapToFile(flight_map, FLIGHTS_PATH.c_str()) && createFile(fileName.c_str());
    }
    return false;
}

bool Flight::removeFlightSchedule(const Date& date) {
    if (flightScheduleMap.erase(date)) {
        String fileName = FLIGHTS_DIR + flightNumber + "/" + date.toString() + ".txt";
        return writeMapToFile(flight_map, FLIGHTS_PATH.c_str()) && removeFile(fileName.c_str());
    }
    return false;
}

const FlightScheduleMap& Flight::getFlightSchedule() const {
    return flightScheduleMap;
}

FlightTicketDetail* Flight::getFlightTicketDetail(const Date& date) {
    return flightScheduleMap.find(date);
}

double Flight::getInitialPrice(CabinType type) const {
    switch (type) {
        case FirstClass:
            return initialFirstClassPrice;
        case BusinessClass:
            return initialBusinessClassPrice;
        case EconomyClass:
            return initialEconomyClassPrice;
        default:
            throw std::invalid_argument("Invalid cabin type");
    }
}

void Flight::setInitialPrice(CabinType type, double price) {
    switch (type) {
        case FirstClass:
            initialFirstClassPrice = price;
            break;
        case BusinessClass:
            initialBusinessClassPrice = price;
            break;
        case EconomyClass:
            initialEconomyClassPrice = price;
            break;
        default:
            throw std::invalid_argument("Invalid cabin type");
    }
}

std::ostream& operator<<(std::ostream& out, const Flight& flight) {
    out << flight.flightNumber << "\n"
        << flight.airline << "\n"
        << flight.airplaneModel << "\n"
        << flight.departureAirport
        << flight.arrivalAirport
        << flight.airRoute << "\n"
        << flight.departureTime << " " << flight.costTime << "\n"
        << flight.firstClassCabin << "\n"
        << flight.businessClassCabin << "\n"
        << flight.economyClassCabin << "\n"
        << flight.initialFirstClassPrice << " " << flight.initialBusinessClassPrice << " " << flight.initialEconomyClassPrice << "\n"
        << flight.flightScheduleMap;
    return out;
}

std::istream& operator>>(std::istream& in, Flight& flight) {
    in >> flight.flightNumber >> flight.airline >> flight.airplaneModel >> flight.departureAirport >> flight.arrivalAirport
       >> flight.airRoute >> flight.departureTime >> flight.costTime >> flight.firstClassCabin
       >> flight.businessClassCabin >> flight.economyClassCabin
       >> flight.initialFirstClassPrice >> flight.initialBusinessClassPrice >> flight.initialEconomyClassPrice
       >> flight.flightScheduleMap;
    return in;
}