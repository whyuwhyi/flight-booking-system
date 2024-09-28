#pragma once

#include <QMainWindow>

class QListWidget;
class QStackedWidget;
class QWebEngineView;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QWidget *centralWidget;
    QListWidget *menuList;
    QStackedWidget *stackedWidget;

    QWidget *routeMapWidget;
    QWidget *serviceHallWidget;
    QWidget *personalCenterWidget;
    QWebEngineView *mapView;

    void setupUI();
    void setupConnections();
    void createMapRoute();
};