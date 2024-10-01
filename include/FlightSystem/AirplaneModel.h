#pragma once

#include <String/String.h>
#include <iostream>

class Cabin {
public:
    int rows;    // 行数
    int columns; // 列数

    Cabin(int r = 0, int c = 0) : rows(r), columns(c) {}
};

class AirplaneModel {
private:
    String modelName;              // 机型名称
    int passengerCapacity;         // 乘客容量
    Cabin cabins[3];               // 三个舱位：头等舱、商务舱、经济舱

public:
    enum CabinType { FirstClass, BusinessClass, EconomyClass };

    AirplaneModel(const String &name = "", int capacity = 0);

    const String& getModelName() const;
    int getPassengerCapacity() const;
    
    void setCabin(CabinType type, int rows, int columns);
    const Cabin& getCabin(CabinType type) const;

    friend std::ostream& operator<<(std::ostream& out, const AirplaneModel& airplane);
    friend std::istream& operator>>(std::istream& in, AirplaneModel& airplane);
};

