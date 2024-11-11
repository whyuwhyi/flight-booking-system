#include <file/fileManage.h>
#include <filesystem>
#include <iostream>
#include <fstream>

const String DATA_DIR("/home/yuyi/cs-learnning/cpp-projects/curriculum-design/flight-booking-system/data/");
const String LOCAL_DATA_DIR("/home/yuyi/.flight-booking/");

const String AIRLINES_PATH = DATA_DIR + "airline/airlines.txt";
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
AirlineMap airline_map([] (const Airline &airline) { return airline.getName(); });
AirplaneModelMap airplane_model_map([] (const AirplaneModel &model) { return model.getName(); });
FlightMap flight_map([] (const Flight &flight) { return flight.getFlightName(); });
OrderMap order_map([] (const Order &order)
{ return order.getOrderNumber(); });
FlightNetwork flight_network(100);

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
    Flight* flight = flight_map.find(order.getFlightNumber());
    FlightTicketDetail* ticketDetail = flight->getFlightTicketDetail(order.getDate());
    int remainTickets = ticketDetail->getRemainingTickets(order.getCabinType());
    if (remainTickets == 0) {
        std::cout << "No more ticket to book!" << std::endl;
        return false;
    }

    Map<String, Order> orderMap([](const Order& order) { return order.getFlightNumber() + order.getPassenger().getIdNumber(); });
    String file1Name = FLIGHTS_DIR + order.getFlightNumber() + "/" +
                       ticketDetail->getFlightDate().toString() + ".txt";
    String file2Name = USERS_DIR + current_login_user.getPhoneNumber() + "/tickets.txt";
    Order *exist_order = order_map.find(order.getOrderNumber());

    if (!loadMapFromFile(orderMap, file1Name.c_str())) {
        std::cerr << "Load ticket info failed!" << std::endl;
        return false;
    }
    
    ticketDetail->setRemainingTickets(order.getCabinType(), remainTickets-1);

    if(!writeMapToFile(flight_map, FLIGHTS_PATH.c_str())) {
        std::cerr << "Update flight info failed!" << std::endl;
        return false;
    }

    if (exist_order != nullptr && exist_order->getStatus() == REFUNDED) {
        return addElementToMap(orderMap, order, file1Name.c_str()) && modifyElementInMap(order_map, order, file2Name.c_str());
    }

    return addElementToMap(orderMap, order, file1Name.c_str()) && addElementToMap(order_map, order, file2Name.c_str());
}

bool refundTicket(const Order& order) {
    Order newOrder(order);
    Map<String, Order> orderMap([](const Order& order) { return order.getOrderNumber(); });
    String file1Name = FLIGHTS_DIR + order.getFlightNumber() + "/" +
                       order.getDate().toString() + ".txt";
    String file2Name = USERS_DIR + current_login_user.getPhoneNumber() + "/tickets.txt";

    if (!loadMapFromFile(orderMap, file1Name.c_str())) {
        std::cerr << "Load ticket info failed!" << std::endl;
        return false;
    }

    Flight* flight = flight_map.find(order.getFlightNumber());

    if (flight == nullptr) {
        std::cerr << "Flight not found!" << std::endl;
        return false;
    }

    FlightTicketDetail* flightTicketDetail = flight->getFlightSchedule().find(order.getDate());

    if (flightTicketDetail == nullptr) {
        std::cerr << "Flight ticket detail not found!" << std::endl;
        return false;
    }

    flightTicketDetail->setRemainingTickets(order.getCabinType(), flightTicketDetail->getRemainingTickets(order.getCabinType()) - 1);

    if(!writeMapToFile(flight_map, FLIGHTS_PATH.c_str())) {
        std::cerr << "Update flight info failed!" << std::endl;
        return false;
    }
    
    newOrder.setStatus(TicketStatus::REFUNDED);
    
    return deleteElementInMap(orderMap, order.getOrderNumber() , file1Name.c_str()) && 
           modifyElementInMap(order_map, newOrder , file2Name.c_str());

}

bool changeTicket(const Order& order, const Date& newDate) {
    Order newOrder(order);
    newOrder.setDate(newDate);
    return refundTicket(order) && buyTicket(newOrder);
}

bool buyMeal(const Order& order, enum Meal meal) {
    Order newOrder(order);
    newOrder.setMeal(meal);
    Map<String, Order> orderMap([](const Order& order) { return order.getOrderNumber(); });
    String file1Name = FLIGHTS_DIR + order.getFlightNumber() + "/" +
                       order.getDate().toString() + ".txt";
    String file2Name = USERS_DIR + current_login_user.getPhoneNumber() + "/tickets.txt";

    if (!loadMapFromFile(orderMap, file1Name.c_str())) {
        std::cerr << "Load ticket info failed!" << std::endl;
        return false;
    }

    return modifyElementInMap(orderMap, newOrder, file1Name.c_str()) && 
           modifyElementInMap(order_map, newOrder, file2Name.c_str());
}

bool chooseSeat(const Order& order, const String& seatNum) {
    Order newOrder(order);
    newOrder.setSeatNum(seatNum);
    Map<String, Order> orderMap([](const Order& order) { return order.getOrderNumber(); });
    String file1Name = FLIGHTS_DIR + order.getFlightNumber() + "/" +
                       order.getDate().toString() + ".txt";
    String file2Name = USERS_DIR + current_login_user.getPhoneNumber() + "/tickets.txt";

    if (!loadMapFromFile(orderMap, file1Name.c_str())) {
        std::cerr << "Load ticket info failed!" << std::endl;
        return false;
    }

    return modifyElementInMap(orderMap, newOrder, file1Name.c_str()) && 
           modifyElementInMap(order_map, newOrder, file2Name.c_str());
}