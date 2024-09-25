#pragma once

#include <Time/Date.h>
#include <Time/Time.h>
#include <String/String.h>

class Flight {
private:
    String flightNumber;   // 航班编号
    String departureCity;  // 出发城市
    String arrivalCity;    // 到达城市
    Date departureDate;    // 出发日期
    Time departureTime;    // 出发时间
    Time arrivalTime;      // 到达时间

public:
    Flight(const String& flightNumber, const String& departureCity, const String& arrivalCity,
            const Date& departureDate, const Time& departureTime, const Time& arrivalTime);

    Flight(const Flight& other);

    const String& getFlightNumber() const;

    const String& getDepartureCity() const;
    const String& getArrivalCity() const;
    const Date& getDepartureDate() const;
    const Time& getDepartureTime() const;
    const Time& getArrivalTime() const;
    
};

inline Flight::Flight(const String& flightNumber, const String& departureCity, const String& arrivalCity,
                            const Date& departureDate, const Time& departureTime, const Time& arrivalTime)
    : flightNumber(flightNumber), departureCity(departureCity), arrivalCity(arrivalCity),
    departureDate(departureDate), departureTime(departureTime), arrivalTime(arrivalTime) {}

inline Flight::Flight(const Flight& other)
    : flightNumber(other.flightNumber), departureCity(other.departureCity), arrivalCity(other.arrivalCity),
    departureDate(other.departureDate), departureTime(other.departureTime), arrivalTime(other.arrivalTime) {}

const String& Flight::getFlightNumber() const { return flightNumber; }
const String& Flight::getDepartureCity() const { return departureCity; }
const String& Flight::getArrivalCity() const { return arrivalCity; }
const Date& Flight::getDepartureDate() const { return departureDate; }
const Time& Flight::getDepartureTime() const { return departureTime; }
const Time& Flight::getArrivalTime() const { return arrivalTime; }


