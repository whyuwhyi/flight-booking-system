#include <FlightSystem/AirplaneModel.h>

Cabin::Cabin(int r, int c) : rows(r), columns(c) {}

int Cabin::getPassengerCapacity() const {
    return rows * columns;
}

int Cabin::getRows() const {
    return rows;
}

void Cabin::setRows(int r) {
    rows = r;
}

int Cabin::getColumns() const {
    return columns;
}

void Cabin::setColumns(int c) {
    columns = c;
}

std::ostream& operator<<(std::ostream& out, const Cabin& cabin) {
    out << cabin.rows <<" " << cabin.columns;
    return out;
}

std::istream& operator>>(std::istream& in, Cabin& cabin) {
    in >> cabin.rows >> cabin.columns;
    return in;
}

// AirplaneModel class implementation
AirplaneModel::AirplaneModel() : name(""), firstClassCabin(), businessClassCabin(), economyClassCabin() {}

AirplaneModel::AirplaneModel(const String &name, int capacity) : name(name) {
    // Capacity can be used to initialize cabins if needed
}

const String& AirplaneModel::getName() const {
    return name;
}

void AirplaneModel::setName(const String& name) {
    this->name = name;
}

int AirplaneModel::getPassengerCapacity() const {
    return firstClassCabin.getPassengerCapacity() +
           businessClassCabin.getPassengerCapacity() +
           economyClassCabin.getPassengerCapacity();
}

Cabin& AirplaneModel::getCabin(CabinType type) {
    switch (type) {
        case FirstClass: return firstClassCabin;
        case BusinessClass: return businessClassCabin;
        case EconomyClass: return economyClassCabin;
    }
    throw std::out_of_range("Invalid cabin type");
}

const Cabin& AirplaneModel::getCabin(CabinType type) const {
    switch (type) {
        case FirstClass: return firstClassCabin;
        case BusinessClass: return businessClassCabin;
        case EconomyClass: return economyClassCabin;
    }
    throw std::out_of_range("Invalid cabin type");
}

void AirplaneModel::setCabin(CabinType type, const Cabin& cabin) {
    switch (type) {
        case FirstClass: firstClassCabin = cabin; break;
        case BusinessClass: businessClassCabin = cabin; break;
        case EconomyClass: economyClassCabin = cabin; break;
    }
}

std::ostream& operator<<(std::ostream& out, const AirplaneModel& airplane) {
    out << airplane.name << "\n"
        << airplane.firstClassCabin << "\n"
        << airplane.businessClassCabin << "\n"
        << airplane.economyClassCabin << "\n";
    return out;
}

std::istream& operator>>(std::istream& in, AirplaneModel& airplane) {
    std::string name;
    int firstRows, firstCols, businessRows, businessCols, economyRows, economyCols;

    in >> name >> firstRows >> firstCols >> businessRows >> businessCols >> economyRows >> economyCols;

    airplane.name = name.c_str();
    airplane.firstClassCabin = Cabin(firstRows, firstCols);
    airplane.businessClassCabin = Cabin(businessRows, businessCols);
    airplane.economyClassCabin = Cabin(economyRows, economyCols);

    return in;
}