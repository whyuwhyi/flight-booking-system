#include <FlightSystem/Point.h>

Point::Point(double longitude, double latitude) : longitude(longitude), latitude(latitude) {}

const double& Point::getLongitude() const {
    return longitude;
}

const double& Point::getLatitude() const {
    return latitude;
}

void Point::setLongitude(double longitude) {
    this->longitude = longitude;
}

void Point::setLatitude(double latitude) {
    this->latitude = latitude;
}

bool Point::operator==(const Point& other) const {
    return (longitude == other.longitude) && (latitude == other.latitude);
}

std::ostream& operator<<(std::ostream& out, const Point& point) {
    out << point.longitude << ' ' << point.latitude;
    return out;
}

std::istream& operator>>(std::istream& in, Point& point) {
    in >> point.longitude >> point.latitude;
    return in;
}