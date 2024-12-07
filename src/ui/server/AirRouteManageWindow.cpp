#include <ui/server/AirRouteManageWindow.h>
#include <file/fileManage.h>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QWebEngineView>

AirRouteManageWindow::AirRouteManageWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupConnections();
    populateList();
}

void AirRouteManageWindow::setupUI() {
    mainLayout = new QVBoxLayout(this);

    airRouteListWidget = new QListWidget(this);
    airRouteListWidget->setStyleSheet("border: 1px solid #ccc; background-color: #f9f9f9;");
    mainLayout->addWidget(airRouteListWidget);

    addAirRouteButton = new QPushButton("添加航线", this);
    addAirRouteButton->setStyleSheet("background-color: #4CAF50; color: white; font-size: 14px; padding: 8px;");
    mainLayout->addWidget(addAirRouteButton);

    setLayout(mainLayout);
}

void AirRouteManageWindow::setupConnections() {
    connect(addAirRouteButton, &QPushButton::clicked, this, &AirRouteManageWindow::openAddAirRouteWindow);
}

void AirRouteManageWindow::populateList() {
    airRouteListWidget->clear();
    air_route_map.traverse([this](const AirRoute &route) { addAirRouteItem(route); });
}

void AirRouteManageWindow::openAddAirRouteWindow() {
    QDialog dialog(this);
    dialog.setWindowTitle("添加航线");
    dialog.setFixedSize(400, 300);
    dialog.setStyleSheet("background-color: #f5f5f5; border-radius: 10px;");

    QVBoxLayout layout(&dialog);

    airRouteNameLineEdit = createLineEdit(&dialog, "请输入航线名称");
    airport1ComboBox = createComboBox(&dialog, "选择出发机场");
    airport2ComboBox = createComboBox(&dialog, "选择到达机场");

    populateAirportComboBoxes();

    QPushButton confirmButton("确认添加航线", &dialog);
    confirmButton.setStyleSheet("background-color: #4CAF50; color: white; font-size: 14px; padding: 8px;");
    layout.addWidget(airRouteNameLineEdit);
    layout.addWidget(airport1ComboBox);
    layout.addWidget(airport2ComboBox);
    layout.addWidget(&confirmButton);

    connect(&confirmButton, &QPushButton::clicked, this, [&]() {
        QString airlineName = airRouteNameLineEdit->text();
        QString airport1 = airport1ComboBox->currentText();
        QString airport2 = airport2ComboBox->currentText();
        if (airlineName.isEmpty() || !airport1ComboBox->currentIndex() || !airport2ComboBox->currentIndex()) {
            QMessageBox::warning(this, "错误", "航线名称和机场信息不能为空！");
            return;
        }
        openMapSearchWindow(airport1, airport2);
        dialog.accept();
    });

    dialog.exec();
}

QLineEdit* AirRouteManageWindow::createLineEdit(QWidget* parent, const QString& placeholder) {
    QLineEdit *lineEdit = new QLineEdit(parent);
    lineEdit->setPlaceholderText(placeholder);
    lineEdit->setStyleSheet("font-size: 14px; padding: 5px; border: 1px solid #ccc; border-radius: 5px;");
    return lineEdit;
}

QComboBox* AirRouteManageWindow::createComboBox(QWidget* parent, const QString& defaultText) {
    QComboBox *comboBox = new QComboBox(parent);
    comboBox->addItem(defaultText);
    comboBox->setStyleSheet("font-size: 14px; padding: 5px; border: 1px solid #ccc; border-radius: 5px;");
    return comboBox;
}

void AirRouteManageWindow::populateAirportComboBoxes() {
    airport_map.traverse([this](const Airport &airport) {
        airport1ComboBox->addItem(airport.getName().c_str());
        airport2ComboBox->addItem(airport.getName().c_str());
    });
}

void AirRouteManageWindow::openMapSearchWindow(const QString &airport1, const QString &airport2) {
    QDialog dialog(this);
    dialog.setWindowTitle("绘制航线");
    dialog.setFixedSize(900, 900);
    dialog.setStyleSheet("background-color: #f5f5f5; border-radius: 10px;");

    QVBoxLayout layout(&dialog);
    QWebEngineView webView(&dialog);
    QWebChannel channel(this);

    AirRouteManageBackend backend(this);
    setupBackendConnections(&backend, airport1, airport2);

    webView.page()->setWebChannel(&channel);
    channel.registerObject(QStringLiteral("qt_addAirRoute"), &backend);

    webView.load(QUrl("qrc:/pages/airRoute/addAirRoute.html"));
    layout.addWidget(&webView);

    dialog.setLayout(&layout);
    dialog.exec();
}

void AirRouteManageWindow::setupBackendConnections(AirRouteManageBackend *backend, const QString &airport1, const QString &airport2) {
    Airport *airport_p1 = airport_map.find(airport1.toStdString().c_str());
    Airport *airport_p2 = airport_map.find(airport2.toStdString().c_str());

    connect(backend, &AirRouteManageBackend::routeDataReceived, this, &AirRouteManageWindow::handleRouteData);
    connect(backend, &AirRouteManageBackend::airRouteDataRequested, this, [backend, airport1, airport2, airport_p1, airport_p2]() {
        QMetaObject::invokeMethod(backend, "receiveAirRouteData",
                                  Q_ARG(QString, airport1), Q_ARG(QString, airport2),
                                  Q_ARG(double, airport_p1->getPosition().getLatitude()), Q_ARG(double, airport_p1->getPosition().getLongitude()),
                                  Q_ARG(double, airport_p2->getPosition().getLatitude()), Q_ARG(double, airport_p2->getPosition().getLongitude()));
    });
}

void AirRouteManageWindow::handleRouteData(const QVariantList &routePoints, double routeLengthInKm) {
    LinkedList<Point> points;
    for (const QVariant &pointVariant : routePoints) {
        QVariantMap pointMap = pointVariant.toMap();
        double lat = pointMap["lat"].toDouble();
        double lng = pointMap["lng"].toDouble();
        points.append(Point(lat, lng));
    }
    QString airlineName = airRouteNameLineEdit->text();
    QString airport1 = airport1ComboBox->currentText();
    QString airport2 = airport2ComboBox->currentText();
    AirRoute newAirline(airlineName.toStdString().c_str(), airport1.toStdString().c_str(), airport2.toStdString().c_str(), points, routeLengthInKm);
    if (addElementToMap(air_route_map, newAirline, AIRLINES_PATH.c_str())) {
        addAirRouteItem(newAirline);
    } else {
        QMessageBox::warning(this, "错误", "此航线已存在！！！");
    }
}

void AirRouteManageWindow::addAirRouteItem(const AirRoute &airline) {
    AirRouteItem *item = new AirRouteItem(airline, airRouteListWidget);
    airRouteListWidget->addItem(item);
    connect(item->getDeleteButton(), &QPushButton::clicked, this, [this, item]() {
        onDeleteAirRoute(item);
    });
}

void AirRouteManageWindow::onDeleteAirRoute(AirRouteItem *item) {
    QMessageBox::StandardButton reply = QMessageBox::question(this, "确认删除", "确定要删除这个航线吗？", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        int row = airRouteListWidget->row(item);
        if (row != -1) {
            String key = item->getAirlineName().toStdString().c_str();
            if (deleteElementInMap(air_route_map, key, AIRLINES_PATH.c_str())) {
                delete airRouteListWidget->takeItem(row);
            } else {
                QMessageBox::warning(this, "错误", "删除航线失败！");
            }
        }
    }
}

AirRouteItem::AirRouteItem(const AirRoute &airline, QListWidget *parent)
    : QListWidgetItem(parent) {
    deleteButton = new QPushButton("删除", parent);
    deleteButton->setStyleSheet("background-color: #f44336; color: white; padding: 5px;");
    nameLabel = new QLabel(airline.getName().c_str(), parent);
    airport1Label = new QLabel(airline.getAirport1().c_str(), parent);
    airport2Label = new QLabel(airline.getAirport2().c_str(), parent);

    deleteButton->setFixedWidth(80);
    nameLabel->setFixedWidth(200);
    airport1Label->setFixedWidth(200);
    airport2Label->setFixedWidth(200);

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

QPushButton* AirRouteItem::getDeleteButton() {
    return deleteButton;
}

QString AirRouteItem::getAirlineName() {
    return nameLabel->text();
}