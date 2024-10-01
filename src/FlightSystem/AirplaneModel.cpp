#include "FlightSystem/AirplaneModel.h"

AirplaneModel::AirplaneModel(const String &name, int capacity)
    : modelName(name), passengerCapacity(capacity) {
    cabins[FirstClass] = Cabin();   // 默认初始化
    cabins[BusinessClass] = Cabin(); // 默认初始化
    cabins[EconomyClass] = Cabin();   // 默认初始化
}

const String& AirplaneModel::getModelName() const {
    return modelName;
}

int AirplaneModel::getPassengerCapacity() const {
    return passengerCapacity;
}

void AirplaneModel::setCabin(CabinType type, int rows, int columns) {
    if (type >= FirstClass && type <= EconomyClass) {
        cabins[type] = Cabin(rows, columns);
    }
}

const Cabin& AirplaneModel::getCabin(CabinType type) const {
    return cabins[type];
}

// 重载输出运算符
std::ostream& operator<<(std::ostream& out, const AirplaneModel& airplane) {
    out << airplane.modelName << " " << airplane.passengerCapacity << "\n";
    for (const auto& cabin : airplane.cabins) {
        out << cabin.rows << " " << cabin.columns << "\n";
    }
    return out;
}

// 重载输入运算符
std::istream& operator>>(std::istream& in, AirplaneModel& airplane) {
    in >> airplane.modelName >> airplane.passengerCapacity;
    for (auto& cabin : airplane.cabins) {
        in >> cabin.rows >> cabin.columns;
    }
    return in;
}