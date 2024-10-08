#include <ui/client/TicketBookingWindow.h>

TicketBookingWindow::TicketBookingWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupConnections();
}

void TicketBookingWindow::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *departureLabel = new QLabel("出发城市:", this);
    departureCityComboBox = new QComboBox(this);
    departureCityComboBox->setMinimumHeight(30);
    departureCityComboBox->setStyleSheet("font-size: 16px;");

    QLabel *arrivalLabel = new QLabel("到达城市:", this);
    arrivalCityComboBox = new QComboBox(this);
    arrivalCityComboBox->setMinimumHeight(30);
    arrivalCityComboBox->setStyleSheet("font-size: 16px;");

    QLabel *dateLabel = new QLabel("出发日期:", this);
    dateEdit = new QDateEdit(QDate::currentDate(), this);
    dateEdit->setCalendarPopup(true);
    dateEdit->setMinimumHeight(30);
    dateEdit->setStyleSheet("font-size: 16px;");

    searchButton = new QPushButton("搜索航班", this);
    searchButton->setMinimumHeight(40);
    searchButton->setStyleSheet("font-size: 18px; background-color: #007bff; color: white; border-radius: 5px;");

    flightListWidget = new QListWidget(this);
    flightListWidget->setMinimumHeight(150);
    flightListWidget->setStyleSheet("font-size: 16px;");

    bookButton = new QPushButton("预订航班", this);
    bookButton->setMinimumHeight(40);
    bookButton->setStyleSheet("font-size: 18px; background-color: #28a745; color: white; border-radius: 5px;");

    mainLayout->setSpacing(15);
    mainLayout->addWidget(departureLabel);
    mainLayout->addWidget(departureCityComboBox);
    mainLayout->addWidget(arrivalLabel);
    mainLayout->addWidget(arrivalCityComboBox);
    mainLayout->addWidget(dateLabel);
    mainLayout->addWidget(dateEdit);
    mainLayout->addWidget(searchButton);
    mainLayout->addWidget(flightListWidget);
    mainLayout->addWidget(bookButton);

    setLayout(mainLayout);
}

void TicketBookingWindow::setupConnections() {
    connect(searchButton, &QPushButton::clicked, this, &TicketBookingWindow::onSearchButtonClicked);
    connect(bookButton, &QPushButton::clicked, this, &TicketBookingWindow::onBookButtonClicked);
}

void TicketBookingWindow::onSearchButtonClicked() {
    populateFlightList();
}

void TicketBookingWindow::onBookButtonClicked() {
    QListWidgetItem *selectedItem = flightListWidget->currentItem();
    if (selectedItem) {
        QMessageBox::information(this, "预订成功", "您已成功预订航班: " + selectedItem->text());
    } else {
        QMessageBox::warning(this, "预订失败", "请选择要预订的航班。");
    }
}

void TicketBookingWindow::populateFlightList() {

}