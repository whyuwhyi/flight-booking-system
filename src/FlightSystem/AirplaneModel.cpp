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
