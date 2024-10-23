#include <ui/client/TicketBookingWindow.h>
#include <file/fileManage.h>
#include <QHBoxLayout>
#include <QListWidgetItem>
#include <QDebug>
#include <QStringList>
#include <QRadioButton>

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
    connect(flightListWidget, &QListWidget::itemClicked, this, &TicketBookingWindow::onFlightItemClicked);
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
    TicketItem *item = new TicketItem(ticket);
    QListWidgetItem *listItem = new QListWidgetItem(flightListWidget);
    listItem->setSizeHint(item->sizeHint());
    flightListWidget->addItem(listItem);
    flightListWidget->setItemWidget(listItem, item);
}

void TicketBookingWindow::addConnectingTicketItem(const ConnectingTicket &connectingTicket) {
    ConnectingTicketItem *item = new ConnectingTicketItem(connectingTicket);
    QListWidgetItem *listItem = new QListWidgetItem(flightListWidget);
    listItem->setSizeHint(item->sizeHint());
    flightListWidget->addItem(listItem);
    flightListWidget->setItemWidget(listItem, item);
}

void TicketBookingWindow::onFlightItemClicked(QListWidgetItem *item) {
    QWidget *widget = flightListWidget->itemWidget(item);
    if (TicketItem *ticketItem = qobject_cast<TicketItem *>(widget)) {
        onTicketItemClicked(ticketItem->getTicket());
    } else if (ConnectingTicketItem *connectingTicketItem = qobject_cast<ConnectingTicketItem *>(widget)) {
        onConnectingTicketItemClicked(connectingTicketItem->getConnectingTicket());
    }
}

void TicketBookingWindow::onTicketItemClicked(const Ticket &ticket) {
    QDialog *detailWindow = new QDialog(this);
    detailWindow->setWindowTitle("航班详情");
    detailWindow->setMinimumSize(400, 300);
    QVBoxLayout *layout = new QVBoxLayout(detailWindow);

    QLabel *flightNumberLabel = new QLabel("航班号: " + QString::fromUtf8(ticket.getFlight()->getFlightName().c_str()), detailWindow);
    QLabel *departureTimeLabel = new QLabel("出发时间: " + QString(ticket.getDepartureDateTime().toString().c_str()), detailWindow);
    QLabel *arrivalTimeLabel = new QLabel("到达时间: " + QString(ticket.getArrivalDateTime().toString().c_str()), detailWindow);

    layout->addWidget(flightNumberLabel);
    layout->addWidget(departureTimeLabel);
    layout->addWidget(arrivalTimeLabel);

    QGridLayout *cabinLayout = new QGridLayout;
    cabinLayout->addWidget(new QLabel("仓位", detailWindow), 0, 0);
    cabinLayout->addWidget(new QLabel("余票", detailWindow), 0, 1);
    cabinLayout->addWidget(new QLabel("价格", detailWindow), 0, 2);
    cabinLayout->addWidget(new QLabel("折扣", detailWindow), 0, 3);
    cabinLayout->addWidget(new QLabel("", detailWindow), 0, 4);

    QLabel *economyClassLabel = new QLabel("经济舱", detailWindow);
    int economyTickets = ticket.getFlightTicketDetail()->getRemainingTickets(EconomyClass);
    QLabel *economyTicketsLabel = new QLabel(QString::number(economyTickets), detailWindow);
    double economyPrice = ticket.getFlightTicketDetail()->getCabinPrice(EconomyClass);
    QLabel *economyPriceLabel = new QLabel(QString::number(economyPrice) + " 元", detailWindow);
    QLabel *economyDiscountLabel = new QLabel("9折", detailWindow);
    QPushButton *economyBookButton = new QPushButton("订购", detailWindow);
    if (economyTickets == 0) {
        economyBookButton->setEnabled(false);
    }

    cabinLayout->addWidget(economyClassLabel, 1, 0);
    cabinLayout->addWidget(economyTicketsLabel, 1, 1);
    cabinLayout->addWidget(economyPriceLabel, 1, 2);
    cabinLayout->addWidget(economyDiscountLabel, 1, 3);
    cabinLayout->addWidget(economyBookButton, 1, 4);

    QLabel *businessClassLabel = new QLabel("商务舱", detailWindow);
    int businessTickets = ticket.getFlightTicketDetail()->getRemainingTickets(BusinessClass);
    QLabel *businessTicketsLabel = new QLabel(QString::number(businessTickets), detailWindow);
    double businessPrice = ticket.getFlightTicketDetail()->getCabinPrice(BusinessClass);
    QLabel *businessPriceLabel = new QLabel(QString::number(businessPrice) + " 元", detailWindow);
    QLabel *businessDiscountLabel = new QLabel("8折", detailWindow);
    QPushButton *businessBookButton = new QPushButton("订购", detailWindow);
    if (businessTickets == 0) {
        businessBookButton->setEnabled(false);
    }

    cabinLayout->addWidget(businessClassLabel, 2, 0);
    cabinLayout->addWidget(businessTicketsLabel, 2, 1);
    cabinLayout->addWidget(businessPriceLabel, 2, 2);
    cabinLayout->addWidget(businessDiscountLabel, 2, 3);
    cabinLayout->addWidget(businessBookButton, 2, 4);

    QLabel *firstClassLabel = new QLabel("头等舱", detailWindow);
    int firstClassTickets = ticket.getFlightTicketDetail()->getRemainingTickets(FirstClass);
    QLabel *firstClassTicketsLabel = new QLabel(QString::number(firstClassTickets), detailWindow);
    double firstClassPrice = ticket.getFlightTicketDetail()->getCabinPrice(FirstClass);
    QLabel *firstClassPriceLabel = new QLabel(QString::number(firstClassPrice) + " 元", detailWindow);
    QLabel *firstClassDiscountLabel = new QLabel("7折", detailWindow);
    QPushButton *firstClassBookButton = new QPushButton("订购", detailWindow);
    if (firstClassTickets == 0) {
        firstClassBookButton->setEnabled(false);
    }

    cabinLayout->addWidget(firstClassLabel, 3, 0);
    cabinLayout->addWidget(firstClassTicketsLabel, 3, 1);
    cabinLayout->addWidget(firstClassPriceLabel, 3, 2);
    cabinLayout->addWidget(firstClassDiscountLabel, 3, 3);
    cabinLayout->addWidget(firstClassBookButton, 3, 4);

    layout->addLayout(cabinLayout);
    detailWindow->setLayout(layout);
    detailWindow->show();
}


void TicketBookingWindow::onConnectingTicketItemClicked(const ConnectingTicket &connectingTicket) {
    QDialog *detailWindow = new QDialog(this);
    detailWindow->setWindowTitle("联程航班详情");
    detailWindow->setMinimumSize(600, 400);

    QVBoxLayout *mainLayout = new QVBoxLayout(detailWindow);

    // 遍历所有航段，逐个显示每程的详细信息
    for (int i = 0; i < connectingTicket.getNumberOfTickets(); ++i) {
        const Ticket &ticket = connectingTicket.getTickets().getHead()->getElement();

        QLabel *flightInfoLabel = new QLabel(QString("第%1程").arg(i + 1), detailWindow);
        flightInfoLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
        mainLayout->addWidget(flightInfoLabel);

        // 显示舱位信息
        QGridLayout *ticketLayout = new QGridLayout();
        ticketLayout->addWidget(new QLabel("仓位", detailWindow), 0, 0);
        ticketLayout->addWidget(new QLabel("余票", detailWindow), 0, 1);
        ticketLayout->addWidget(new QLabel("价格", detailWindow), 0, 2);
        ticketLayout->addWidget(new QLabel("折扣", detailWindow), 0, 3);
        ticketLayout->addWidget(new QLabel("选择", detailWindow), 0, 4);

        // 经济舱
        QLabel *economyLabel = new QLabel("经济舱", detailWindow);
        QLabel *economyTicketsLabel = new QLabel(QString::number(ticket.getFlightTicketDetail()->getRemainingTickets(EconomyClass)), detailWindow);
        QLabel *economyPriceLabel = new QLabel(QString::number(ticket.getFlightTicketDetail()->getCabinPrice(EconomyClass)), detailWindow);
        QLabel *economyDiscountLabel = new QLabel("9折", detailWindow);  // 假设折扣为9折，可以根据需要调整
        QRadioButton *economySelectButton = new QRadioButton(detailWindow);

        ticketLayout->addWidget(economyLabel, 1, 0);
        ticketLayout->addWidget(economyTicketsLabel, 1, 1);
        ticketLayout->addWidget(economyPriceLabel, 1, 2);
        ticketLayout->addWidget(economyDiscountLabel, 1, 3);
        ticketLayout->addWidget(economySelectButton, 1, 4);

        // 商务舱
        QLabel *businessLabel = new QLabel("商务舱", detailWindow);
        QLabel *businessTicketsLabel = new QLabel(QString::number(ticket.getFlightTicketDetail()->getRemainingTickets(BusinessClass)), detailWindow);
        QLabel *businessPriceLabel = new QLabel(QString::number(ticket.getFlightTicketDetail()->getCabinPrice(BusinessClass)), detailWindow);
        QLabel *businessDiscountLabel = new QLabel("8折", detailWindow);  // 假设折扣为8折，可以根据需要调整
        QRadioButton *businessSelectButton = new QRadioButton(detailWindow);

        ticketLayout->addWidget(businessLabel, 2, 0);
        ticketLayout->addWidget(businessTicketsLabel, 2, 1);
        ticketLayout->addWidget(businessPriceLabel, 2, 2);
        ticketLayout->addWidget(businessDiscountLabel, 2, 3);
        ticketLayout->addWidget(businessSelectButton, 2, 4);

        // 头等舱
        QLabel *firstClassLabel = new QLabel("头等舱", detailWindow);
        QLabel *firstClassTicketsLabel = new QLabel(QString::number(ticket.getFlightTicketDetail()->getRemainingTickets(FirstClass)), detailWindow);
        QLabel *firstClassPriceLabel = new QLabel(QString::number(ticket.getFlightTicketDetail()->getCabinPrice(FirstClass)), detailWindow);
        QLabel *firstClassDiscountLabel = new QLabel("7折", detailWindow);
        QRadioButton *firstClassSelectButton = new QRadioButton(detailWindow);

        ticketLayout->addWidget(firstClassLabel, 3, 0);
        ticketLayout->addWidget(firstClassTicketsLabel, 3, 1);
        ticketLayout->addWidget(firstClassPriceLabel, 3, 2);
        ticketLayout->addWidget(firstClassDiscountLabel, 3, 3);
        ticketLayout->addWidget(firstClassSelectButton, 3, 4);

        mainLayout->addLayout(ticketLayout);
    }

    // 确认订购按钮
    QPushButton *confirmButton = new QPushButton("确认订购", detailWindow);
    mainLayout->addWidget(confirmButton, 0, Qt::AlignCenter);

    connect(confirmButton, &QPushButton::clicked, detailWindow, &QDialog::accept);

    detailWindow->setLayout(mainLayout);
    detailWindow->exec();
}


TicketItem::TicketItem(const Ticket &ticket, QWidget *parent)
    : QWidget(parent), ticket(ticket) {
        setupUI();
}

const Ticket& TicketItem::getTicket() const {
    return ticket;
}

void TicketItem::setupUI() {
    QHBoxLayout *layout = new QHBoxLayout(this);
    
    QLabel *departureTimeLabel = new QLabel(ticket.getDepartureDateTime().getTime().toString().c_str(), this);
    QLabel *arrivalTimeLabel = new QLabel(ticket.getArrivalDateTime().getTime().toString().c_str(), this);
    QLabel *departureAirportLabel = new QLabel(ticket.getFlight()->getDepartureAirport().getName().c_str(), this);
    QLabel *arrivalAirportLabel = new QLabel(ticket.getFlight()->getArrivalAirport().getName().c_str(), this);
    QLabel *priceLabel = new QLabel(QString("¥%1 起").arg(ticket.getFlightTicketDetail()->getCabinPrice(EconomyClass)), this);

    QString arrivalInfo = "";
    if (ticket.getArrivalDateTime().getDate() > ticket.getDepartureDateTime().getDate()) {
        int daysDifference = ticket.getArrivalDateTime().getDate().getDay() - ticket.getDepartureDateTime().getDate().getDay();
        arrivalInfo = QString(" (+%1天)").arg(daysDifference);
    }
    arrivalTimeLabel->setText(arrivalTimeLabel->text() + arrivalInfo);
    
    layout->addWidget(departureTimeLabel);
    layout->addWidget(departureAirportLabel);
    layout->addWidget(arrivalTimeLabel);
    layout->addWidget(arrivalAirportLabel);
    layout->addWidget(priceLabel);
    setLayout(layout);
}

// ConnectingTicketItem 实现
ConnectingTicketItem::ConnectingTicketItem(const ConnectingTicket &ticket, QWidget *parent)
    : QWidget(parent), connectingTicket(ticket) {
        setupUI();
}

const ConnectingTicket& ConnectingTicketItem::getConnectingTicket() const {
    return connectingTicket;
}

void ConnectingTicketItem::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QLabel *departureLabel = new QLabel(QString("出发: %1").arg(connectingTicket.getDepartureDateTime().toString().c_str()), this);
    QLabel *arrivalLabel = new QLabel(QString("到达: %1").arg(connectingTicket.getArrivalDateTime().toString().c_str()), this);
    QLabel *totalPriceLabel = new QLabel(QString("总价格: ¥%1 起").arg(connectingTicket.getTotalPrice(EconomyClass)), this);

    mainLayout->addWidget(departureLabel);
    mainLayout->addWidget(arrivalLabel);
    mainLayout->addWidget(totalPriceLabel);
    setLayout(mainLayout);
}