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
TicketMap ticket_map([] (const TicketInfo &ticketInfo) { return ticketInfo.getFlightNumber() + ticketInfo.getPassenger().getIdNumber();});
FlightNetwork flight_network(10);

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

bool buyTicket(const Ticket& ticket, const CabinType& cabin, const String& user, const Passenger& passenger) {
    int remainTickets = ticket.getFlightTicketDetail()->getRemainingTickets(cabin);
    if (remainTickets == 0) {
        std::cout << "No more ticket to book!" << std::endl;
        return false;
    }

    Map<String, TicketInfo> ticketMap([](const TicketInfo& ticketInfo) { return ticketInfo.getPassenger().getIdNumber(); });
    TicketInfo ticketInfo(ticket.getFlight()->getFlightName(), user, passenger, cabin, ticket.getFlightTicketDetail()->getCabinPrice(cabin),
                          ticket.getFlightTicketDetail()->getFlightDate(), nullptr);
    String file1Name = FLIGHTS_DIR + ticket.getFlight()->getFlightName() + "/" +
                       ticket.getFlightTicketDetail()->getFlightDate().toString() + ".txt";
    String file2Name = USERS_DIR + user + "/tickets.txt";

    if (!loadMapFromFile(ticketMap, file1Name.c_str())) {
        std::cerr << "Load ticket info failed!" << std::endl;
        return false;
    }
    
    ticket.getFlightTicketDetail()->setRemainingTickets(cabin, remainTickets-1);

    if(!writeMapToFile(flight_map, FLIGHTS_PATH.c_str())) {
        std::cerr << "Update flight info failed!" << std::endl;
        return false;
    }

    return addElementToMap(ticketMap, ticketInfo, file1Name.c_str()) && addElementToMap(ticket_map, ticketInfo, file2Name.c_str());
}

