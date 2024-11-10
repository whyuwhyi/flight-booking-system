#pragma

#include <functional>
#include <FlightSystem/Ticket.h>

enum SortRule {
    NO_RULE,
    EARLIEST_DEPARTURE,
    EARLIEST_ARRIVAL,
    SHORTEST_DURATION,
    CHEAPEST_PRICE
};

template <typename T>
std::function<bool(const T&, const T&)> getSortFunction(SortRule rule) {
    switch (rule) {
        case EARLIEST_DEPARTURE:
            return [](const T& a, const T& b) {
                return a.getDepartureDateTime() < b.getDepartureDateTime();
            };
        case EARLIEST_ARRIVAL:
            return [](const T& a, const T& b) {
                return a.getArrivalDateTime() < b.getArrivalDateTime();
            };
        case SHORTEST_DURATION:
            return [](const T& a, const T& b) {
                return a.getDuration() < b.getDuration();
            };
        case CHEAPEST_PRICE:
            return [](const T& a, const T& b) {
                return a.getPrice() < b.getPrice();
            };
        default:
            return [](const T& a, const T& b) {
                return false;
            };
    }
}