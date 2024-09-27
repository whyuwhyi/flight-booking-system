#pragma once

#include <QMainWindow>


class QListWidget;
class QStackedWidget;
class QGraphicsView;
class QGraphicsScene;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QWidget *centralWidget;
    QListWidget *menuList;
    QStackedWidget *stackedWidget;

    // 这里可以添加各个页面的指针
    QWidget *routeMapWidget;
    QWidget *serviceHallWidget;
    QWidget *personalCenterWidget;
    QGraphicsView *mapView;  // 地图视图
    QGraphicsScene *mapScene; // 地图场景

    void setupUI();
    void setupConnections();
    void createRouteMap();
};
