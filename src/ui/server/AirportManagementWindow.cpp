#include <ui/server/AirportManagementWindow.h>
#include <data/datamanagement.h>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QWebEngineView>

AirportManagementWindow::AirportManagementWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupConnections();
}

void AirportManagementWindow::setupUI() {
    mainLayout = new QVBoxLayout(this);

    // searchLineEdit = new QLineEdit(this);
    // searchLineEdit->setPlaceholderText("搜索机场...");
    // mainLayout->addWidget(searchLineEdit);

    airportListWidget = new QListWidget(this);
    mainLayout->addWidget(airportListWidget);

    addAirportButton = new QPushButton("添加机场", this);
    mainLayout->addWidget(addAirportButton);

    airport_map.traverse([this](const Airport& airport){ addAirportItem(airport); });
}

void AirportManagementWindow::setupConnections() {
    connect(addAirportButton, &QPushButton::clicked, this, [this]() {
        openMapSearchWindow();
    });

    // connect(searchLineEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
    //     // 搜索逻辑
    // });
}

void AirportManagementWindow::openMapSearchWindow() {
    // 创建并配置 HTML 窗口
    QDialog *mapDialog = new QDialog(this);
    mapDialog->setWindowTitle("搜索机场");

    // 设置对话框的初始大小，先给个适中的初始值
    mapDialog->resize(800, 650);

    QVBoxLayout *layout = new QVBoxLayout(mapDialog);
    QWebEngineView *webView = new QWebEngineView(mapDialog);
    QWebChannel *channel = new QWebChannel(this);

    Backend *backend = new Backend(this);
    connect(backend, &Backend::airportDataReceived, this, &AirportManagementWindow::handleAirportData);

    webView->page()->setWebChannel(channel);
    channel->registerObject(QStringLiteral("qt"), backend);

    webView->load(QUrl("qrc:/pages/addAirport.html"));
    layout->addWidget(webView);
    mapDialog->setLayout(layout);
    mapDialog->exec();
}


void AirportManagementWindow::handleAirportData(const QString &name, const QString &country, const QString &city, double latitude, double longitude) {
    Airport newAirport(name.toStdString().c_str(), country.toStdString().c_str(), city.toStdString().c_str(), Point(latitude, longitude));
    if (addAirport(newAirport)) {
        addAirportItem(newAirport);
    } else {
        QMessageBox::warning(this, "错误", "此机场已存在！！！");
    }
}


void AirportManagementWindow::addAirportItem(const Airport& airport) {
    AirportItem *item = new AirportItem(airport, airportListWidget);
    airportListWidget->addItem(item);
    connect(item->getDeleteButton(), &QPushButton::clicked, this, [this, item]() {
        onDeleteAirport(item);
    });
}

void AirportManagementWindow::onDeleteAirport(AirportItem *item) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除", "确定要删除这个机场吗？", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        int row = airportListWidget->row(item);
        if (row != -1) {
            deleteAirport(item->getAirportName());
            delete airportListWidget->takeItem(row);
        }
    }
}

AirportManagementWindow::AirportItem::AirportItem(const Airport& airport, QListWidget *parent)
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

QPushButton* AirportManagementWindow::AirportItem::getDeleteButton() {
    return deleteButton;
}

String AirportManagementWindow::AirportItem::getAirportName() {
    return nameLabel->text().toStdString().c_str();
}
