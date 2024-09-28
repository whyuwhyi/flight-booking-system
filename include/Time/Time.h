#pragma once
#include <cstdio>

struct Time {
    int hours;
    int minutes;
    int seconds;

    Time(int hours = 0, int minutes = 0, int seconds = 0);

};

Time::Time(int hours, int minutes, int seconds) : hours(hours), minutes(minutes), seconds(seconds) {}


