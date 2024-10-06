#pragma once

#include <QMainWindow>

class QListWidget;
class QStackedWidget;
class AirportManageWindow;
class AirlineManageWindow;
class AirplaneModelManageWindow;
class FlightManageWindow;

class ServerWindow : public QMainWindow {
    Q_OBJECT

public:
    ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow();

private:
    QWidget *centralWidget;
    QListWidget *menuList;
    QStackedWidget *stackedWidget;

    AirplaneModelManageWindow *airplaneModelManageWidget;
    AirportManageWindow *airportManageWidget;
    AirlineManageWindow *airlineManageWidget;
    FlightManageWindow *flightManageWidget;

    void setupUI();
    void setupConnections();
};

