#include <ui/ServerWindow.h>
#include <QListWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

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

    stackedWidget = new QStackedWidget();

    // 机场管理界面
    airportManagementWidget = new QWidget();
    QLabel *airportLabel = new QLabel("机场管理内容", airportManagementWidget);
    QVBoxLayout *airportLayout = new QVBoxLayout(airportManagementWidget);
    airportLayout->addWidget(airportLabel);

    // 机型管理界面
    airplaneModelManagementWidget = new QWidget();
    QLabel *airplaneModelLabel = new QLabel("机型管理内容", airplaneModelManagementWidget);
    QVBoxLayout *airplaneModelLayout = new QVBoxLayout(airplaneModelManagementWidget);
    airplaneModelLayout->addWidget(airplaneModelLabel);

    // 航班管理界面
    flightManagementWidget = new QWidget();
    QLabel *flightLabel = new QLabel("航班管理内容", flightManagementWidget);
    QVBoxLayout *flightLayout = new QVBoxLayout(flightManagementWidget);
    flightLayout->addWidget(flightLabel);

    stackedWidget->addWidget(airportManagementWidget);
    stackedWidget->addWidget(airplaneModelManagementWidget);
    stackedWidget->addWidget(flightManagementWidget);

    layout->addWidget(menuList);
    layout->addWidget(stackedWidget);
}

void ServerWindow::setupConnections() {
    connect(menuList, &QListWidget::currentRowChanged, this, [this](int index) {
        stackedWidget->setCurrentIndex(index);
    });
}
