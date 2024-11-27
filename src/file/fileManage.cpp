#include <file/fileManage.h>
#include <filesystem>
#include <iostream>
#include <fstream>

const String DATA_DIR("/home/yuyi/cs-learnning/cpp-projects/curriculum-design/flight-booking-system/data/");
const String LOCAL_DATA_DIR("/home/yuyi/.flight-booking/");

const String AIRLINES_PATH = DATA_DIR + "airRoute/airRoutes.txt";
const String MODELS_PATH = DATA_DIR + "airplanemodel/models.txt";
const String AIRPORTS_PATH = DATA_DIR + "airport/airports.txt";
const String USERS_DIR = DATA_DIR + "user/";
const String USERS_PATH = USERS_DIR + "users.txt";
const String FLIGHTS_DIR = DATA_DIR + "flight/";
const String FLIGHTS_PATH = FLIGHTS_DIR + "flights.txt";
const String LOCAL_USER_PATH = LOCAL_DATA_DIR + "local-user.txt";

User current_login_user;
UserMap user_map([] (const User &user) { return user.getPhoneNumber(); });
AirportMap airport_map([] (const Airport &airport) { return airport.getName(); });
AirRouteMap air_route_map([] (const AirRoute &airline) { return airline.getName(); });
AirplaneModelMap airplane_model_map([] (const AirplaneModel &model) { return model.getName(); });
FlightMap flight_map([] (const Flight &flight) { return flight.getFlightName(); });
OrderMap order_map([] (const Order &order){ return order.getOrderNumber(); });
FlightNetwork flight_network(100);
UserProfile user_profile;

bool createDirectory(const char* directoryPath) {
    std::error_code ec;
    if (!std::filesystem::exists(directoryPath)) {
        return std::filesystem::create_directories(directoryPath, ec) && !ec;
    }
    return true;
}

bool removeDirectory(const char* directoryPath) {
    std::error_code ec;
    if (std::filesystem::exists(directoryPath)) {
        return std::filesystem::remove_all(directoryPath, ec) > 0 && !ec;
    }
    return false;
}

bool createFile(const char* fileName) {
    std::ofstream file(fileName, std::ios::out);
    bool success = file.is_open();
    file.close();
    return success;
}

bool removeFile(const char* fileName) {
    std::error_code ec;
    return std::filesystem::remove(fileName, ec) && !ec;
}

bool loadLocalUserFromFile(User &local_user, const char* fileName) {
    std::ifstream inFile(fileName, std::ios::in);

    if (!inFile.is_open()) {
        std::cout << "File open failed" << std::endl;
        return false;
    }

    inFile >> local_user;
    inFile.close();
    return true;
}

bool writeLocalUserToFile(User &local_user, const char* fileName) {
    std::ofstream outFile(fileName, std::ios::out);

    if (!outFile.is_open()) {
        std::cout << "File open failed" << std::endl;
        return false;
    }

    outFile << local_user;

    outFile.close();
    return true;
}

bool loadFlightNetworkFromFile() {
    if(loadMapFromFile(flight_map, FLIGHTS_PATH.c_str())) {
        flight_map.traverse([&](Flight *flight) {
            flight_network.addFlight(flight);
        });
        return true;
    }
    return false;
}

bool buyTicket(const Order& order) {
    int segmentCount = order.getOrderInfos().size();
    FlightTicketDetail* ticketDetails[segmentCount];
    int remainingTickets[segmentCount];
    PassengerMap passengerMaps[segmentCount]; // Updated to use PassengerMap

    for (int i = 0; i < segmentCount; ++i)
        passengerMaps[i].setGetKeyFunc([](const Passenger& passenger) { return passenger.getIdNumber(); });

    bool allAvailable = true;
    int index = 0;

    order.getOrderInfos().traverse([&](const OrderInfo& orderInfo) {
        if (!allAvailable) return;

        Flight* flight = flight_map.find(orderInfo.getFlightNumber());
        if (!flight) {
            allAvailable = false;
            return;
        }

        FlightTicketDetail* ticketDetail = flight->getFlightTicketDetail(orderInfo.getDate());
        if (!ticketDetail) {
            allAvailable = false;
            return;
        }

        int remainTickets = ticketDetail->getRemainingTickets(orderInfo.getCabinType());
        if (remainTickets == 0) {
            allAvailable = false;
            return;
        }

        String file1Name = FLIGHTS_DIR + orderInfo.getFlightNumber() + "/" + orderInfo.getDate().toString() + ".txt";
        loadMapFromFile(passengerMaps[index], file1Name.c_str());

        ticketDetails[index] = ticketDetail;
        remainingTickets[index] = remainTickets;
        index++;
    });

    if (!allAvailable)
        return false;

    bool passengerExists = false;
    index = 0;
    order.getOrderInfos().traverse([&](const OrderInfo& orderInfo) {
        const Passenger& passenger = orderInfo.getPassenger();
        if (passengerMaps[index].find(passenger.getIdNumber())) {
            passengerExists = true;
            return;
        }
        index++;
    });

    if (passengerExists)
        return false;

    index = 0;
    order.getOrderInfos().traverse([&](const OrderInfo& orderInfo) {
        ticketDetails[index]->setRemainingTickets(orderInfo.getCabinType(), remainingTickets[index] - 1);
        String file1Name = FLIGHTS_DIR + orderInfo.getFlightNumber() + "/" + orderInfo.getDate().toString() + ".txt";
        const Passenger& passenger = orderInfo.getPassenger();
        addElementToMap(passengerMaps[index], passenger, file1Name.c_str());
        index++;
    });

    if (!writeMapToFile(flight_map, FLIGHTS_PATH.c_str()))
        return false;

    String file2Name = USERS_DIR + current_login_user.getPhoneNumber() + "/tickets.txt";
    if (!addElementToMap(order_map, order, file2Name.c_str()))
        return false;

    user_profile.update(order);
    return true;
}


bool refundTicket(const Order& order) {
    Order newOrder(order);
    newOrder.setStatus(TicketStatus::REFUNDED);

    order.getOrderInfos().traverse([&](const OrderInfo& orderInfo) {
        String file1Name = FLIGHTS_DIR + orderInfo.getFlightNumber() + "/" + orderInfo.getDate().toString() + ".txt";

        Flight* flight = flight_map.find(orderInfo.getFlightNumber());
        if (!flight) {
            std::cerr << "Flight not found for " << orderInfo.getFlightNumber() << std::endl;
            return;
        }

        FlightTicketDetail* flightTicketDetail = flight->getFlightSchedule().find(orderInfo.getDate());
        if (!flightTicketDetail) {
            std::cerr << "Flight ticket detail not found for " << orderInfo.getFlightNumber() << std::endl;
            return;
        }

        flightTicketDetail->setRemainingTickets(orderInfo.getCabinType(), flightTicketDetail->getRemainingTickets(orderInfo.getCabinType()) + 1);

        if (!writeMapToFile(flight_map, FLIGHTS_PATH.c_str())) {
            std::cerr << "Update flight info failed for " << orderInfo.getFlightNumber() << std::endl;
            return;
        }

        PassengerMap passengerMap([](const Passenger& passenger) { return passenger.getIdNumber(); });
        if (!loadMapFromFile(passengerMap, file1Name.c_str())) {
            std::cerr << "Load passenger info failed for " << orderInfo.getFlightNumber() << std::endl;
            return;
        }

        const Passenger& passenger = orderInfo.getPassenger();
        if (!deleteElementInMap(passengerMap, passenger.getIdNumber(), file1Name.c_str())) {
            std::cerr << "Failed to modify passenger record" << std::endl;
            return;
        }
    });

    String file2Name = USERS_DIR + current_login_user.getPhoneNumber() + "/tickets.txt";
    if (!modifyElementInMap(order_map, newOrder, file2Name.c_str())) {
        std::cerr << "Failed to modify order record" << std::endl;
        return false;
    }

    user_profile.update(order, true);
    return true;
}


bool changeTicket(const Order& order, const Order& newOrder) {
    return refundTicket(order) && buyTicket(newOrder);
}


bool buyMeal(const Order& order, int segmentIndex, enum Meal meal) {
    if (segmentIndex < 0 || segmentIndex >= order.getOrderInfos().size()) {
        std::cerr << "Invalid segment index" << std::endl;
        return false;
    }

    Order newOrder(order);
    newOrder.setMeal(segmentIndex, meal);
    OrderInfo orderInfo = newOrder.getOrderInfos().getElementAt(segmentIndex);
    String file1Name = FLIGHTS_DIR + orderInfo.getFlightNumber() + "/" + orderInfo.getDate().toString() + ".txt";
    String file2Name = USERS_DIR + current_login_user.getPhoneNumber() + "/tickets.txt";

    PassengerMap passengerMap([](const Passenger& passenger) { return passenger.getIdNumber(); });

    if (!loadMapFromFile(passengerMap, file1Name.c_str())) {
        std::cerr << "Load passenger info failed for " << orderInfo.getFlightNumber() << std::endl;
        return false;
    }

    const Passenger& passenger = orderInfo.getPassenger();
    if (!modifyElementInMap(passengerMap, passenger, file1Name.c_str())) {
        std::cerr << "Failed to modify passenger record" << std::endl;
        return false;
    }

    if (!modifyElementInMap(order_map, newOrder, file2Name.c_str())) {
        std::cerr << "Failed to modify order record" << std::endl;
        return false;
    }

    return true;
}

bool chooseSeat(const Order& order, int segmentIndex, const String& seatNum) {
    if (segmentIndex < 0 || segmentIndex >= order.getOrderInfos().size()) {
        std::cerr << "Invalid segment index" << std::endl;
        return false;
    }

    Order newOrder(order);
    newOrder.setSeatNum(segmentIndex, seatNum);
    OrderInfo orderInfo = newOrder.getOrderInfos().getElementAt(segmentIndex);
    String file1Name = FLIGHTS_DIR + orderInfo.getFlightNumber() + "/" + orderInfo.getDate().toString() + ".txt";
    String file2Name = USERS_DIR + current_login_user.getPhoneNumber() + "/tickets.txt";

    PassengerMap passengerMap([](const Passenger& passenger) { return passenger.getIdNumber(); });

    if (!loadMapFromFile(passengerMap, file1Name.c_str())) {
        std::cerr << "Load passenger info failed for " << orderInfo.getFlightNumber() << std::endl;
        return false;
    }

    const Passenger& passenger = orderInfo.getPassenger();
    if (!modifyElementInMap(passengerMap, passenger, file1Name.c_str())) {
        std::cerr << "Failed to modify passenger record" << std::endl;
        return false;
    }

    if (!modifyElementInMap(order_map, newOrder, file2Name.c_str())) {
        std::cerr << "Failed to modify order record" << std::endl;
        return false;
    }

    return true;
}
