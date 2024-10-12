#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include <QListWidget>
#include <QWebEngineView>
#include <QWebChannel>
#include <ui/client/LoginWindow.h>
#include <ui/client/RegisterWindow.h>
#include <ui/client/TicketBookingWindow.h>

class MapBackend : public QObject {
    Q_OBJECT

public:
    MapBackend(QObject *parent = nullptr);

signals:
    void sendRoutesData(const QString &routesData);

public slots:
    void requestRoutesData();
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QStackedWidget *stackedWidget;
    QListWidget *menuList;
    QWebEngineView *mapView;
    QWebChannel *webChannel;
    MapBackend *mapBackend;
    LoginWindow *loginWindow;
    RegisterWindow *registerWindow;
    TicketBookingWindow *ticketBookingWindow;
    QWidget *routeMapWidget;
    QWidget *serviceHallWidget;
    QWidget *personalCenterWidget;

    void setupUI();
    void setupConnections();
    void showLoginWindow();
    void showRegisterWindow();
    void showMainContent();
};