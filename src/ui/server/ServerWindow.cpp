#include <ui/server/ServerWindow.h>
#include <QListWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <ui/server/AirportManageWindow.h>
#include <ui/server/AirlineManageWindow.h>
#include <ui/server/AirplaneModelManageWindow.h>
#include <data/datamanage.h>

ServerWindow::ServerWindow(QWidget *parent) : QMainWindow(parent) {
    loadUserFromFile(user_map);
    loadAirportFromFile(airport_map);
    loadAirlineFromFile(airline_map);
    loadAirplaneModelFromFile(airplane_model_map);
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

    airportManageWidget = new AirportManageWindow(this); // 使用AirportManagementWindow类
    airlineManageWidget = new AirlineManageWindow(this);
    airplaneModelManageWidget = new AirplaneModelManageWindow(this);

    // 航班管理界面
    flightManagementWidget = new QWidget(this);
    QLabel *flightLabel = new QLabel("航班管理内容", flightManagementWidget);
    QVBoxLayout *flightLayout = new QVBoxLayout(flightManagementWidget);
    flightLayout->addWidget(flightLabel);

    stackedWidget->addWidget(airportManageWidget);
    stackedWidget->addWidget(airplaneModelManageWidget);
    stackedWidget->addWidget(flightManagementWidget);
    stackedWidget->addWidget(airlineManageWidget);

    layout->addWidget(menuList);
    layout->addWidget(stackedWidget);
}

void ServerWindow::setupConnections() {
    connect(menuList, &QListWidget::currentRowChanged, this, [this](int index) {
        stackedWidget->setCurrentIndex(index);
    });
}
