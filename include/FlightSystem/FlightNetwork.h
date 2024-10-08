#pragma once

#include <FlightSystem/Flight.h>
#include <String/String.h>
#include <Map/Map.h>
#include <Map/HashMap.h>
#include <functional>
#include <stdexcept>

class FlightNetwork {
private:
    HashMap<String, int> cityIndexMap; // 存储城市名到索引的映射
    Map<Flight*, Flight*>** flightNetwork; // 二维Map，用于存储每对城市之间的航班

    // 内部方法：分配和释放网络内存
    void allocateNetwork(int cityCount);
    void deallocateNetwork();

public:
    // 构造和析构
    FlightNetwork(int cityCount);
    ~FlightNetwork();

    // 城市管理
    void addCity(const String& city);
    bool cityExists(const String& city) const;
    int getCityIndex(const String& city) const;

    // 航班管理
    void addFlight(const String& departureCity, const String& arrivalCity, Flight* flight);
    Map<Flight*, Flight*>* getFlightsBetween(const String& departureCity, const String& arrivalCity);

    // 其他功能
    void traverseCities(std::function<void(const String&)> func) const;
    void traverseFlights(std::function<void(const Flight&)> func) const;
};
