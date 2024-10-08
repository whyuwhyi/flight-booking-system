#include <ui/client/MainWindow.h>
#include <QFile>
#include <QTextStream>
#include <QVBoxLayout>
#include <QListWidgetItem>
#include <QDebug>
#include <QTimer>

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
    showLoginWindow();
}

MainWindow::~MainWindow() {
    delete loginWindow;
    delete registerWindow;
    delete ticketBookingWindow;
    delete webChannel;
    delete mapBackend;
    delete mapView;
}

void MainWindow::setupUI() {
    setWindowTitle("航空系统");
    setFixedSize(1200, 800);

    stackedWidget = new QStackedWidget(this);
    loginWindow = new LoginWindow(this);
    registerWindow = new RegisterWindow(this);
    ticketBookingWindow = new TicketBookingWindow(this);

    stackedWidget->addWidget(loginWindow);
    stackedWidget->addWidget(registerWindow);
    stackedWidget->addWidget(ticketBookingWindow);

    routeMapWidget = new QWidget(this);
    serviceHallWidget = new QWidget(this);
    personalCenterWidget = new QWidget(this);

    mapBackend = new MapBackend(this);
    webChannel = new QWebChannel(this);
    webChannel->registerObject(QStringLiteral("qt_map"), mapBackend);

    mapView = new QWebEngineView(this);
    mapView->page()->setWebChannel(webChannel);
    mapView->setUrl(QUrl("qrc:/pages/map.html"));

    QVBoxLayout *routeLayout = new QVBoxLayout(routeMapWidget);
    routeLayout->addWidget(mapView);

    stackedWidget->addWidget(routeMapWidget);
    stackedWidget->addWidget(serviceHallWidget);
    stackedWidget->addWidget(personalCenterWidget);

    menuList = new QListWidget(this);
    menuList->setViewMode(QListView::IconMode);
    menuList->setIconSize(QSize(48, 48));
    menuList->setFixedWidth(100);
    menuList->setSpacing(10);
    menuList->setMovement(QListView::Static);
    menuList->setSelectionMode(QAbstractItemView::SingleSelection);
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/route.svg"), "航线图"));
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/service.svg"), "查/订票"));
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/personal.svg"), "个人中心"));

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(menuList);
    mainLayout->addWidget(stackedWidget);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    menuList->setVisible(false);
}

void MainWindow::setupConnections() {
    connect(loginWindow, &LoginWindow::registerRequested, this, &MainWindow::showRegisterWindow);
    connect(loginWindow, &LoginWindow::loginSuccess, this, &MainWindow::showMainContent);
    connect(registerWindow, &RegisterWindow::loginRequested, this, &MainWindow::showLoginWindow);
    connect(menuList, &QListWidget::currentRowChanged, this, [this](int index) {
        if (index == 1) {
            stackedWidget->setCurrentWidget(ticketBookingWindow);
        } else if (stackedWidget->currentIndex() >= 3) {
            stackedWidget->setCurrentIndex(index + 3);
        }
    });
}

void MainWindow::showLoginWindow() {
    stackedWidget->setCurrentWidget(loginWindow);
    menuList->setVisible(false);
    loginWindow->checkAutoLogin();
}

void MainWindow::showRegisterWindow() {
    stackedWidget->setCurrentWidget(registerWindow);
    menuList->setVisible(false);
}

void MainWindow::showMainContent() {
    stackedWidget->setCurrentIndex(3);
    menuList->setCurrentRow(0);
    menuList->setVisible(true);
}