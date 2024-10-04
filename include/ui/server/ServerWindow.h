#pragma once

#include <QMainWindow>

class QListWidget;
class QStackedWidget;
class AirportManageWindow;
class AirlineManageWindow;
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

    AirportManageWindow *airportManageWidget;
    AirlineManageWindow *airlineManageWidget;
    QWidget *flightManagementWidget;
    AirplaneModelManageWindow *airplaneModelManageWidget;

    void setupUI();
    void setupConnections();
};

