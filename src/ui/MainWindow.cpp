#include <ui/MainWindow.h>
#include <QPushButton>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QIcon>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWebEngineView>




MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUI();
    setupConnections();
}

MainWindow::~MainWindow() {}

void MainWindow::createMapRoute() {
    mapView = new QWebEngineView(this);
    mapView->load(QUrl("qrc:/pages/map.html"));
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

    createMapRoute();

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