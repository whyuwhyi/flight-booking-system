#include <FlightSystem/FlightNetwork.h>
#include <queue>
#include <file/fileManage.h>
#include <iostream>

FlightNetwork::FlightNetwork(int maxCityCount) {
    cityIndexMap = HashMap<String, CityInfo>(maxCityCount * 1.5);
    internationalFlight = new LinkedList<Flight*>*[maxCityCount];
    domesticFlight = new LinkedList<Flight*>*[maxCityCount];
    for (int i = 0; i < maxCityCount; ++i) {
        internationalFlight[i] = new LinkedList<Flight*>[maxCityCount];
        domesticFlight[i] = new LinkedList<Flight*>[maxCityCount];
    }
}

FlightNetwork::~FlightNetwork() {
    for (int i = 0; i < cityIndexMap.getSize(); ++i) {
        delete[] internationalFlight[i];
        delete[] domesticFlight[i];
    }
    delete[] internationalFlight;
    delete[] domesticFlight;
}

void FlightNetwork::addCity(const Airport& airport) {
    cityIndexMap.insert(airport.getCity(), {!(airport.getCountry() == "中国") ,(int)cityIndexMap.getSize()});
}

bool FlightNetwork::cityExists(const String& city) const {
    return cityIndexMap.contains(city);
}

int FlightNetwork::getCityIndex(const String& city) const {
    auto cityIndexOpt = cityIndexMap.get(city);
    if (!cityIndexOpt.has_value()) {
        throw std::invalid_argument("City not found: ");
    }
    return cityIndexOpt.value().index;
}

CityInfo FlightNetwork::getCityInfo(const String& city) const {
    auto cityIndexOpt = cityIndexMap.get(city);
    if (!cityIndexOpt.has_value()) {
        throw std::invalid_argument("City not found: ");
    }
    return cityIndexOpt.value();
}

void FlightNetwork::addFlight(Flight* flight) {
    if (!cityExists(flight->getDepartureAirport().getCity())) {
        addCity(flight->getDepartureAirport());
    }
    if (!cityExists(flight->getArrivalAirport().getCity())) {
        addCity(flight->getArrivalAirport());
    }

    CityInfo departureCityInfo = getCityInfo(flight->getDepartureAirport().getCity());
    CityInfo arrivalCityInfo = getCityInfo(flight->getArrivalAirport().getCity());

    internationalFlight[departureCityInfo.index][arrivalCityInfo.index].append(flight);

    if (!departureCityInfo.isAbroad && !arrivalCityInfo.isAbroad) {
        domesticFlight[departureCityInfo.index][arrivalCityInfo.index].append(flight);
    }

}

LinkedList<ConnectingTicket> FlightNetwork::findConnectingFlights(LinkedList<Flight*>** flightNetwork, const String& departureCity, const String& arrivalCity, const Date& date, int maxStops) const {
    LinkedList<ConnectingTicket> connectingFlights;
    int departureIndex = getCityIndex(departureCity);
    int arrivalIndex = getCityIndex(arrivalCity);

    struct FlightPath {
        LinkedList<Ticket> tickets;
        int currentCityIndex;
        int stops;
        DateTime lastArrivalTime;
        Map<int, int> visitedCities;
    };

    LinkedList<FlightPath> stack;

    for (int i = 0; i < cityIndexMap.getSize(); ++i) {
        flightNetwork[departureIndex][i].traverse([&](Flight* flight) {
            FlightTicketDetail* flightDetail = flight->getFlightSchedule().find(date);
            if (flightDetail) {
                Ticket ticket(flight, flightDetail);
                LinkedList<Ticket> tickets;
                tickets.append(ticket);
                DateTime arrivalTime = ticket.getArrivalDateTime();

                Map<int, int> visitedCities([](const int& value) { return value; });
                visitedCities.insert(departureIndex);
                int arrivalCityIndex = getCityIndex(flight->getArrivalAirport().getCity());
                visitedCities.insert(arrivalCityIndex);

                stack.append({tickets, arrivalCityIndex, 0, arrivalTime, visitedCities});
            }
        });
    }

    while (stack.size() > 0) {
        FlightPath currentPath = stack.removeLast();
        int currentCityIndex = currentPath.currentCityIndex;
        DateTime lastArrivalTime = currentPath.lastArrivalTime;

        if (currentCityIndex == arrivalIndex) {
            ConnectingTicket connectingTicket;
            currentPath.tickets.traverse([&](const Ticket& ticket) {
                connectingTicket.addTicket(ticket);
            });
            connectingFlights.append(connectingTicket);
            continue;
        }

        if (currentPath.stops >= maxStops) {
            continue;
        }

        for (int i = 0; i < cityIndexMap.getSize(); ++i) {
            flightNetwork[currentCityIndex][i].traverse([&](Flight* nextFlight) {
                int nextCityIndex = getCityIndex(nextFlight->getArrivalAirport().getCity());

                if (currentPath.visitedCities.find(nextCityIndex)) {
                    return;
                }

                bool validTicketFound = false;
                DateTime nextDepartureTime = lastArrivalTime + Time(1, 0, 0); 
                Date sameDayDate = nextDepartureTime.getDate();
                Date nextDayDate = sameDayDate + 1;

                FlightTicketDetail* sameDayFlightDetail = nextFlight->getFlightSchedule().find(sameDayDate);
                if (sameDayFlightDetail) {
                    DateTime sameDayArrivalTime = DateTime(sameDayDate, nextFlight->getDepartureTime()) + nextFlight->getCostTime();
                    if (nextFlight->getDepartureTime() >= nextDepartureTime.getTime()) {
                        Ticket nextTicket(nextFlight, sameDayFlightDetail);
                        LinkedList<Ticket> newTickets = currentPath.tickets;
                        newTickets.append(nextTicket);

                        Map<int, int> newVisitedCities = currentPath.visitedCities;
                        newVisitedCities.insert(nextCityIndex);

                        stack.append({newTickets, nextCityIndex, currentPath.stops + 1, nextTicket.getArrivalDateTime(), newVisitedCities});
                        validTicketFound = true;
                    }
                }

                if (!validTicketFound) {
                    FlightTicketDetail* nextDayFlightDetail = nextFlight->getFlightSchedule().find(nextDayDate);
                    if (nextDayFlightDetail) {
                        DateTime nextDayArrivalTime = DateTime(nextDayDate, nextFlight->getDepartureTime()) + nextFlight->getCostTime();
                        if (nextDayArrivalTime - Time(24, 0, 0) < lastArrivalTime) {
                            Ticket nextTicket(nextFlight, nextDayFlightDetail);
                            LinkedList<Ticket> newTickets = currentPath.tickets;
                            newTickets.append(nextTicket);

                            Map<int, int> newVisitedCities = currentPath.visitedCities;
                            newVisitedCities.insert(nextCityIndex);

                            stack.append({newTickets, nextCityIndex, currentPath.stops + 1, nextTicket.getArrivalDateTime(), newVisitedCities});
                        }
                    }
                }
            });
        }
    }

    return std::move(connectingFlights);
}

LinkedList<ConnectingTicket> FlightNetwork::findConnectingFlights(const String& departureCity, const String& arrivalCity, const Date& date, int maxStops, bool userpre) const {
    if (!cityExists(departureCity) || !cityExists(arrivalCity)) {
        return LinkedList<ConnectingTicket>();
    }

    CityInfo departureCityInfo = getCityInfo(departureCity);
    CityInfo arrivalCityInfo = getCityInfo(arrivalCity);

    if (userpre) {
        if (!departureCityInfo.isAbroad && !arrivalCityInfo.isAbroad) {
        return findUserPreferredFlights(domesticFlight, departureCity, arrivalCity, date);
        }
        return findUserPreferredFlights(internationalFlight, departureCity, arrivalCity, date);
    }

    if (!departureCityInfo.isAbroad && !arrivalCityInfo.isAbroad) {
        return findConnectingFlights(domesticFlight, departureCity, arrivalCity, date, maxStops);
    }
    return findConnectingFlights(internationalFlight, departureCity, arrivalCity, date, maxStops);
}

LinkedList<ConnectingTicket> FlightNetwork::findUserPreferredFlights(LinkedList<Flight*>** flightNetwork, const String& departureCity, const String& arrivalCity, const Date& date) const {
    LinkedList<ConnectingTicket> preferredFlights;
    int departureIndex = getCityIndex(departureCity);
    int arrivalIndex = getCityIndex(arrivalCity);

    struct FlightPath {
        LinkedList<Ticket> tickets;
        int currentCityIndex;
        DateTime lastArrivalTime;
        Map<int, int> visitedCities; // 存储已访问的城市索引
        double preferenceScore;
    };

    struct CityPreference {
        int cityIndex;
        double preferenceScore;
    };
    

    // 使用优先队列，根据偏好分数从高到低排序
    auto compareFlightPaths = [](const FlightPath& a, const FlightPath& b) {
        return a.preferenceScore < b.preferenceScore; // 分数高的优先
    };
    std::priority_queue<FlightPath, std::vector<FlightPath>, decltype(compareFlightPaths)> queue(compareFlightPaths);

    // 初始化队列，添加从出发城市出发的航班
    for (int i = 0; i < cityIndexMap.getSize(); ++i) {
        flightNetwork[departureIndex][i].traverse([&](Flight* flight) {
            FlightTicketDetail* flightDetail = flight->getFlightSchedule().find(date);
            if (flightDetail) {
                Ticket ticket(flight, flightDetail);
                LinkedList<Ticket> tickets;
                tickets.append(ticket);
                DateTime arrivalTime = ticket.getArrivalDateTime();

                // 初始化 visitedCities，使用 getKey 函数为 identity
                Map<int, int> visitedCities(
                    [](const int& cityIndex) { return cityIndex; },
                    std::less<int>()
                );
                visitedCities.insert(departureIndex);
                int arrivalCityIndex = getCityIndex(flight->getArrivalAirport().getCity());
                visitedCities.insert(arrivalCityIndex);

                double preferenceScore = calculatePreferenceScore(ticket);

                queue.push({std::move(tickets), arrivalCityIndex, arrivalTime, std::move(visitedCities), preferenceScore});
            }
        });
    }

    // 记录每个城市的最佳偏好分数，避免处理次优路径
    Map<int, CityPreference> bestPreferenceScores(
        [](const CityPreference& cp) { return cp.cityIndex; },
        std::less<int>()
    );

    while (!queue.empty()) {
        FlightPath currentPath = queue.top();
        queue.pop();
        int currentCityIndex = currentPath.currentCityIndex;
        DateTime lastArrivalTime = currentPath.lastArrivalTime;

        // 如果到达目的城市，将航班添加到结果列表并退出循环
        if (currentCityIndex == arrivalIndex) {
            ConnectingTicket connectingTicket;
            currentPath.tickets.traverse([&](const Ticket& ticket) {
                connectingTicket.addTicket(ticket);
            });
            preferredFlights.append(std::move(connectingTicket));
            break; // 只需要找到一个最优的航班，退出循环
        }

        // 处理下一段航班
        for (int i = 0; i < cityIndexMap.getSize(); ++i) {
            flightNetwork[currentCityIndex][i].traverse([&](Flight* nextFlight) {
                int nextCityIndex = getCityIndex(nextFlight->getArrivalAirport().getCity());

                if (currentPath.visitedCities.find(nextCityIndex)) {
                    return; // 避免循环
                }

                DateTime nextDepartureTime = lastArrivalTime + Time(1, 0, 0); // 最小中转时间为1小时
                Date sameDayDate = nextDepartureTime.getDate();
                Date nextDayDate = sameDayDate + 1;

                // 检查当天的航班
                FlightTicketDetail* sameDayFlightDetail = nextFlight->getFlightSchedule().find(sameDayDate);
                if (sameDayFlightDetail && nextFlight->getDepartureTime() >= nextDepartureTime.getTime()) {
                    Ticket nextTicket(nextFlight, sameDayFlightDetail);
                    LinkedList<Ticket> newTickets = currentPath.tickets;
                    newTickets.append(nextTicket);

                    Map<int, int> newVisitedCities = currentPath.visitedCities;
                    newVisitedCities.insert(nextCityIndex);

                    DateTime arrivalTime = nextTicket.getArrivalDateTime();
                    double preferenceScore = currentPath.preferenceScore + calculatePreferenceScore(nextTicket);

                    CityPreference* existingCP = bestPreferenceScores.find(nextCityIndex);
                    if (!existingCP || preferenceScore > existingCP->preferenceScore) {
                        // 更新 bestPreferenceScores
                        CityPreference newCP{nextCityIndex, preferenceScore};
                        if (existingCP) {
                            bestPreferenceScores.erase(nextCityIndex);
                        }
                        bestPreferenceScores.insert(newCP);

                        queue.push({std::move(newTickets), nextCityIndex, arrivalTime, std::move(newVisitedCities), preferenceScore});
                    }
                }

                // 检查次日的航班
                FlightTicketDetail* nextDayFlightDetail = nextFlight->getFlightSchedule().find(nextDayDate);
                if (nextDayFlightDetail && nextFlight->getDepartureTime() >= nextDepartureTime.getTime()) {
                    Ticket nextTicket(nextFlight, nextDayFlightDetail);
                    LinkedList<Ticket> newTickets = currentPath.tickets;
                    newTickets.append(nextTicket);

                    Map<int, int> newVisitedCities = currentPath.visitedCities;
                    newVisitedCities.insert(nextCityIndex);

                    DateTime arrivalTime = nextTicket.getArrivalDateTime();
                    double preferenceScore = currentPath.preferenceScore + calculatePreferenceScore(nextTicket);

                    CityPreference* existingCP = bestPreferenceScores.find(nextCityIndex);
                    if (!existingCP || preferenceScore > existingCP->preferenceScore) {
                        // 更新 bestPreferenceScores
                        CityPreference newCP{nextCityIndex, preferenceScore};
                        if (existingCP) {
                            bestPreferenceScores.erase(nextCityIndex);
                        }
                        bestPreferenceScores.insert(newCP);

                        queue.push({std::move(newTickets), nextCityIndex, arrivalTime, std::move(newVisitedCities), preferenceScore});
                    }
                }
            });
        }
    }

    // 返回结果列表，使用移动赋值
    return std::move(preferredFlights);
}



double FlightNetwork::calculatePreferenceScore(const Ticket& ticket) const {
    double score = 0.0;

    // 时间段偏好
    TimeSlot timeSlot = ticket.getFlight()->getDepartureTime().getTimeSlot();
    int timeSlotPreference = user_profile.getTimeSlotPreferences(timeSlot);
    score += timeSlotPreference * 1.0; // 权重可调整

    // 机型偏好
    String airplaneModel = ticket.getFlight()->getAirplaneModel();
    int airplaneModelPreference = user_profile.getAirplaneModelPreferences(airplaneModel);
    score += airplaneModelPreference * 0.8; // 权重可调整

    // 航空公司偏好
    String airline = ticket.getFlight()->getAirline();
    int airlinePreference = user_profile.getAirlinePreferences(airline);
    score += airlinePreference * 0.5; // 权重可调整

    return score;
}

void FlightNetwork::traverseCities(std::function<void(const String&, const CityInfo&)> func) const {
    cityIndexMap.traverse([&](const String& city, CityInfo cityInfo) {
        func(city, cityInfo);
    });
}