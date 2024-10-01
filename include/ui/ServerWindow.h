#pragma once

#include <QMainWindow>

class QListWidget;
class QStackedWidget;

class ServerWindow : public QMainWindow {
    Q_OBJECT

public:
    ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow();

private:
    QWidget *centralWidget;
    QListWidget *menuList;
    QStackedWidget *stackedWidget;

    QWidget *airportManagementWidget;
    QWidget *flightManagementWidget;
    QWidget *airplaneModelManagementWidget;

    void setupUI();
    void setupConnections();
};
