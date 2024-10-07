#pragma once

#include <QMainWindow>
#include <QListWidget>
#include <QStackedWidget>
#include <QWebEngineView>
#include <QWebChannel>
#include <ui/client/LoginWindow.h>
#include <ui/client/RegisterWindow.h>

class MapBackend : public QObject {
    Q_OBJECT
public:
    explicit MapBackend(QObject *parent = nullptr);

    Q_INVOKABLE void requestRoutesData();

signals:
    void sendRoutesData(const QString &routesData);
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showLoginWindow();
    void showRegisterWindow();
    void showMainContent();

private:
    void setupUI();
    void setupConnections();

    QListWidget *menuList;
    QStackedWidget *stackedWidget;

    LoginWindow *loginWindow;
    RegisterWindow *registerWindow;

    QWidget *routeMapWidget;
    QWidget *serviceHallWidget;
    QWidget *personalCenterWidget;
    QWebEngineView *mapView;
    QWebChannel *webChannel;
    MapBackend *mapBackend;
};