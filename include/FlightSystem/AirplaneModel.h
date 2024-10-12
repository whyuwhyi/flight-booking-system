#pragma once

#include <String/String.h>
#include <iostream>
#include <stdexcept>



enum CabinType { FirstClass, BusinessClass, EconomyClass };

class Cabin {
public:
    int rows;
    int columns;

    Cabin(int r = 0, int c = 0);

    int getPassengerCapacity() const;
    int getRows() const;
    void setRows(int r);
    int getColumns() const;
    void setColumns(int c);

    friend std::ostream& operator<<(std::ostream& out, const Cabin& cabin);
    friend std::istream& operator>>(std::istream& in, Cabin& cabin);
};

class AirplaneModel {
private:
    String name;
    Cabin firstClassCabin;
    Cabin businessClassCabin;
    Cabin economyClassCabin;

public:

    AirplaneModel();
    AirplaneModel(const String &name, int capacity = 0);

    const String& getName() const;
    void setName(const String& name);

    int getPassengerCapacity() const;

    Cabin& getCabin(CabinType type);
    const Cabin& getCabin(CabinType type) const;
    void setCabin(CabinType type, const Cabin& cabin);

    friend std::ostream& operator<<(std::ostream& out, const AirplaneModel& airplane);
    friend std::istream& operator>>(std::istream& in, AirplaneModel& airplane);
};