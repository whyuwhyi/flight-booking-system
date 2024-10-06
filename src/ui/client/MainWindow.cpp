#include <ui/client/MainWindow.h>
#include <QFile>
#include <QTextStream>
#include <QVBoxLayout>
#include <QListWidgetItem>

MapBackend::MapBackend(QObject *parent) : QObject(parent) {}

void MapBackend::requestRoutesData() {
    QFile file("/home/yuyi/cs-learnning/cpp-projects/curriculum-design/flight-booking-system/data/airline/airlines.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件：" << file.errorString();
        emit sendRoutesData("");
        return;
    }
    

    QTextStream in(&file);
    QString routesData = in.readAll();
    emit sendRoutesData(routesData);
}

MainWindow::MainWindow(QWidget *parent) 
    : QMainWindow(parent), webChannel(nullptr), mapBackend(nullptr) {
    setupUI();
    setupConnections();
}

MainWindow::~MainWindow() {
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

    menuList->addItem(new QListWidgetItem(QIcon(":/icons/route.svg"), "航线图"));
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/service.svg"), "服务大厅"));
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/personal.svg"), "个人中心"));

    stackedWidget = new QStackedWidget();

    mapBackend = new MapBackend(this);
    webChannel = new QWebChannel(this);
    webChannel->registerObject(QStringLiteral("qt_map"), mapBackend);

    mapView = new QWebEngineView(this);
    mapView->page()->setWebChannel(webChannel);
    mapView->setUrl(QUrl("qrc:/pages/map.html"));

    routeMapWidget = new QWidget(this);
    QVBoxLayout *routeLayout = new QVBoxLayout(routeMapWidget);
    routeLayout->addWidget(mapView);

    serviceHallWidget = new QWidget();
    personalCenterWidget = new QWidget();

    stackedWidget->addWidget(routeMapWidget);
    stackedWidget->addWidget(serviceHallWidget);
    stackedWidget->addWidget(personalCenterWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->addWidget(menuList);
    mainLayout->addWidget(stackedWidget);
}

void MainWindow::setupConnections() {
    connect(menuList, &QListWidget::currentRowChanged, this, [this](int index) {
        stackedWidget->setCurrentIndex(index);
    });
}

void MainWindow::createMapRoute() {
    mapView->setUrl(QUrl("qrc:/pages/map.html"));
}