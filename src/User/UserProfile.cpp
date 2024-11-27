#include <User/UserProfile.h>
#include <file/fileManage.h>

UserProfile::UserProfile() : timeSlotPreferences(), airplaneModelPreferences(), airlinePreferences() {
    for (int i = 0;i < 4;++i) {
        timeSlotPreferences[i] = 0;
    }
}

void UserProfile::create() {
    order_map.traverse([&](const Order& order) {
        if (order.getStatus() == CANCELED || order.getStatus() == REFUNDED) {
            return;
        }
        update(order);
    });
}

void UserProfile::update(const Order &order, bool reverse) {
    if (reverse) {
        order.getOrderInfos().traverse([&](const OrderInfo& orderInfo) {
            int modelP = airplaneModelPreferences.get(orderInfo.getAirplaneModel()).value();
            int airlineP = airlinePreferences.get(orderInfo.getAirline()).value();
            timeSlotPreferences[static_cast<int>(orderInfo.getDepartureTimeSlot())] -= 1;
            airplaneModelPreferences.update(orderInfo.getAirplaneModel(), modelP-1);
            airlinePreferences.update(orderInfo.getAirline(), airlineP-1);
        });
    } else {
        order.getOrderInfos().traverse([&](const OrderInfo& orderInfo) {
            addAirline(orderInfo.getAirline());
            addAirplaneModel(orderInfo.getAirplaneModel());
            addTimeSlotPreference(orderInfo.getDepartureTimeSlot());
        });
    }
}

void UserProfile::addTimeSlotPreference(TimeSlot timeSlot) {
    timeSlotPreferences[static_cast<int>(timeSlot)] += 1;
}

void UserProfile::addAirplaneModel(const String &model) {
    if (airplaneModelPreferences.contains(model)) {
        int value = airplaneModelPreferences.get(model).value();
        airplaneModelPreferences.update(model, value+1);
    } else {
        airplaneModelPreferences.insert(model, 1);
    }
}

void UserProfile::addAirline(const String &airline) {
    if (airlinePreferences.contains(airline)) {
        int value = airlinePreferences.get(airline).value();
        airlinePreferences.update(airline, value+1);
    } else {
        airlinePreferences.insert(airline, 1);
    }
}

int UserProfile::getTimeSlotPreferences(TimeSlot timeslot) const {
    return timeSlotPreferences[timeslot];
}

int UserProfile::getAirplaneModelPreferences(const String& model) const {
    if (airplaneModelPreferences.contains(model)) {
        return airplaneModelPreferences.get(model).value();
    }
    return 0;
}

int UserProfile::getAirlinePreferences(const String& airline) const {
    if (airlinePreferences.contains(airline)) {
        return airlinePreferences.get(airline).value();
    }
    return 0;
}
