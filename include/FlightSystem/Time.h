#pragma once

#include <iostream>
#include <String/String.h>

class Time {
private:
    int hours;
    int minutes;
    int seconds;

public:
    Time(int hours = 0, int minutes = 0, int seconds = 0);

    int getHours() const;
    void setHours(int hours);
    int getMinutes() const;
    void setMinutes(int minutes);
    int getSeconds() const;
    void setSeconds(int seconds);

    String toString() const;
    static Time fromString(const String& str);

    friend std::ostream& operator<<(std::ostream& out, const Time& time);
    friend std::istream& operator>>(std::istream& in, Time& time);

    bool operator==(const Time& other) const;
    bool operator!=(const Time& other) const;
    Time operator+(const Time& other) const;
    Time operator-(const Time& other) const;
};

class Date {
private:
    int year;
    int month;
    int day;

public:
    Date(int year = 0, int month = 1, int day = 1);

    int getYear() const;
    void setYear(int year);
    int getMonth() const;
    void setMonth(int month);
    int getDay() const;
    void setDay(int day);

    String toString() const;
    static Date fromString(const String& str);

    friend std::ostream& operator<<(std::ostream& out, const Date& date);
    friend std::istream& operator>>(std::istream& in, Date& date);

    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;
    bool operator<(const Date& other) const;
    bool operator>(const Date& other) const;
    Date operator+(int days) const;
    Date operator-(int days) const;
};