#include <ui/server/ServerWindow.h>
#include <QListWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <ui/server/AirportManageWindow.h>
#include <ui/server/AirlineManageWindow.h>
#include <ui/server/AirplaneModelManageWindow.h>
#include <ui/server/FlightManageWindow.h>
#include <file/fileManage.h>

ServerWindow::ServerWindow(QWidget *parent) : QMainWindow(parent) {
    loadMapFromFile(airport_map, AIRPORTS_PATH.c_str());
    loadMapFromFile(airline_map, AIRLINES_PATH.c_str());
    loadMapFromFile(airplane_model_map, MODELS_PATH.c_str());
    loadMapFromFile(flight_map, FLIGHTS_PATH.c_str());

    setupUI();
    setupConnections();
}

ServerWindow::~ServerWindow() {}

void ServerWindow::setupUI() {
    setWindowTitle("服务器管理系统");
    setFixedSize(1200, 800);

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout *layout = new QHBoxLayout(centralWidget);
    layout->setContentsMargins(10, 10, 10, 10);

    menuList = new QListWidget();
    menuList->setViewMode(QListView::IconMode);
    menuList->setIconSize(QSize(48, 48));
    menuList->setFixedWidth(100);
    menuList->setSpacing(10);
    menuList->setDragEnabled(false);
    menuList->setDropIndicatorShown(false);
    menuList->setDragDropMode(QAbstractItemView::NoDragDrop);

    menuList->addItem(new QListWidgetItem(QIcon(":/icons/airplane.svg"), "机型管理"));
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/airport.svg"), "机场管理"));
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/airline.svg"), "航线管理"));
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/flight.svg"), "航班管理"));

    stackedWidget = new QStackedWidget(this);

    airportManageWidget = new AirportManageWindow(this);
    airlineManageWidget = new AirlineManageWindow(this);
    airplaneModelManageWidget = new AirplaneModelManageWindow(this);
    flightManageWidget = new FlightManageWindow(this);


    stackedWidget->addWidget(airplaneModelManageWidget);
    stackedWidget->addWidget(airportManageWidget);
    stackedWidget->addWidget(airlineManageWidget);
    stackedWidget->addWidget(flightManageWidget);

    layout->addWidget(menuList);
    layout->addWidget(stackedWidget);
}

void ServerWindow::setupConnections() {
    connect(menuList, &QListWidget::currentRowChanged, this, [this](int index) {
        stackedWidget->setCurrentIndex(index);
    });
}
