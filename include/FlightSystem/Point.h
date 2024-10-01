#pragma once

class Point {
public:
    Point(double longitude = 0.0, double latitude = 0.0);
    const double& getLongitude() const;
    const double& getLatitude() const;

    void setLongitude(double longitude);
    void setLatitude(double latitude);

private:
    double longitude;
    double latitude;
};
