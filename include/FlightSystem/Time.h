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
    bool operator<(const Time& other) const;
    bool operator<=(const Time& other) const;
    bool operator>(const Time& other) const;
    bool operator>=(const Time& other) const;
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
    bool operator<=(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator>=(const Date& other) const;
    Date operator+(int days) const;
    Date operator-(int days) const;
};

class DateTime {
private:
    Date date;
    Time time;

public:
    DateTime();
    DateTime(const Date& date, const Time& time);
    DateTime(int year, int month, int day, int hours, int minutes, int seconds);

    // Getters and Setters
    Date getDate() const;
    void setDate(const Date& date);
    Time getTime() const;
    void setTime(const Time& time);

    // Arithmetic operations
    DateTime operator+(int seconds) const;  // Add seconds to the DateTime
    DateTime operator-(int seconds) const;  // Subtract seconds from the DateTime
    DateTime operator+(const Time& duration) const;
    DateTime operator-(const Time& duration) const;

    // Comparison operators
    bool operator==(const DateTime& other) const;
    bool operator!=(const DateTime& other) const;
    bool operator<(const DateTime& other) const;
    bool operator>(const DateTime& other) const;

    // Conversion to/from string
    String toString() const;
    static DateTime fromString(const String& str);

    // I/O operators
    friend std::ostream& operator<<(std::ostream& out, const DateTime& dateTime);
    friend std::istream& operator>>(std::istream& in, DateTime& dateTime);
};
