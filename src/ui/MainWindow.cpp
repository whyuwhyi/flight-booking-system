#include <ui/MainWindow.h>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QIcon>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGeoServiceProvider>
#include <QGeoRoutingManager>
#include <QGeoRoute>
#include <QPlace>
#include <QPlaceManager>



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUI();
    setupConnections();
}

MainWindow::~MainWindow() {}

void MainWindow::createRouteMap() {
    // 创建地图视图
    mapView = new QGraphicsView(this);
    mapView->setRenderHint(QPainter::Antialiasing);

    // 创建场景
    mapScene = new QGraphicsScene(this);
    mapView->setScene(mapScene);

    // 使用 QGeoServiceProvider 创建地图服务提供者
    QGeoServiceProvider serviceProvider("osm"); // 这里指定地图服务提供者，例如 "osm"

    // 创建 QGeoCodingManager 用于地址编码
    QGeoCodingManager *codingManager = serviceProvider.geocodingManager();

    // 示例：将起点和终点设置为经纬度坐标
    QGeoCoordinate startCoordinate(39.9042, 116.4074); // 北京
    QGeoCoordinate endCoordinate(39.915, 116.404); // 北京的另一个位置

    // 在场景中添加起点和终点标记
    QGraphicsEllipseItem *startMarker = new QGraphicsEllipseItem(-5, -5, 10, 10);
    startMarker->setBrush(Qt::red);
    mapScene->addItem(startMarker);
    startMarker->setPos(startCoordinate.longitude(), startCoordinate.latitude());

    QGraphicsEllipseItem *endMarker = new QGraphicsEllipseItem(-5, -5, 10, 10);
    endMarker->setBrush(Qt::green);
    mapScene->addItem(endMarker);
    endMarker->setPos(endCoordinate.longitude(), endCoordinate.latitude());

    // 创建路径示例（请根据实际需要创建路径）
    QPainterPath path;
    path.moveTo(startCoordinate.longitude(), startCoordinate.latitude());
    path.lineTo(endCoordinate.longitude(), endCoordinate.latitude());

    QGraphicsPathItem *pathItem = new QGraphicsPathItem(path);
    pathItem->setPen(QPen(Qt::blue, 2)); // 设置路径颜色和宽度
    mapScene->addItem(pathItem);

    // 设置视图的范围
    mapView->setSceneRect(-180, -90, 360, 180); // 设置视图的显示范围
    mapView->setRenderHint(QPainter::Antialiasing);

    // 将地图视图添加到 stackedWidget 中
    stackedWidget->addWidget(mapView);
}



void MainWindow::setupUI() {
    setWindowTitle("航空系统");
    setFixedSize(1200, 800);

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);



    menuList = new QListWidget();
    menuList->setViewMode(QListView::IconMode);
    menuList->setIconSize(QSize(48, 48));
    menuList->setFixedWidth(100);
    menuList->setSpacing(10);
    menuList->setDragEnabled(false);
    menuList->setDropIndicatorShown(false);


    menuList->addItem(new QListWidgetItem(QIcon(":/icons/route.svg"), "航线图"));
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/service.svg"), "服务大厅"));
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/personal.svg"), "个人中心"));

    menuList->setStyleSheet("QListWidget { background-color: #2E8BFF; color: white; font-size: 14px; border: none; }"
                            "QListWidget::item { padding: 10px; }"
                            "QListWidget::item:selected { background-color: #007BFF; }");
    stackedWidget = new QStackedWidget();

    createRouteMap();

    serviceHallWidget = new QWidget();
    QLabel *serviceHallLabel = new QLabel("服务大厅内容", serviceHallWidget);
    serviceHallLabel->setStyleSheet("font-size: 20px; font-weight: bold;");
    QVBoxLayout *serviceHallLayout = new QVBoxLayout(serviceHallWidget);
    serviceHallLayout->addWidget(serviceHallLabel);

    personalCenterWidget = new QWidget();
    QLabel *personalCenterLabel = new QLabel("个人中心内容", personalCenterWidget);
    personalCenterLabel->setStyleSheet("font-size: 20px; font-weight: bold;");
    QVBoxLayout *personalCenterLayout = new QVBoxLayout(personalCenterWidget);
    personalCenterLayout->addWidget(personalCenterLabel);

    stackedWidget->addWidget(serviceHallWidget);
    stackedWidget->addWidget(personalCenterWidget);

    QHBoxLayout *layout = new QHBoxLayout(centralWidget);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(10);
    layout->addWidget(menuList);
    layout->addWidget(stackedWidget);
}

void MainWindow::setupConnections() {
    connect(menuList, &QListWidget::currentRowChanged, this, [this](int index) {
        stackedWidget->setCurrentIndex(index);
    });
}