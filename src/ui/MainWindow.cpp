#include <ui/MainWindow.h>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidgetItem>
#include <QIcon>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGeoServiceProvider>
#include <QGeoRoutingManager>
#include <QPlace>
#include <QGraphicsEllipseItem>
#include <QPlaceManager>
#include <QGeoRoute>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUI();
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
    setWindowTitle("天地航空系统");
    setFixedSize(1200, 800); // 固定窗口大小

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout *layout = new QHBoxLayout(centralWidget);
    layout->setContentsMargins(10, 10, 10, 10); // 设置边距
    layout->setSpacing(10); // 设置间距

    menuList = new QListWidget();
    menuList->setViewMode(QListView::IconMode); // 设置为图标模式
    menuList->setIconSize(QSize(48, 48)); // 设置图标大小
    menuList->setFixedWidth(100); // 减短菜单栏宽度
    menuList->setSpacing(10); // 图标与文字间距

    // 添加图标和文字
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/route.svg"), "航线图"));
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/service.svg"), "服务大厅"));
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/personal.svg"), "个人中心"));

    menuList->setStyleSheet("QListWidget { background-color: #2E8BFF; color: white; font-size: 14px; border: none; }"
                            "QListWidget::item { padding: 10px; }"
                            "QListWidget::item:selected { background-color: #007BFF; }");

    stackedWidget = new QStackedWidget();

    createRouteMap();

    // 设置其他模块
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

    layout->addWidget(menuList);
    layout->addWidget(stackedWidget);

    connect(menuList, &QListWidget::currentRowChanged, this, &MainWindow::changePage);
}


void MainWindow::changePage(int index) {
    stackedWidget->setCurrentIndex(index);
}
