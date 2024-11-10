#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <stdexcept>
#include <String/String.h>

// 类声明部分
class Time {
public:
    Time(int hours = 0, int minutes = 0, int seconds = 0);

    int getHours() const;
    void setHours(int hours);

    int getMinutes() const;
    void setMinutes(int minutes);

    int getSeconds() const;
    void setSeconds(int seconds);

    // 总秒数表示
    long long toTotalSeconds() const;
    static Time fromTotalSeconds(long long totalSeconds);

    // 字符串表示
    String toString() const;
    static Time fromString(const String& timeStr);

    // 算术运算符
    Time operator+(const Time& other) const;
    Time operator-(const Time& other) const;
    Time& operator+=(const Time& other);
    Time& operator-=(const Time& other);

    // 比较运算符
    bool operator==(const Time& other) const;
    bool operator!=(const Time& other) const;
    bool operator<(const Time& other) const;
    bool operator<=(const Time& other) const;
    bool operator>(const Time& other) const;
    bool operator>=(const Time& other) const;

    // 流操作符
    friend std::ostream& operator<<(std::ostream& out, const Time& time);
    friend std::istream& operator>>(std::istream& in, Time& time);

private:
    long long totalSeconds; // 用于存储任意持续时间的总秒数
};

class Date {
public:
    Date(int year = 1970, int month = 1, int day = 1);

    int getYear() const;
    void setYear(int year);

    int getMonth() const;
    void setMonth(int month);

    int getDay() const;
    void setDay(int day);

    // 字符串表示
    String toString() const;
    static Date fromString(const String& dateStr);

    // 日期运算
    Date operator+(int days) const;
    Date operator-(int days) const;
    int operator-(const Date& other) const; // 返回天数差

    // 比较运算符
    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;
    bool operator<(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator>=(const Date& other) const;
    
    int toJulianDay() const;

    // 流操作符
    friend std::ostream& operator<<(std::ostream& out, const Date& date);
    friend std::istream& operator>>(std::istream& in, Date& date);

private:
    int year, month, day;

    // 辅助函数
    bool isLeapYear(int year) const;
    int daysInMonth(int year, int month) const;

    static Date fromJulianDay(int julianDay);
};

class DateTime {
public:
    DateTime();
    DateTime(const Date& date, const Time& time);
    DateTime(int year, int month, int day, int hours, int minutes, int seconds);

    const Date& getDate() const;
    void setDate(const Date& date);

    const Time& getTime() const;
    void setTime(const Time& time);

    // 字符串表示
    String toString() const;
    static DateTime fromString(const String& str);

    // 算术运算符
    DateTime operator+(const Time& duration) const;
    DateTime operator-(const Time& duration) const;
    Time operator-(const DateTime& other) const; // 返回时间差（Time类型）

    // 比较运算符
    bool operator==(const DateTime& other) const;
    bool operator!=(const DateTime& other) const;
    bool operator<(const DateTime& other) const;
    bool operator<=(const DateTime& other) const;
    bool operator>(const DateTime& other) const;
    bool operator>=(const DateTime& other) const;

    // 流操作符
    friend std::ostream& operator<<(std::ostream& out, const DateTime& dateTime);
    friend std::istream& operator>>(std::istream& in, DateTime& dateTime);

private:
    Date date;
    Time time;

    // 辅助函数
    long long toTotalSeconds() const;
    static DateTime fromTotalSeconds(long long totalSeconds);
};