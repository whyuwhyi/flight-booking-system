#include <ui/server/ServerWindow.h>
#include <QListWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <ui/server/AirportManagementWindow.h>
#include <ui/server/AirlineManagementWindow.h>
#include <ui/server/AirplaneModelManagementWindow.h>
#include <data/datamanagement.h>

ServerWindow::ServerWindow(QWidget *parent) : QMainWindow(parent) {
    loadUserFromFile(user_map);
    loadAirportFromFile(airport_map);
    loadAirlineFromFile(airline_map);
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

    menuList->addItem(new QListWidgetItem(QIcon(":/icons/airport.svg"), "机场管理"));
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/airplane.svg"), "机型管理"));
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/flight.svg"), "航班管理"));
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/airline.svg"), "航线管理"));

    stackedWidget = new QStackedWidget(this);

    // 机场管理界面
    airportManagementWidget = new AirportManagementWindow(this); // 使用AirportManagementWindow类
    airlineManagementWidget = new AirlineManagementWindow(this);

    // 机型管理界面
    airplaneModelManagementWidget = new AirplaneModelManageWindow(this);
    QLabel *airplaneModelLabel = new QLabel("机型管理内容", airplaneModelManagementWidget);
    QVBoxLayout *airplaneModelLayout = new QVBoxLayout(airplaneModelManagementWidget);
    airplaneModelLayout->addWidget(airplaneModelLabel);

    // 航班管理界面
    flightManagementWidget = new QWidget(this);
    QLabel *flightLabel = new QLabel("航班管理内容", flightManagementWidget);
    QVBoxLayout *flightLayout = new QVBoxLayout(flightManagementWidget);
    flightLayout->addWidget(flightLabel);

    stackedWidget->addWidget(airportManagementWidget);
    stackedWidget->addWidget(airplaneModelManagementWidget);
    stackedWidget->addWidget(flightManagementWidget);
    stackedWidget->addWidget(airlineManagementWidget);

    layout->addWidget(menuList);
    layout->addWidget(stackedWidget);
}

void ServerWindow::setupConnections() {
    connect(menuList, &QListWidget::currentRowChanged, this, [this](int index) {
        stackedWidget->setCurrentIndex(index);
    });
}
