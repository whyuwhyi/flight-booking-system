#pragma once

#include <QMainWindow>

class QListWidget;
class QStackedWidget;
class AirportManagementWindow;
class AirlineManagementWindow;
class AirplaneModelManageWindow;

class ServerWindow : public QMainWindow {
    Q_OBJECT

public:
    ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow();

private:
    QWidget *centralWidget;
    QListWidget *menuList;
    QStackedWidget *stackedWidget;

    AirportManagementWindow *airportManagementWidget;
    AirlineManagementWindow *airlineManagementWidget;
    QWidget *flightManagementWidget;
    AirplaneModelManageWindow *airplaneModelManagementWidget;

    void setupUI();
    void setupConnections();
};

