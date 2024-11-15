#include <FlightSystem/Time.h>
// -------------------- Time 类实现 --------------------

Time::Time(int hours, int minutes, int seconds) {
    totalSeconds = static_cast<long long>(hours) * 3600 + minutes * 60 + seconds;
}

int Time::getHours() const {
    return static_cast<int>(totalSeconds / 3600);
}

void Time::setHours(int hours) {
    totalSeconds = static_cast<long long>(hours) * 3600 + getMinutes() * 60 + getSeconds();
}

int Time::getMinutes() const {
    return static_cast<int>((totalSeconds % 3600) / 60);
}

void Time::setMinutes(int minutes) {
    totalSeconds = getHours() * 3600 + minutes * 60 + getSeconds();
}

int Time::getSeconds() const {
    return static_cast<int>(totalSeconds % 60);
}

void Time::setSeconds(int seconds) {
    totalSeconds = getHours() * 3600 + getMinutes() * 60 + seconds;
}

long long Time::toTotalSeconds() const {
    return totalSeconds;
}

Time Time::fromTotalSeconds(long long totalSeconds) {
    Time time;
    time.totalSeconds = totalSeconds;
    return time;
}

String Time::toString() const {
    int hours = getHours();
    int minutes = getMinutes();
    int seconds = getSeconds();
    char buffer[20];
    std::snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", hours, minutes, seconds);
    return String(buffer);
}

Time Time::fromString(const String& timeStr) {
    int hours, minutes, seconds;
    sscanf(timeStr.c_str(), "%d:%d:%d", &hours, &minutes, &seconds);
    return Time(hours, minutes, seconds);
}

Time Time::operator+(const Time& other) const {
    return Time::fromTotalSeconds(totalSeconds + other.totalSeconds);
}

Time Time::operator-(const Time& other) const {
    return Time::fromTotalSeconds(totalSeconds - other.totalSeconds);
}

Time& Time::operator+=(const Time& other) {
    totalSeconds += other.totalSeconds;
    return *this;
}

Time& Time::operator-=(const Time& other) {
    totalSeconds -= other.totalSeconds;
    return *this;
}

bool Time::operator==(const Time& other) const {
    return totalSeconds == other.totalSeconds;
}

bool Time::operator!=(const Time& other) const {
    return totalSeconds != other.totalSeconds;
}

bool Time::operator<(const Time& other) const {
    return totalSeconds < other.totalSeconds;
}

bool Time::operator<=(const Time& other) const {
    return totalSeconds <= other.totalSeconds;
}

bool Time::operator>(const Time& other) const {
    return totalSeconds > other.totalSeconds;
}

bool Time::operator>=(const Time& other) const {
    return totalSeconds >= other.totalSeconds;
}

std::ostream& operator<<(std::ostream& out, const Time& time) {
    out << time.toString();
    return out;
}

std::istream& operator>>(std::istream& in, Time& time) {
    int hours, minutes, seconds;
    char delimiter1, delimiter2;
    in >> hours >> delimiter1 >> minutes >> delimiter2 >> seconds;
    if (delimiter1 != ':' || delimiter2 != ':') {
        in.setstate(std::ios::failbit);
        return in;
    }
    time = Time(hours, minutes, seconds);
    return in;
}

// -------------------- Date 类实现 --------------------

Date::Date(int year, int month, int day) : year(year), month(month), day(day) {}

int Date::getYear() const { return year; }
void Date::setYear(int year) { this->year = year; }

int Date::getMonth() const { return month; }
void Date::setMonth(int month) { this->month = month; }

int Date::getDay() const { return day; }
void Date::setDay(int day) { this->day = day; }

String Date::toString() const {
    char buffer[11];
    std::snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d", year, month, day);
    return String(buffer);
}

Date Date::fromString(const String& dateStr) {
    int year, month, day;
    sscanf(dateStr.c_str(), "%d-%d-%d", &year, &month, &day);
    return Date(year, month, day);
}

bool Date::isLeapYear(int year) const {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int Date::daysInMonth(int year, int month) const {
    static const int daysPerMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    if (month == 2 && isLeapYear(year))
        return 29;
    return daysPerMonth[month - 1];
}

int Date::toJulianDay() const {
    int a = (14 - month) / 12;
    int y = year + 4800 - a;
    int m = month + 12 * a - 3;
    return day + (153 * m + 2)/5 + y * 365 + y/4 - y/100 + y/400 - 32045;
}

Date Date::fromJulianDay(int julianDay) {
    int a = julianDay + 32044;
    int b = (4 * a + 3)/146097;
    int c = a - (146097 * b)/4;
    int d = (4 * c + 3)/1461;
    int e = c - (1461 * d)/4;
    int m = (5 * e + 2)/153;
    int day = e - (153 * m + 2)/5 + 1;
    int month = m + 3 - 12 * (m/10);
    int year = 100 * b + d - 4800 + m/10;
    return Date(year, month, day);
}

Date Date::operator+(int days) const {
    int julianDay = toJulianDay() + days;
    return fromJulianDay(julianDay);
}

Date Date::operator-(int days) const {
    int julianDay = toJulianDay() - days;
    return fromJulianDay(julianDay);
}

int Date::operator-(const Date& other) const {
    return toJulianDay() - other.toJulianDay();
}

bool Date::operator==(const Date& other) const {
    return year == other.year && month == other.month && day == other.day;
}

bool Date::operator!=(const Date& other) const {
    return !(*this == other);
}

bool Date::operator<(const Date& other) const {
    return toJulianDay() < other.toJulianDay();
}

bool Date::operator<=(const Date& other) const {
    return toJulianDay() <= other.toJulianDay();
}

bool Date::operator>(const Date& other) const {
    return toJulianDay() > other.toJulianDay();
}

bool Date::operator>=(const Date& other) const {
    return toJulianDay() >= other.toJulianDay();
}

std::ostream& operator<<(std::ostream& out, const Date& date) {
    out << date.toString();
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

// -------------------- DateTime 类实现 --------------------

DateTime::DateTime() : date(), time() {}

DateTime::DateTime(const Date& date, const Time& time) : date(date), time(time) {}

DateTime::DateTime(int year, int month, int day, int hours, int minutes, int seconds)
    : date(year, month, day), time(hours, minutes, seconds) {}

const Date& DateTime::getDate() const {
    return date;
}

void DateTime::setDate(const Date& date) {
    this->date = date;
}

const Time& DateTime::getTime() const {
    return time;
}

void DateTime::setTime(const Time& time) {
    this->time = time;
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

long long DateTime::toTotalSeconds() const {
    int daysSinceEpoch = date.toJulianDay() - Date(1970, 1, 1).toJulianDay();
    return static_cast<long long>(daysSinceEpoch) * 86400 + time.toTotalSeconds();
}

DateTime DateTime::fromTotalSeconds(long long totalSeconds) {
    int days = static_cast<int>(totalSeconds / 86400);
    long long secondsInDay = totalSeconds % 86400;
    if (secondsInDay < 0) {
        secondsInDay += 86400;
        days -= 1;
    }
    Date date = Date(1970, 1, 1) + days;
    Time time = Time::fromTotalSeconds(secondsInDay);
    return DateTime(date, time);
}

DateTime DateTime::operator+(const Time& duration) const {
    long long totalSeconds = toTotalSeconds() + duration.toTotalSeconds();
    return fromTotalSeconds(totalSeconds);
}

DateTime DateTime::operator-(const Time& duration) const {
    long long totalSeconds = toTotalSeconds() - duration.toTotalSeconds();
    return fromTotalSeconds(totalSeconds);
}

Time DateTime::operator-(const DateTime& other) const {
    long long secondsDiff = toTotalSeconds() - other.toTotalSeconds();
    return Time::fromTotalSeconds(secondsDiff);
}

bool DateTime::operator==(const DateTime& other) const {
    return toTotalSeconds() == other.toTotalSeconds();
}

bool DateTime::operator!=(const DateTime& other) const {
    return toTotalSeconds() != other.toTotalSeconds();
}

bool DateTime::operator<(const DateTime& other) const {
    return toTotalSeconds() < other.toTotalSeconds();
}

bool DateTime::operator<=(const DateTime& other) const {
    return toTotalSeconds() <= other.toTotalSeconds();
}

bool DateTime::operator>(const DateTime& other) const {
    return toTotalSeconds() > other.toTotalSeconds();
}

bool DateTime::operator>=(const DateTime& other) const {
    return toTotalSeconds() >= other.toTotalSeconds();
}

std::ostream& operator<<(std::ostream& out, const DateTime& dateTime) {
    out << dateTime.toString();
    return out;
}

std::istream& operator>>(std::istream& in, DateTime& dateTime) {
    Date date;
    Time time;
    char space;
    in >> date >> space >> time;
    if (!in || space != ' ') {
        in.setstate(std::ios::failbit);
        return in;
    }
    dateTime = DateTime(date, time);
    return in;
}