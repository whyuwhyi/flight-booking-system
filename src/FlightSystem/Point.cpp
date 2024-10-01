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
