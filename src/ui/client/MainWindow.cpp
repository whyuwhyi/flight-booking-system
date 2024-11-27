#include <ui/client/MainWindow.h>
#include <file/fileManage.h>
#include <QFile>
#include <QTextStream>
#include <QVBoxLayout>
#include <QListWidgetItem>
#include <QDebug>
#include <QTimer>

MapBackend::MapBackend(QObject *parent) : QObject(parent) {}

void MapBackend::requestRoutesData() {
    QFile file(AIRLINES_PATH.c_str());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件：" << file.errorString();
        emit sendRoutesData("");
        return;
    }

    QTextStream in(&file);
    QString routesData = in.readAll();
    emit sendRoutesData(routesData);
}

void MapBackend::requestUserOrderInfo() {
    String filePath = USERS_DIR + current_login_user.getPhoneNumber() + "/tickets.txt";
    QFile file(filePath.c_str());

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件：" << file.errorString();
        emit sendUserOrderInfo("");
        return;
    }

    QTextStream in(&file);
    QString orderInfo = in.readAll();
    emit sendUserOrderInfo(orderInfo);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), webChannel(nullptr), mapBackend(nullptr) {
    
    loadFlightNetworkFromFile();
    loadMapFromFile(airplane_model_map, MODELS_PATH.c_str());

    setupUI();
    setupConnections();
    showLoginWindow();
}    

MainWindow::~MainWindow() {}


void MainWindow::setupUI() {
    setWindowTitle("航空系统");
    setFixedSize(1300, 1000);

    stackedWidget = new QStackedWidget(this);
    loginWindow = new LoginWindow(this);
    registerWindow = new RegisterWindow(this);
    ticketBookingWindow = new TicketBookingWindow(this);
    orderWindow = new OrderWindow(this);
    routeMapWidget = new QWidget(this);

    stackedWidget->addWidget(loginWindow);
    stackedWidget->addWidget(registerWindow);
    stackedWidget->addWidget(routeMapWidget);
    stackedWidget->addWidget(ticketBookingWindow);
    stackedWidget->addWidget(orderWindow);

    mapBackend = new MapBackend(this);
    webChannel = new QWebChannel(this);
    webChannel->registerObject(QStringLiteral("qt_map"), mapBackend);

    mapView = new QWebEngineView(this);
    mapView->page()->setWebChannel(webChannel);
    mapView->setUrl(QUrl("qrc:/pages/map.html"));

    QVBoxLayout *routeLayout = new QVBoxLayout(routeMapWidget);
    routeLayout->addWidget(mapView);


    menuList = new QListWidget(this);
    menuList->setViewMode(QListView::IconMode);
    menuList->setIconSize(QSize(48, 48));
    menuList->setFixedWidth(100);
    menuList->setSpacing(10);
    menuList->setMovement(QListView::Static);
    menuList->setSelectionMode(QAbstractItemView::SingleSelection);

    // 设置样式表以对齐图标和文本，并自定义选中颜色
    menuList->setStyleSheet(R"(
        QListWidget::item {
            width: 80px; /* 固定宽度确保文本对齐 */
            text-align: center; /* 居中对齐文本 */
            margin: 5px;
        }
        QListWidget::item:selected {
            background: #2a9d8f; /* 自定义选中背景色 */
            color: white; /* 选中文字颜色 */
        }

    )");

    menuList->addItem(new QListWidgetItem(QIcon(":/icons/route.svg"), "航线图"));
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/service.svg"), "订  票"));
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/personal.svg"), "订  单"));


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
        switch (index) {
            case 0:
                stackedWidget->setCurrentWidget(routeMapWidget);
                break;
            case 1:
                stackedWidget->setCurrentWidget(ticketBookingWindow);
                break;
            case 2:
                stackedWidget->setCurrentWidget(orderWindow);
                orderWindow->refreshOrderList();
                break;
            default:
                break;
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
    mapBackend->requestRoutesData();
    mapBackend->requestUserOrderInfo();
    stackedWidget->removeWidget(loginWindow);
    delete loginWindow;
    loginWindow = nullptr;

    stackedWidget->removeWidget(registerWindow);
    delete registerWindow;
    registerWindow = nullptr;


    stackedWidget->setCurrentWidget(routeMapWidget);
    menuList->setCurrentRow(0);
    menuList->setVisible(true);
}