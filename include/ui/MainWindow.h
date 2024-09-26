#pragma once

#include <QMainWindow>
#include <QListWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QGraphicsScene>


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private slots:
    void changePage(int index);

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
    void createRouteMap();
};
