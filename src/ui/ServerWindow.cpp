#include <ui/ServerWindow.h>
#include <QListWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QAction>

ServerWindow::ServerWindow(QWidget *parent) : QMainWindow(parent) {
    setupUI();
    setupConnections();
}

ServerWindow::~ServerWindow() {}

void ServerWindow::setupUI() {
    setWindowTitle("服务器管理系统");
    setFixedSize(1200, 800);

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    menuList = new QListWidget();
    menuList->setViewMode(QListView::IconMode);
    menuList->setIconSize(QSize(48, 48));
    menuList->setFixedWidth(100);
    menuList->setSpacing(10);

    menuList->addItem(new QListWidgetItem(QIcon(":/icons/airport.svg"), "机场管理"));
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/flight.svg"), "航班管理"));

    stackedWidget = new QStackedWidget();

    airportManagementWidget = new QWidget();
    QLabel *airportLabel = new QLabel("机场管理界面", airportManagementWidget);
    QVBoxLayout *airportLayout = new QVBoxLayout(airportManagementWidget);
    airportLayout->addWidget(airportLabel);

    flightManagementWidget = new QWidget();
    QLabel *flightLabel = new QLabel("航班管理界面", flightManagementWidget);
    QVBoxLayout *flightLayout = new QVBoxLayout(flightManagementWidget);
    flightLayout->addWidget(flightLabel);

    stackedWidget->addWidget(airportManagementWidget);
    stackedWidget->addWidget(flightManagementWidget);

    QHBoxLayout *layout = new QHBoxLayout(centralWidget);
    layout->addWidget(menuList);
    layout->addWidget(stackedWidget);
}

void ServerWindow::setupConnections() {
    connect(menuList, &QListWidget::currentRowChanged, this, [this](int index) {
        stackedWidget->setCurrentIndex(index);
    });
}
