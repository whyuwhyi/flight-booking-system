#pragma once

#include <iostream>

class Point {
private:
    double longitude;
    double latitude;
public:
    Point(double longitude = 0.0, double latitude = 0.0);
    const double& getLongitude() const;
    const double& getLatitude() const;

    void setLongitude(double longitude);
    void setLatitude(double latitude);

    bool operator==(const Point& other) const;

    friend std::ostream& operator<<(std::ostream& out, const Point& point);
    friend std::istream& operator>>(std::istream& in, Point& point);
};
