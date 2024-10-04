#pragma once

#include <QMainWindow>

class QListWidget;
class QStackedWidget;
class AirportManagementWindow;

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
    QWidget *flightManagementWidget;
    QWidget *airplaneModelManagementWidget;

    void setupUI();
    void setupConnections();
};

