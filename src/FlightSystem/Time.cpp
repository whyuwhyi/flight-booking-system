#include <FlightSystem/Time.h>
#include <String/String.h>
#include <iomanip>

// Time class implementation
Time::Time(int hours, int minutes, int seconds) : hours(hours), minutes(minutes), seconds(seconds) {}

int Time::getHours() const { return hours; }
void Time::setHours(int hours) { this->hours = hours; }

int Time::getMinutes() const { return minutes; }
void Time::setMinutes(int minutes) { this->minutes = minutes; }

int Time::getSeconds() const { return seconds; }
void Time::setSeconds(int seconds) { this->seconds = seconds; }

String Time::toString() const {
    return (std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds)).c_str();
}

Time Time::fromString(const String& timeStr) {
    int hours, minutes, seconds;
    sscanf(timeStr.c_str(), "%d:%d:%d", &hours, &minutes, &seconds);
    return Time(hours, minutes, seconds);
}

std::ostream& operator<<(std::ostream& out, const Time& time) {
    out << (time.hours < 10 ? "0" : "") << time.hours << ":"
        << (time.minutes < 10 ? "0" : "") << time.minutes << ":"
        << (time.seconds < 10 ? "0" : "") << time.seconds;
    return out;
}

std::istream& operator>>(std::istream& in, Time& time) {
    char delimiter;
    in >> time.hours >> delimiter >> time.minutes >> delimiter >> time.seconds;
    return in;
}

bool Time::operator==(const Time& other) const {
    return hours == other.hours && minutes == other.minutes && seconds == other.seconds;
}

bool Time::operator!=(const Time& other) const {
    return !(*this == other);
}

Time Time::operator+(const Time& other) const {
    int totalSeconds = (hours + other.hours) * 3600 + (minutes + other.minutes) * 60 + (seconds + other.seconds);
    return Time(totalSeconds / 3600, (totalSeconds % 3600) / 60, totalSeconds % 60);
}

Time Time::operator-(const Time& other) const {
    int totalSeconds = (hours - other.hours) * 3600 + (minutes - other.minutes) * 60 + (seconds - other.seconds);
    return Time(totalSeconds / 3600, (totalSeconds % 3600) / 60, totalSeconds % 60);
}

bool Time::operator<(const Time& other) const {
    if (hours != other.hours) {
        return hours < other.hours;
    }
    if (minutes != other.minutes) {
        return minutes < other.minutes;
    }
    return seconds < other.seconds;
}

bool Time::operator<=(const Time& other) const {
    return *this < other || *this == other;
}

bool Time::operator>(const Time& other) const {
    return !(*this <= other);
}

bool Time::operator>=(const Time& other) const {
    return !(*this < other);
}

Date::Date(int year, int month, int day) : year(year), month(month), day(day) {}

int Date::getYear() const { return year; }
void Date::setYear(int year) { this->year = year; }

int Date::getMonth() const { return month; }
void Date::setMonth(int month) { this->month = month; }

int Date::getDay() const { return day; }
void Date::setDay(int day) { this->day = day; }

String Date::toString() const {
    return (std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day)).c_str();
}

Date Date::fromString(const String& dateStr) {
    int year, month, day;
    sscanf(dateStr.c_str(), "%d-%d-%d", &year, &month, &day);
    return Date(year, month, day);
}

std::ostream& operator<<(std::ostream& out, const Date& date) {
    out << std::setw(4) << std::setfill('0') << date.year << "-"
        << (date.month < 10 ? "0" : "") << date.month << "-"
        << (date.day < 10 ? "0" : "") << date.day;
    return out;
}

std::istream& operator>>(std::istream& in, Date& date) {
    char delimiter1, delimiter2;
    in >> date.year >> delimiter1 >> date.month >> delimiter2 >> date.day;
    if (delimiter1 != '-' || delimiter2 != '-') {
        in.setstate(std::ios::failbit);
    }
    return in;
}

bool Date::operator==(const Date& other) const {
    return year == other.year && month == other.month && day == other.day;
}

bool Date::operator!=(const Date& other) const {
    return !(*this == other);
}

bool Date::operator<(const Date& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    return day < other.day;
}

bool Date::operator<=(const Date& other) const {
    return *this < other || *this == other;
}

bool Date::operator>(const Date& other) const {
    return other < *this;
}

bool Date::operator>=(const Date& other) const {
    return !(*this < other);
}

Date Date::operator+(int days) const {
    Date result = *this;
    result.day += days;
    return result;
}

Date Date::operator-(int days) const {
    Date result = *this;
    result.day -= days;
    return result;
}

DateTime::DateTime() : date(), time() {}

DateTime::DateTime(const Date& date, const Time& time) : date(date), time(time) {}

DateTime::DateTime(int year, int month, int day, int hours, int minutes, int seconds)
    : date(year, month, day), time(hours, minutes, seconds) {}

Date DateTime::getDate() const {
    return date;
}

void DateTime::setDate(const Date& date) {
    this->date = date;
}

Time DateTime::getTime() const {
    return time;
}

void DateTime::setTime(const Time& time) {
    this->time = time;
}

DateTime DateTime::operator+(int seconds) const {
    DateTime result(*this);
    result.time = result.time + Time(0, 0, seconds);

    if (result.time.getHours() >= 24) {
        result.date = result.date + (result.time.getHours() / 24);
        result.time.setHours(result.time.getHours() % 24);
    }
    return result;
}

DateTime DateTime::operator-(int seconds) const {
    DateTime result(*this);
    result.time = result.time - Time(0, 0, seconds);

    if (result.time.getHours() < 0) {
        result.date = result.date - 1;
        result.time.setHours(24 + result.time.getHours());
    }
    return result;
}

DateTime DateTime::operator+(const Time& duration) const {
    return *this + (duration.getHours() * 3600 + duration.getMinutes() * 60 + duration.getSeconds());
}

DateTime DateTime::operator-(const Time& duration) const {
    return *this - (duration.getHours() * 3600 + duration.getMinutes() * 60 + duration.getSeconds());
}

bool DateTime::operator==(const DateTime& other) const {
    return date == other.date && time == other.time;
}

bool DateTime::operator!=(const DateTime& other) const {
    return !(*this == other);
}

bool DateTime::operator<(const DateTime& other) const {
    if (date == other.date) {
        return time < other.time;
    }
    return date < other.date;
}

bool DateTime::operator>(const DateTime& other) const {
    return other < *this;
}

String DateTime::toString() const {
    std::ostringstream oss;
    oss << date.toString() << " " << time.toString();
    return String(oss.str().c_str());
}

DateTime DateTime::fromString(const String& str) {
    std::istringstream iss(str.c_str());
    Date date;
    Time time;
    char space;
    iss >> date >> space >> time;
    if (!iss) {
        throw std::runtime_error("Invalid DateTime format");
    }
    return DateTime(date, time);
}

std::ostream& operator<<(std::ostream& out, const DateTime& dateTime) {
    out << dateTime.date << " " << dateTime.time;
    return out;
}

std::istream& operator>>(std::istream& in, DateTime& dateTime) {
    in >> dateTime.date >> dateTime.time;
    return in;
}
