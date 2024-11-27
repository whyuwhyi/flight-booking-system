#pragma once

#include <String/String.h>
#include <iostream>

enum Meal {
    NO_MEAL,
    WESTERN,
    CHINESE,
    VEGETARIAN
};

class Passenger {
private:
    String name;
    String idNumber;
    String seatNum;
    Meal meal;

public:
    Passenger();
    Passenger(const String& name, const String& idNumber, const String& seatNum = "NULL", Meal meal = NO_MEAL);
    Passenger(const Passenger& other);

    const String& getName() const;
    const String& getIdNumber() const;
    const String& getSeatNum() const;
    Meal getMeal() const;

    void setName(const String& name);
    void setIdNumber(const String& idNumber);
    void setSeatNum(const String& seatNum);
    void setMeal(Meal meal);

    friend std::istream& operator>>(std::istream& is, Passenger& passenger);
    friend std::ostream& operator<<(std::ostream& os, const Passenger& passenger);
};
