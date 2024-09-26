#include <ui/MainWindow.h>
#include <QHBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUi();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUi() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    

    QHBoxLayout *layout = new QHBoxLayout(centralWidget);

    menuList = new QListWidget();
    menuList->addItem("航线图");
    menuList->addItem("服务大厅");
    menuList->addItem("个人中心");

    stackedWidget = new QStackedWidget();

    routeMapWidget = new QWidget();
    QLabel *routeMapLabel = new QLabel("航线图内容", routeMapWidget);
    QVBoxLayout *routeMapLayout = new QVBoxLayout(routeMapWidget);
    routeMapLayout->addWidget(routeMapLabel);

    serviceHallWidget = new QWidget();
    QLabel *serviceHallLabel = new QLabel("服务大厅内容", serviceHallWidget);
    QVBoxLayout *serviceHallLayout = new QVBoxLayout(serviceHallWidget);
    serviceHallLayout->addWidget(serviceHallLabel);

    personalCenterWidget = new QWidget();
    QLabel *personalCenterLabel = new QLabel("个人中心内容", personalCenterWidget);
    QVBoxLayout *personalCenterLayout = new QVBoxLayout(personalCenterWidget);
    personalCenterLayout->addWidget(personalCenterLabel);

    stackedWidget->addWidget(routeMapWidget);
    stackedWidget->addWidget(serviceHallWidget);
    stackedWidget->addWidget(personalCenterWidget);

    layout->addWidget(menuList);
    layout->addWidget(stackedWidget);

    connect(menuList, &QListWidget::currentRowChanged, this, &MainWindow::changePage);
}

void MainWindow::changePage(int index) {
    stackedWidget->setCurrentIndex(index);
}
