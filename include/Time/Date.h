#pragma once

struct Date {
    int year;
    int month;
    int day;

    Date(int year = 0, int month = 1, int day = 1);

};

Date::Date(int year, int month, int day) : year(year), month(month), day(day) {}

