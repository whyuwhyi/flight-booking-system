#include <ui/client/TicketBookingWindow.h>
#include <data/datamanage.h>
#include <QHBoxLayout>
#include <QListWidgetItem>
#include <QDebug>
#include <QStringList>

TicketBookingWindow::TicketBookingWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupConnections();
    populateCityComboBoxes();
}

void TicketBookingWindow::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    QHBoxLayout *selectionLayout = new QHBoxLayout();
    
    QVBoxLayout *departureLayout = new QVBoxLayout();
    QLabel *departureLabel = new QLabel("出发城市:", this);
    departureCityComboBox = new QComboBox(this);
    departureCityComboBox->setMinimumHeight(30);
    departureCityComboBox->setStyleSheet("font-size: 16px;");
    departureLayout->addWidget(departureLabel);
    departureLayout->addWidget(departureCityComboBox);
    
    QVBoxLayout *arrivalLayout = new QVBoxLayout();
    QLabel *arrivalLabel = new QLabel("到达城市:", this);
    arrivalCityComboBox = new QComboBox(this);
    arrivalCityComboBox->setMinimumHeight(30);
    arrivalCityComboBox->setStyleSheet("font-size: 16px;");
    arrivalLayout->addWidget(arrivalLabel);
    arrivalLayout->addWidget(arrivalCityComboBox);
    
    QVBoxLayout *dateLayout = new QVBoxLayout();
    QLabel *dateLabel = new QLabel("出发日期:", this);
    dateEdit = new QDateEdit(QDate::currentDate(), this);
    dateEdit->setCalendarPopup(true);
    dateEdit->setMinimumHeight(30);
    dateEdit->setStyleSheet("font-size: 16px;");
    dateLayout->addWidget(dateLabel);
    dateLayout->addWidget(dateEdit);
    
    searchButton = new QPushButton("搜索", this);
    searchButton->setMinimumHeight(40);
    searchButton->setFixedWidth(120);
    
    selectionLayout->addLayout(departureLayout);
    selectionLayout->addLayout(arrivalLayout);
    selectionLayout->addLayout(dateLayout);
    selectionLayout->addStretch();
    selectionLayout->addWidget(searchButton);
    
    QVBoxLayout *filterLayout = new QVBoxLayout();
    QLabel *filterLabel = new QLabel("排序选项:", this);
    filterComboBox = new QComboBox(this);
    filterComboBox->addItems({"时间最短", "价格最低"});
    filterLayout->addWidget(filterLabel);
    filterLayout->addWidget(filterComboBox);
    selectionLayout->addLayout(filterLayout);
    
    flightListWidget = new QListWidget(this);
    flightListWidget->setStyleSheet("font-size: 16px;");
    
    mainLayout->addLayout(selectionLayout);
    mainLayout->addWidget(flightListWidget, 1);
    
    setLayout(mainLayout);
    
    flightListWidget->setUniformItemSizes(true);
    flightListWidget->setSelectionMode(QAbstractItemView::NoSelection);
    flightListWidget->setFocusPolicy(Qt::NoFocus);
}

void TicketBookingWindow::setupConnections() {
    connect(searchButton, &QPushButton::clicked, this, &TicketBookingWindow::onSearchButtonClicked);
}

void TicketBookingWindow::populateCityComboBoxes() {
    flight_network.traverseCities([this](const String &city) {
        QString cityName = QString::fromUtf8(city.c_str());
        departureCityComboBox->addItem(cityName);
        arrivalCityComboBox->addItem(cityName);
    });
}

void TicketBookingWindow::onSearchButtonClicked() {
    populateFlightList();
}

void TicketBookingWindow::populateFlightList() {
    flightListWidget->clear();

    String departureCity(departureCityComboBox->currentText().toStdString().c_str());
    String arrivalCity(arrivalCityComboBox->currentText().toStdString().c_str());
    QString qDateStr = dateEdit->date().toString("yyyy-MM-dd");
    Date selectedDate = Date::fromString(qDateStr.toStdString().c_str());

    if (departureCity == arrivalCity) {
        QMessageBox::warning(this, "错误", "出发城市和到达城市不能相同");
        return;
    }

    Map<Ticket, Ticket> directFlights = std::move(flight_network.findDirectFlights(departureCity, arrivalCity, selectedDate));
    directFlights.traverse([this](const Ticket& ticket) {
        addTicketItem(ticket);
    });

    Map<ConnectingTicket, ConnectingTicket> connectingFlights = std::move(flight_network.findConnectingFlights(departureCity, arrivalCity, selectedDate, 2));
    connectingFlights.traverse([this](const ConnectingTicket& connectingTicket) {
        addConnectingTicketItem(connectingTicket);
    });

    if (flightListWidget->count() == 0) {
        QMessageBox::information(this, "无航班", "未查到符合条件的航班");
    }
}

void TicketBookingWindow::addTicketItem(const Ticket &ticket) {
    TicketItem *item = new TicketItem(ticket, flightListWidget);
    QListWidgetItem *listItem = new QListWidgetItem(flightListWidget);
    listItem->setSizeHint(item->sizeHint());
    flightListWidget->addItem(listItem);
    flightListWidget->setItemWidget(listItem, item);
    connect(item->getBookButton(), &QPushButton::clicked, this, [this, ticket]() {
        onBookButtonClicked(ticket);
    });
}

void TicketBookingWindow::addConnectingTicketItem(const ConnectingTicket &connectingTicket) {
    ConnectingTicketItem *item = new ConnectingTicketItem(connectingTicket, flightListWidget);
    QListWidgetItem *listItem = new QListWidgetItem(flightListWidget);
    listItem->setSizeHint(item->sizeHint());
    flightListWidget->addItem(listItem);
    flightListWidget->setItemWidget(listItem, item);
    connect(item->getBookButton(), &QPushButton::clicked, this, [this, connectingTicket]() {
        onBookConnectingButtonClicked(connectingTicket);
    });
}

void TicketBookingWindow::onBookButtonClicked(const Ticket &ticket) {
    QMessageBox::information(this, "预订成功", "你已成功预订航班: " + QString::fromUtf8(ticket.getFlight()->getFlightName().c_str()));
}

void TicketBookingWindow::onBookConnectingButtonClicked(const ConnectingTicket &connectingTicket) {
    QMessageBox::information(this, "预订成功", "您已成功预订联程航班。");
}

TicketItem::TicketItem(const Ticket &ticket, QWidget *parent)
    : QWidget(parent), ticket(ticket) {
    setupUI();
}

void TicketItem::setupUI() {
    layout = new QHBoxLayout(this);
    flightNumberLabel = new QLabel(QString::fromUtf8(ticket.getFlight()->getFlightName().c_str()), this);
    DateTime departureDateTime = ticket.getDepartureDateTime();
    DateTime arrivalDateTime = ticket.getArrivalDateTime();

    departureTimeLabel = new QLabel(departureDateTime.getTime().toString().c_str(), this);
    arrivalTimeLabel = new QLabel(arrivalDateTime.getTime().toString().c_str(), this);
    QString arrivalInfo = "";
    if (arrivalDateTime.getDate() > departureDateTime.getDate()) {
        int daysDifference = arrivalDateTime.getDate().getDay() - departureDateTime.getDate().getDay();
        arrivalInfo = QString(" (+%1)").arg(daysDifference);
    }
    arrivalTimeLabel->setText(arrivalTimeLabel->text() + arrivalInfo);
    priceLabel = new QLabel("价格: " + QString::number(1000), this);
    durationLabel = new QLabel("耗时: " + QString(ticket.getFlight()->getCostTime().toString().c_str()), this);
    bookButton = new QPushButton("预订", this);
    layout->addWidget(flightNumberLabel);
    layout->addWidget(departureTimeLabel);
    layout->addWidget(arrivalTimeLabel);
    layout->addWidget(durationLabel);
    layout->addWidget(priceLabel);
    layout->addWidget(bookButton);
    setLayout(layout);
}

QPushButton* TicketItem::getBookButton() const {
    return bookButton;
}

const Ticket& TicketItem::getTicket() const {
    return ticket;
}

ConnectingTicketItem::ConnectingTicketItem(const ConnectingTicket &ticket, QWidget *parent)
    : QWidget(parent), connectingTicket(ticket) {
    setupUI();
}

void ConnectingTicketItem::setupUI() {
    mainLayout = new QVBoxLayout(this);
    ticketsLayout = new QVBoxLayout();
    
    totalPriceLabel = new QLabel("总价格 " + QString::number(connectingTicket.getTotalPrice(EconomyClass)), this);
    totalDurationLabel = new QLabel("总时长: " + QString(connectingTicket.getTotalDuration().toString().c_str()), this);
    departureDateLabel = new QLabel("出发: " + QString(connectingTicket.getDepartureDateTime().toString().c_str()), this);
    arrivalDateLabel = new QLabel("到达: " + QString(connectingTicket.getArrivalDateTime().toString().c_str()), this);

    mainLayout->addWidget(totalPriceLabel);
    mainLayout->addWidget(totalDurationLabel);
    mainLayout->addWidget(departureDateLabel);
    mainLayout->addWidget(arrivalDateLabel);

    for (int i = 0; i < connectingTicket.getNumberOfTickets(); ++i) {
        addTicketDetails(connectingTicket.getTickets().getHead()->getElement());
    }

    bookButton = new QPushButton("预订", this);
    mainLayout->addWidget(bookButton);
    setLayout(mainLayout);
}

void ConnectingTicketItem::addTicketDetails(const Ticket &ticket) {
    QLabel *ticketLabel = new QLabel(ticket.getFlight()->getFlightName().c_str(), this);
    ticketsLayout->addWidget(ticketLabel);
}

QPushButton* ConnectingTicketItem::getBookButton() const {
    return bookButton;
}

const ConnectingTicket& ConnectingTicketItem::getConnectingTicket() const {
    return connectingTicket;
}