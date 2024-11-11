#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include <QListWidget>
#include <QWebEngineView>
#include <QWebChannel>
#include <ui/client/LoginWindow.h>
#include <ui/client/RegisterWindow.h>
#include <ui/client/TicketBookingWindow.h>
#include <ui/client/PersonalCenterWindow.h>

class MapBackend : public QObject {
    Q_OBJECT

public:
    MapBackend(QObject *parent = nullptr);

signals:
    void sendRoutesData(const QString &routesData);
    void sendUserOrderInfo(const QString &orderInfo);

public slots:
    void requestRoutesData();
    void requestUserOrderInfo();
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
    PersonalCenterWindow *personalCenterWindow;
    QWidget *routeMapWidget;

    void setupUI();
    void setupConnections();
    void showLoginWindow();
    void showRegisterWindow();
    void showMainContent();
};