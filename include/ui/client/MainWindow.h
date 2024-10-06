#pragma once

#include <QMainWindow>
#include <QListWidget>
#include <QStackedWidget>
#include <QWebEngineView>
#include <QWebChannel>

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

private:
    QWidget *centralWidget;
    QListWidget *menuList;
    QStackedWidget *stackedWidget;

    QWidget *routeMapWidget;
    QWidget *serviceHallWidget;
    QWidget *personalCenterWidget;
    QWebEngineView *mapView;

    QWebChannel *webChannel;   // QWebChannel 对象
    MapBackend *mapBackend;    // MapBackend 类对象

    void setupUI();
    void setupConnections();
    void createMapRoute();     // 创建地图航线
};
