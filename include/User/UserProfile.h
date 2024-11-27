#pragma once

#include <String/String.h>
#include <Map/HashMap.h>
#include <FlightSystem/Order.h>
#include <FlightSystem/Flight.h>

class UserProfile {
private:
    int timeSlotPreferences[4];
    HashMap<String, int> airplaneModelPreferences;
    HashMap<String, int> airlinePreferences;

    void addTimeSlotPreference(TimeSlot timeSlot);
    void addAirplaneModel(const String &model);
    void addAirline(const String &airline);
public:
    explicit UserProfile();

    int getTimeSlotPreferences(TimeSlot timeSolt) const;
    int getAirplaneModelPreferences(const String &model) const;
    int getAirlinePreferences(const String &airline) const;
    void update(const Order &order, bool reverse = false);
    void create();
};