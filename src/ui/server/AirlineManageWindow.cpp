#include <ui/server/AirlineManageWindow.h>
#include <data/datamanage.h>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QWebEngineView>

AirlineManageWindow::AirlineManageWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupConnections();
}

void AirlineManageWindow::setupUI() {
    mainLayout = new QVBoxLayout(this);
    airlineListWidget = new QListWidget(this);
    mainLayout->addWidget(airlineListWidget);
    addAirlineButton = new QPushButton("添加航线", this);
    mainLayout->addWidget(addAirlineButton);
    airline_map.traverse([this](const Airline& airline){ addAirlineItem(airline); });
    setLayout(mainLayout);
}

void AirlineManageWindow::setupConnections() {
    connect(addAirlineButton, &QPushButton::clicked, this, &AirlineManageWindow::openAddAirlineWindow);
}

void AirlineManageWindow::openAddAirlineWindow() {
    QDialog *addAirlineDialog = new QDialog(this);
    addAirlineDialog->setWindowTitle("添加航线");
    airlineNameLineEdit = new QLineEdit(addAirlineDialog);
    airlineNameLineEdit->setPlaceholderText("请输入航线名称");
    airport1ComboBox = new QComboBox(addAirlineDialog);
    airport2ComboBox = new QComboBox(addAirlineDialog);

    airport_map.traverse([this](const Airport& airport) {
        airport1ComboBox->addItem(airport.getName().c_str());
        airport2ComboBox->addItem(airport.getName().c_str());
    });

    confirmAddAirlineButton = new QPushButton("确认添加航线", addAirlineDialog);
    QVBoxLayout *layout = new QVBoxLayout(addAirlineDialog);
    layout->addWidget(airlineNameLineEdit);
    layout->addWidget(airport1ComboBox);
    layout->addWidget(airport2ComboBox);
    layout->addWidget(confirmAddAirlineButton);
    addAirlineDialog->setLayout(layout);
    addAirlineDialog->resize(400, 300);

    connect(confirmAddAirlineButton, &QPushButton::clicked, this, [this, addAirlineDialog]() {
        QString airlineName = airlineNameLineEdit->text();
        QString airport1 = airport1ComboBox->currentText();
        QString airport2 = airport2ComboBox->currentText();
        if (airlineName.isEmpty() || airport1.isEmpty() || airport2.isEmpty()) {
            QMessageBox::warning(this, "错误", "航线名称和机场信息不能为空！");
            return;
        }
        openMapSearchWindow(airport1, airport2);
        addAirlineDialog->accept();
    });

    addAirlineDialog->exec();
}

void AirlineManageWindow::handleRouteData(const QVariantList &routePoints, double routeLengthInKm) {
    LinkedList<Point> points;
    for (const QVariant &pointVariant : routePoints) {
        QVariantMap pointMap = pointVariant.toMap();
        double lat = pointMap["lat"].toDouble();
        double lng = pointMap["lng"].toDouble();
        points.append(Point(lat, lng));
    }
    QString airlineName = airlineNameLineEdit->text();
    QString airport1 = airport1ComboBox->currentText();
    QString airport2 = airport2ComboBox->currentText();
    Airline newAirline(airlineName.toStdString().c_str(), airport1.toStdString().c_str(), airport2.toStdString().c_str(), points, routeLengthInKm);
    if (addAirline(newAirline)) {
        addAirlineItem(newAirline);
    } else {
        QMessageBox::warning(this, "错误", "此航线已存在！！！");
    }
}

void AirlineManageWindow::openMapSearchWindow(const QString &airport1, const QString &airport2) {
    QDialog *mapDialog = new QDialog(this);
    Airport* airport_p1 = airport_map.find(airport1.toStdString().c_str());
    Airport* airport_p2 = airport_map.find(airport2.toStdString().c_str());
    mapDialog->setWindowTitle("绘制航线");
    mapDialog->resize(900, 900);
    QVBoxLayout *layout = new QVBoxLayout(mapDialog);
    QWebEngineView *webView = new QWebEngineView(mapDialog);
    QWebChannel *channel = new QWebChannel(this);
    AirlineManageBackend *backend = new AirlineManageBackend(this);
    connect(backend, &AirlineManageBackend::routeDataReceived, this, &AirlineManageWindow::handleRouteData);
    connect(backend, &AirlineManageBackend::airlineDataRequested, this, [backend, airport1, airport2, airport_p1, airport_p2]() {
        QMetaObject::invokeMethod(backend, "receiveAirlineData",
            Q_ARG(QString, airport1), Q_ARG(QString, airport2),
            Q_ARG(double, airport_p1->getPosition().getLatitude()), Q_ARG(double, airport_p1->getPosition().getLongitude()),
            Q_ARG(double, airport_p2->getPosition().getLatitude()), Q_ARG(double, airport_p2->getPosition().getLongitude()));
    });

    webView->page()->setWebChannel(channel);
    channel->registerObject(QStringLiteral("qt_addAirline"), backend);

    webView->load(QUrl("qrc:/pages/airline/addAirline.html"));
    layout->addWidget(webView);
    mapDialog->setLayout(layout);
    mapDialog->exec();
}

void AirlineManageWindow::handleAirlineData(const QString &name, const QString &airport1, const QString &airport2) {
    Airline newAirline(name.toStdString().c_str(), airport1.toStdString().c_str(), airport2.toStdString().c_str());
    if (addAirline(newAirline)) {
        addAirlineItem(newAirline);
    } else {
        QMessageBox::warning(this, "错误", "此航线已存在！！！");
    }
}

void AirlineManageWindow::addAirlineItem(const Airline& airline) {
    AirlineItem *item = new AirlineItem(airline, airlineListWidget);
    airlineListWidget->addItem(item);
    connect(item->getDeleteButton(), &QPushButton::clicked, this, [this, item]() {
        onDeleteAirline(item);
    });
}

void AirlineManageWindow::onDeleteAirline(AirlineItem *item) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除", "确定要删除这个航线吗？", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        int row = airlineListWidget->row(item);
        if (row != -1) {
            deleteAirline(item->getAirlineName().toStdString().c_str());
            delete airlineListWidget->takeItem(row);
        }
    }
}

AirlineManageWindow::AirlineItem::AirlineItem(const Airline& airline, QListWidget *parent)
    : QListWidgetItem(parent) {
    deleteButton = new QPushButton("删除", parent);
    nameLabel = new QLabel(airline.getName().c_str(), parent);
    airport1Label = new QLabel(airline.getAirport1().c_str(), parent);
    airport2Label = new QLabel(airline.getAirport2().c_str(), parent);
    QWidget *itemWidget = new QWidget(parent);
    QHBoxLayout *layout = new QHBoxLayout(itemWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(nameLabel);
    layout->addWidget(airport1Label);
    layout->addWidget(airport2Label);
    layout->addWidget(deleteButton);
    itemWidget->setLayout(layout);
    itemWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setSizeHint(itemWidget->sizeHint());
    parent->setItemWidget(this, itemWidget);
}

QPushButton* AirlineManageWindow::AirlineItem::getDeleteButton() {
    return deleteButton;
}

QString AirlineManageWindow::AirlineItem::getAirlineName() {
    return nameLabel->text();
}