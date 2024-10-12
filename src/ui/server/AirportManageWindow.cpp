#include <ui/server/AirportManageWindow.h>
#include <data/datamanage.h>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QWebEngineView>

AirportManageWindow::AirportManageWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupConnections();
}

void AirportManageWindow::setupUI() {
    mainLayout = new QVBoxLayout(this);

    airportListWidget = new QListWidget(this);
    mainLayout->addWidget(airportListWidget);

    addAirportButton = new QPushButton("添加机场", this);
    mainLayout->addWidget(addAirportButton);

    airport_map.traverse([this](const Airport& airport){ addAirportItem(airport); });
}

void AirportManageWindow::setupConnections() {
    connect(addAirportButton, &QPushButton::clicked, this, &AirportManageWindow::openMapSearchWindow);
}

void AirportManageWindow::openMapSearchWindow() {
    QDialog *mapDialog = new QDialog(this);
    mapDialog->setWindowTitle("搜索机场");
    mapDialog->resize(800, 650);

    QVBoxLayout *layout = new QVBoxLayout(mapDialog);
    QWebEngineView *webView = new QWebEngineView(mapDialog);
    QWebChannel *channel = new QWebChannel(this);

    AirportManageBackend *backend = new AirportManageBackend(this);
    connect(backend, &AirportManageBackend::airportDataReceived, this, &AirportManageWindow::handleAirportData);

    webView->page()->setWebChannel(channel);
    channel->registerObject(QStringLiteral("qt_addAirport"), backend);

    webView->load(QUrl("qrc:/pages/airport/addAirport.html"));
    layout->addWidget(webView);
    mapDialog->setLayout(layout);
    mapDialog->exec();
}

void AirportManageWindow::handleAirportData(const QString &name, const QString &country, const QString &city, double latitude, double longitude) {
    Airport newAirport(name.toStdString().c_str(), country.toStdString().c_str(), city.toStdString().c_str(), Point(longitude, latitude));
    if (addAirport(newAirport)) {
        addAirportItem(newAirport);
    } else {
        QMessageBox::warning(this, "错误", "此机场已存在！！！");
    }
}

void AirportManageWindow::addAirportItem(const Airport& airport) {
    AirportItem *item = new AirportItem(airport, airportListWidget);
    airportListWidget->addItem(item);
    connect(item->getDeleteButton(), &QPushButton::clicked, this, [this, item]() {
        onDeleteAirport(item);
    });
}

void AirportManageWindow::onDeleteAirport(AirportItem *item) {
    QMessageBox::StandardButton reply = QMessageBox::question(this, "确认删除", "确定要删除这个机场吗？", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        int row = airportListWidget->row(item);
        if (row != -1) {
            deleteAirport(item->getAirportName());
            delete airportListWidget->takeItem(row);
        }
    }
}

AirportItem::AirportItem(const Airport& airport, QListWidget *parent)
    : QListWidgetItem(parent) {
    deleteButton = new QPushButton("删除", parent);
    nameLabel = new QLabel(airport.getName().c_str(), parent);
    countryLabel = new QLabel(airport.getCountry().c_str(), parent);
    cityLabel = new QLabel(airport.getCity().c_str(), parent);

    QWidget *itemWidget = new QWidget(parent);
    QHBoxLayout *layout = new QHBoxLayout(itemWidget);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(nameLabel);
    layout->addWidget(countryLabel);
    layout->addWidget(cityLabel);
    layout->addWidget(deleteButton);

    itemWidget->setLayout(layout);
    itemWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    setSizeHint(itemWidget->sizeHint());
    parent->setItemWidget(this, itemWidget);
}

QPushButton* AirportItem::getDeleteButton() {
    return deleteButton;
}

String AirportItem::getAirportName() {
    return nameLabel->text().toStdString().c_str();
}
