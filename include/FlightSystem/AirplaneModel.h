#pragma once

#include <String/String.h>
#include <iostream>
#include <stdexcept>

class Cabin {
public:
    int rows;    // 行数
    int columns; // 列数

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
    String name;                   // 机型名称
    Cabin firstClassCabin;         // 头等舱
    Cabin businessClassCabin;      // 商务舱
    Cabin economyClassCabin;       // 经济舱

public:
    enum CabinType { FirstClass, BusinessClass, EconomyClass };

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