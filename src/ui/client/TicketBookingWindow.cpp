#include <ui/client/TicketBookingWindow.h>
#include <ui/client/sort.h>
#include <file/fileManage.h>
#include <QHBoxLayout>
#include <QListWidgetItem>
#include <QDebug>
#include <QStringList>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLineEdit>
#include <QMessageBox>
#include <QScrollArea>

static enum SortRule sortRule = NO_RULE;


static std::function<bool(const Ticket&, const Ticket&)> ticketSortFunction;
static std::function<bool(const ConnectingTicket&, const ConnectingTicket&)> connectingTicketSortFunction;


TicketBookingWindow::TicketBookingWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupConnections();
    populateCityComboBoxes();
}

void TicketBookingWindow::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    mainLayout->addLayout(createSelectionLayout());
    mainLayout->addWidget(createFlightListWidget());

    setLayout(mainLayout);
}

QHBoxLayout* TicketBookingWindow::createSelectionLayout() {
    QHBoxLayout *selectionLayout = new QHBoxLayout();

    QVBoxLayout *departureLayout = new QVBoxLayout();
    QLabel *departureLabel = new QLabel("出发城市:", this);
    departureCityComboBox = new QComboBox(this);
    setupComboBox(departureCityComboBox);
    departureLayout->addWidget(departureLabel);
    departureLayout->addWidget(departureCityComboBox);

    QVBoxLayout *arrivalLayout = new QVBoxLayout();
    QLabel *arrivalLabel = new QLabel("到达城市:", this);
    arrivalCityComboBox = new QComboBox(this);
    setupComboBox(arrivalCityComboBox);
    arrivalLayout->addWidget(arrivalLabel);
    arrivalLayout->addWidget(arrivalCityComboBox);

    QVBoxLayout *dateLayout = new QVBoxLayout();
    QLabel *dateLabel = new QLabel("出发日期:", this);
    dateEdit = new QDateEdit(QDate::currentDate(), this);
    setupDateEdit(dateEdit);
    dateLayout->addWidget(dateLabel);
    dateLayout->addWidget(dateEdit);

    searchButton = new QPushButton("搜索", this);
    searchButton->setMinimumHeight(40);
    searchButton->setFixedWidth(120);

    QVBoxLayout *filterLayout = new QVBoxLayout();
    QLabel *filterLabel = new QLabel("排序选项:", this);
    filterComboBox = new QComboBox(this);
    filterComboBox->addItems({"默认", "出发最早", "到达最早", "耗时最短", "价格最低"});
    filterLayout->addWidget(filterLabel);
    filterLayout->addWidget(filterComboBox);

    selectionLayout->addLayout(departureLayout);
    selectionLayout->addLayout(arrivalLayout);
    selectionLayout->addLayout(dateLayout);
    selectionLayout->addStretch();
    selectionLayout->addWidget(searchButton);
    selectionLayout->addLayout(filterLayout);

    return selectionLayout;
}

void TicketBookingWindow::setupComboBox(QComboBox *comboBox) {
    comboBox->setMinimumHeight(30);
    comboBox->setStyleSheet("font-size: 16px;");
}

void TicketBookingWindow::setupDateEdit(QDateEdit *dateEdit) {
    dateEdit->setCalendarPopup(true);
    dateEdit->setMinimumHeight(30);
    dateEdit->setStyleSheet("font-size: 16px;");
}

QListWidget* TicketBookingWindow::createFlightListWidget() {
    flightListWidget = new QListWidget(this);
    flightListWidget->setStyleSheet(
        "QListWidget {"
        "   font-size: 16px;"
        "}"
        "QListWidget::item {"
        "   background-color: lightblue;"
        "   border: 1px solid lightblue;"
        "   border-radius: 10px;"
        "   padding: 1px;"
        "}"
    );

    flightListWidget->setSpacing(5);
    flightListWidget->setUniformItemSizes(false);
    flightListWidget->setSelectionMode(QAbstractItemView::NoSelection);
    flightListWidget->setFocusPolicy(Qt::NoFocus);
    


    return flightListWidget;
}

void TicketBookingWindow::setupConnections() {
    connect(searchButton, &QPushButton::clicked, this, &TicketBookingWindow::onSearchButtonClicked);
    connect(flightListWidget, &QListWidget::itemClicked, this, &TicketBookingWindow::onFlightItemClicked);
}

void TicketBookingWindow::populateCityComboBoxes() {
    flight_network.traverseCities([this](const String &city, const CityInfo& cityInfo) {
        if (cityInfo.isAbroad) return;
        QString cityName = QString::fromUtf8(city.c_str());
        departureCityComboBox->addItem(cityName);
        arrivalCityComboBox->addItem(cityName);
    });

    flight_network.traverseCities([this](const String &city, const CityInfo& cityInfo) {
        if (!cityInfo.isAbroad) return;
        QString cityName = QString::fromUtf8(city.c_str());
        departureCityComboBox->addItem(cityName);
        arrivalCityComboBox->addItem(cityName);
    });
}

void TicketBookingWindow::onSearchButtonClicked() {
    QString selectedFilter = filterComboBox->currentText();
    switch (filterComboBox->currentIndex()) {
        case 0:
            sortRule = NO_RULE;
            break;
        case 1:
            sortRule = EARLIEST_DEPARTURE;
            break;
        case 2:
            sortRule = EARLIEST_ARRIVAL;
            break;
        case 3:
            sortRule = SHORTEST_DURATION;
            break;
        case 4:
            sortRule = CHEAPEST_PRICE;
            break;
        default:
            sortRule = NO_RULE;
            break;
    }
    
    ticketSortFunction = getSortFunction<Ticket>(sortRule);
    connectingTicketSortFunction = getSortFunction<ConnectingTicket>(sortRule);

    populateFlightList();
}

void TicketBookingWindow::populateFlightList() {
    flightListWidget->clear();

    String departureCity = getSelectedCity(departureCityComboBox);
    String arrivalCity = getSelectedCity(arrivalCityComboBox);
    Date selectedDate = getSelectedDate();

    if (!validateCities(departureCity, arrivalCity)) {
        QMessageBox::warning(this, "错误", "出发城市和到达城市不能相同");
        return;
    }

    queryDirectFlights(departureCity, arrivalCity, selectedDate);
    queryConnectingFlights(departureCity, arrivalCity, selectedDate);

    if (flightListWidget->count() == 0) {
        QMessageBox::information(this, "无航班", "未查到符合条件的航班");
    }
}

String TicketBookingWindow::getSelectedCity(QComboBox *comboBox) {
    return String(comboBox->currentText().toStdString().c_str());
}

Date TicketBookingWindow::getSelectedDate() {
    QString qDateStr = dateEdit->date().toString("yyyy-MM-dd");
    return Date::fromString(qDateStr.toStdString().c_str());
}

bool TicketBookingWindow::validateCities(const String &departureCity, const String &arrivalCity) {
    return !(departureCity == arrivalCity);
}

void TicketBookingWindow::queryDirectFlights(const String &departureCity, const String &arrivalCity, const Date &date) {
    LinkedList<Ticket> directFlights = std::move(flight_network.findDirectFlights(departureCity, arrivalCity, date));
    directFlights.sort(ticketSortFunction);
    directFlights.traverse([this](const Ticket& ticket) {
        addTicketItem(ticket);
    });
}

void TicketBookingWindow::queryConnectingFlights(const String &departureCity, const String &arrivalCity, const Date &date) {
    LinkedList<ConnectingTicket> connectingFlights = std::move(flight_network.findConnectingFlights(departureCity, arrivalCity, date, 2));
    connectingFlights.sort(connectingTicketSortFunction);
    connectingFlights.traverse([this](const ConnectingTicket& connectingTicket) {
        addConnectingTicketItem(connectingTicket);
    });
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
        onOrdertItemClicked(ticketItem->getTicket());
    } else if (ConnectingTicketItem *connectingTicketItem = qobject_cast<ConnectingTicketItem *>(widget)) {
        onConnectingTicketItemClicked(connectingTicketItem->getConnectingTicket());
    }
}

void TicketBookingWindow::onOrdertItemClicked(Ticket &ticket) {
    QDialog *detailWindow = createDetailWindow("航班详情");

    QVBoxLayout *layout = static_cast<QVBoxLayout*>(detailWindow->property("contentLayout").value<void*>());
    layout->addWidget(createFlightInfoLabels(ticket, detailWindow));
    layout->addLayout(createCabinSelectionLayout(ticket, detailWindow));

    detailWindow->exec();
}

void TicketBookingWindow::onConnectingTicketItemClicked(ConnectingTicket &connectingTicket) {
    QDialog *detailWindow = createDetailWindow("联程航班详情");

    QVBoxLayout *mainLayout = static_cast<QVBoxLayout*>(detailWindow->property("contentLayout").value<void*>());

    int segmentCount = connectingTicket.getTickets().size();

    CabinType *selectedCabins = new CabinType[segmentCount];
    const Ticket **tickets = new const Ticket*[segmentCount];

    for (int i = 0; i < segmentCount; ++i) {
        selectedCabins[i] = CabinType::None;
    }

    for (int i = 0; i < segmentCount; ++i) {
        Ticket &ticket = connectingTicket.getTickets().getElementAt(i);
        tickets[i] = &ticket;

        mainLayout->addWidget(createConnectingFlightInfoLabel(i + 1, detailWindow));
        mainLayout->addWidget(createFlightInfoLabels(ticket, detailWindow));
        mainLayout->addLayout(createCabinSelectionLayout(ticket, i, detailWindow, selectedCabins));
    }

    QPushButton *confirmButton = new QPushButton("确认订购", detailWindow);
    mainLayout->addWidget(confirmButton, 0, Qt::AlignCenter);

    connect(confirmButton, &QPushButton::clicked, this, [=]() {
        bool allSelected = true;
        for (int i = 0; i < segmentCount; ++i) {
            if (selectedCabins[i] == CabinType::None) {
                allSelected = false;
                break;
            }
        }

        if (allSelected) {
            handleConnectingTicketPurchase(tickets, selectedCabins, segmentCount, detailWindow);
        } else {
            QMessageBox::warning(detailWindow, "错误", "请为每一程选择舱位");
        }
    });

    detailWindow->exec();

    delete[] selectedCabins;
    delete[] tickets;
}

QDialog* TicketBookingWindow::createDetailWindow(const QString &title) {
    QDialog *detailWindow = new QDialog(this);
    detailWindow->setWindowTitle(title);
    detailWindow->setMinimumSize(600, 400);

    QScrollArea *scrollArea = new QScrollArea(detailWindow);
    scrollArea->setWidgetResizable(true);

    QWidget *contentWidget = new QWidget();
    scrollArea->setWidget(contentWidget);

    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(detailWindow);
    mainLayout->addWidget(scrollArea);

    detailWindow->setProperty("contentLayout", QVariant::fromValue(static_cast<void*>(contentLayout)));

    return detailWindow;
}

QWidget* TicketBookingWindow::createFlightInfoLabels(const Ticket &ticket, QWidget *parent) {
    QWidget *widget = new QWidget(parent);
    QVBoxLayout *layout = new QVBoxLayout(widget);

    QLabel *flightNumberLabel = new QLabel("航班号: " + QString::fromUtf8(ticket.getFlight()->getFlightName().c_str()), parent);
    QLabel *airlineLabel = new QLabel("航空公司: " + QString::fromUtf8(ticket.getFlight()->getAirline().c_str()), parent);
    QLabel *airplaneModelLabel = new QLabel("飞机型号: " + QString::fromUtf8(ticket.getFlight()->getAirplaneModel().c_str()), parent);
    QLabel *departureAirportLabel = new QLabel("出发机场: " + QString::fromUtf8(ticket.getFlight()->getDepartureAirport().getName().c_str()), parent);
    QLabel *arrivalAirportLabel = new QLabel("到达机场: " + QString::fromUtf8(ticket.getFlight()->getArrivalAirport().getName().c_str()), parent);
    QLabel *departureTimeLabel = new QLabel("出发时间: " + QString(ticket.getDepartureDateTime().toString().c_str()), parent);
    QLabel *arrivalTimeLabel = new QLabel("到达时间: " + QString(ticket.getArrivalDateTime().toString().c_str()), parent);

    layout->addWidget(flightNumberLabel);
    layout->addWidget(airlineLabel);
    layout->addWidget(airplaneModelLabel);
    layout->addWidget(departureAirportLabel);
    layout->addWidget(arrivalAirportLabel);
    layout->addWidget(departureTimeLabel);
    layout->addWidget(arrivalTimeLabel);

    return widget;
}

QLabel* TicketBookingWindow::createConnectingFlightInfoLabel(int segmentNumber, QWidget *parent) {
    QLabel *flightInfoLabel = new QLabel(QString("第%1程").arg(segmentNumber), parent);
    flightInfoLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
    return flightInfoLabel;
}

QGridLayout* TicketBookingWindow::createCabinSelectionLayout(const Ticket &ticket, QWidget *parent) {
    QGridLayout *cabinLayout = new QGridLayout();

    addCabinLayoutHeaders(cabinLayout, parent);

    QPushButton *economyBookButton = nullptr;
    double economyDiscountValue = ticket.getCabinDiscount(EconomyClass);
    QString economyDiscount = (economyDiscountValue == 10.0) ? "无" : QString::number(economyDiscountValue, 'f', 1) + "折";
    addCabinRow(cabinLayout, ticket, "经济舱", EconomyClass, economyDiscount, economyBookButton, parent);

    QPushButton *businessBookButton = nullptr;
    double businessDiscountValue = ticket.getCabinDiscount(BusinessClass);
    QString businessDiscount = (businessDiscountValue == 10.0) ? "无" : QString::number(businessDiscountValue, 'f', 1) + "折";
    addCabinRow(cabinLayout, ticket, "商务舱", BusinessClass, businessDiscount, businessBookButton, parent);

    QPushButton *firstClassBookButton = nullptr;
    double firstClassDiscountValue = ticket.getCabinDiscount(FirstClass);
    QString firstClassDiscount = (firstClassDiscountValue == 10.0) ? "无" : QString::number(firstClassDiscountValue, 'f', 1) + "折";
    addCabinRow(cabinLayout, ticket, "头等舱", FirstClass, firstClassDiscount, firstClassBookButton, parent);

    return cabinLayout;
}

QGridLayout* TicketBookingWindow::createCabinSelectionLayout(const Ticket &ticket, int segmentIndex, QWidget *parent, CabinType *selectedCabins) {
    QGridLayout *ticketLayout = new QGridLayout();

    addCabinLayoutHeaders(ticketLayout, parent);

    QButtonGroup *cabinGroup = new QButtonGroup(parent);

    int row = 1;

    if (ticket.getFlightTicketDetail()->getRemainingTickets(EconomyClass) > 0) {
        QLabel *economyLabel = new QLabel("经济舱", parent);
        QLabel *economyTicketsLabel = new QLabel(QString::number(ticket.getFlightTicketDetail()->getRemainingTickets(EconomyClass)), parent);
        QLabel *economyPriceLabel = new QLabel(QString::number(ticket.getFlightTicketDetail()->getCabinPrice(EconomyClass)), parent);
        
        double economyDiscountValue = ticket.getCabinDiscount(EconomyClass);
        QString economyDiscount = (economyDiscountValue == 10.0) ? "无" : QString::number(economyDiscountValue, 'f', 1) + "折";
        QLabel *economyDiscountLabel = new QLabel(economyDiscount, parent);
        QRadioButton *economySelectButton = new QRadioButton(parent);

        ticketLayout->addWidget(economyLabel, row, 0);
        ticketLayout->addWidget(economyTicketsLabel, row, 1);
        ticketLayout->addWidget(economyPriceLabel, row, 2);
        ticketLayout->addWidget(economyDiscountLabel, row, 3);
        ticketLayout->addWidget(economySelectButton, row, 4);

        cabinGroup->addButton(economySelectButton, static_cast<int>(EconomyClass));

        row++;
    }

    if (ticket.getFlightTicketDetail()->getRemainingTickets(BusinessClass) > 0) {
        QLabel *businessLabel = new QLabel("商务舱", parent);
        QLabel *businessTicketsLabel = new QLabel(QString::number(ticket.getFlightTicketDetail()->getRemainingTickets(BusinessClass)), parent);
        QLabel *businessPriceLabel = new QLabel(QString::number(ticket.getFlightTicketDetail()->getCabinPrice(BusinessClass)), parent);
        
        double businessDiscountValue = ticket.getCabinDiscount(BusinessClass);
        QString businessDiscount = (businessDiscountValue == 10.0) ? "无" : QString::number(businessDiscountValue, 'f', 1) + "折";
        QLabel* bussinessDiscountLabel = new QLabel(businessDiscount, parent);
        QRadioButton *businessSelectButton = new QRadioButton(parent);

        ticketLayout->addWidget(businessLabel, row, 0);
        ticketLayout->addWidget(businessTicketsLabel, row, 1);
        ticketLayout->addWidget(businessPriceLabel, row, 2);
        ticketLayout->addWidget(bussinessDiscountLabel, row, 3);
        ticketLayout->addWidget(businessSelectButton, row, 4);

        cabinGroup->addButton(businessSelectButton, static_cast<int>(BusinessClass));

        row++;
    }

    if (ticket.getFlightTicketDetail()->getRemainingTickets(FirstClass) > 0) {
        QLabel *firstClassLabel = new QLabel("头等舱", parent);
        QLabel *firstClassTicketsLabel = new QLabel(QString::number(ticket.getFlightTicketDetail()->getRemainingTickets(FirstClass)), parent);
        QLabel *firstClassPriceLabel = new QLabel(QString::number(ticket.getFlightTicketDetail()->getCabinPrice(FirstClass)), parent);
        
        double firstClassDiscountValue = ticket.getCabinDiscount(FirstClass);
        QString firstClassDiscount = (firstClassDiscountValue == 10.0) ? "无" : QString::number(firstClassDiscountValue, 'f', 1) + "折";
        QLabel* firstClassDiscountLabel = new QLabel(firstClassDiscount, parent);
        QRadioButton *firstClassSelectButton = new QRadioButton(parent);

        ticketLayout->addWidget(firstClassLabel, row, 0);
        ticketLayout->addWidget(firstClassTicketsLabel, row, 1);
        ticketLayout->addWidget(firstClassPriceLabel, row, 2);
        ticketLayout->addWidget(firstClassDiscountLabel, row, 3);
        ticketLayout->addWidget(firstClassSelectButton, row, 4);

        cabinGroup->addButton(firstClassSelectButton, static_cast<int>(FirstClass));

        row++;
    }

    if (cabinGroup->buttons().isEmpty()) {
        QLabel *noCabinLabel = new QLabel("无可用舱位", parent);
        ticketLayout->addWidget(noCabinLabel, row, 0, 1, 4);
    } else {
        connect(cabinGroup, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::idClicked), this, [=](int id) {
            selectedCabins[segmentIndex] = static_cast<CabinType>(id);
        });
    }

    return ticketLayout;
}

void TicketBookingWindow::addCabinLayoutHeaders(QGridLayout *layout, QWidget *parent) {
    layout->addWidget(new QLabel("仓位", parent), 0, 0);
    layout->addWidget(new QLabel("余票", parent), 0, 1);
    layout->addWidget(new QLabel("价格", parent), 0, 2);
    layout->addWidget(new QLabel("折扣", parent), 0, 3);
}

void TicketBookingWindow::addCabinRow(QGridLayout *layout, const Ticket &ticket, const QString &cabinTypeStr, CabinType cabinType, const QString &discount, QPushButton *&bookButton, QWidget *parent) {
    int remainingTickets = ticket.getFlightTicketDetail()->getRemainingTickets(cabinType);
    double price = ticket.getFlightTicketDetail()->getCabinPrice(cabinType);

    QLabel *cabinTypeLabel = new QLabel(cabinTypeStr, parent);
    QLabel *ticketsLabel = new QLabel(QString::number(remainingTickets), parent);
    QLabel *priceLabel = new QLabel(QString::number(price) + " 元", parent);
    QLabel *discountLabel = new QLabel(discount, parent);
    bookButton = new QPushButton("订购", parent);
    if (remainingTickets == 0) {
        bookButton->setEnabled(false);
    }

    int row = layout->rowCount();
    layout->addWidget(cabinTypeLabel, row, 0);
    layout->addWidget(ticketsLabel, row, 1);
    layout->addWidget(priceLabel, row, 2);
    layout->addWidget(discountLabel, row, 3);
    layout->addWidget(bookButton, row, 4);

    connect(bookButton, &QPushButton::clicked, this, [=]() {
        handleTicketPurchase(ticket, cabinType, parent);
    });
}

void TicketBookingWindow::handleTicketPurchase(const Ticket &ticket, CabinType cabinType, QWidget *parent) {
    QDialog passengerInfoDialog(parent);
    passengerInfoDialog.setWindowTitle("乘车人信息");
    passengerInfoDialog.setMinimumSize(300, 200);
    QVBoxLayout layout(&passengerInfoDialog);

    QLineEdit nameInput(&passengerInfoDialog);
    nameInput.setPlaceholderText("请输入乘客姓名");
    layout.addWidget(&nameInput);

    QLineEdit idInput(&passengerInfoDialog);
    idInput.setPlaceholderText("请输入乘客身份证号");
    layout.addWidget(&idInput);

    QPushButton confirmPassengerButton("确认", &passengerInfoDialog);
    layout.addWidget(&confirmPassengerButton);

    connect(&confirmPassengerButton, &QPushButton::clicked, &passengerInfoDialog, [&]() {
        String user = current_login_user.getPhoneNumber();
        String name = nameInput.text().toStdString().c_str();
        String id = idInput.text().toStdString().c_str();
        Passenger passenger(name, id);
        Flight* flight = ticket.getFlight();
        FlightTicketDetail* ticketDetail = ticket.getFlightTicketDetail();
        Order order(flight->getFlightName(), flight->getairRoute(), user, passenger, cabinType,
                    ticketDetail->getCabinPrice(cabinType) ,ticketDetail->getFlightDate(), "NULL");

        QDialog orderDialog(&passengerInfoDialog);
        orderDialog.setWindowTitle("订单确认");
        orderDialog.setMinimumSize(300, 200);
        QVBoxLayout orderLayout(&orderDialog);

        QString cabinStr;

        switch(cabinType) {
            case FirstClass:
                cabinStr = "头等舱";
                break;
            case BusinessClass:
                cabinStr = "商务舱";
                break;
            case EconomyClass:
                cabinStr = "经济舱";
                break;
            default:
                cabinStr = "";
        }

        QLabel orderInfoLabel(QString("订单号: %1\n乘客: %2\n航班: %3\n舱位: %4\n总费用: %5")
                              .arg(order.getOrderNumber().c_str())
                              .arg(nameInput.text())
                              .arg(order.getFlightNumber().c_str())
                              .arg(cabinStr)
                              .arg(order.getPrice()), &orderDialog);
        orderLayout.addWidget(&orderInfoLabel);

        QPushButton payButton("支付", &orderDialog);
        orderLayout.addWidget(&payButton);

        connect(&payButton, &QPushButton::clicked, &orderDialog, [&]() {
            bool success = buyTicket(order);
            if (success) {
                QMessageBox::information(this, "订票成功", "订票成功！");
                orderDialog.accept();
                passengerInfoDialog.accept();
            } else {
                QMessageBox::warning(this, "订票失败", "订票失败，请重试");
            }
        });

        orderDialog.exec();
    });

    passengerInfoDialog.exec();
}

void TicketBookingWindow::handleConnectingTicketPurchase(const Ticket **tickets, const CabinType *selectedCabins, int segmentCount, QWidget *parent) {
    QDialog passengerInfoDialog(parent);
    passengerInfoDialog.setWindowTitle("乘车人信息");
    passengerInfoDialog.setMinimumSize(300, 200);
    QVBoxLayout layout(&passengerInfoDialog);

    QLineEdit nameInput(&passengerInfoDialog);
    nameInput.setPlaceholderText("请输入乘客姓名");
    layout.addWidget(&nameInput);

    QLineEdit idInput(&passengerInfoDialog);
    idInput.setPlaceholderText("请输入乘客身份证号");
    layout.addWidget(&idInput);

    QPushButton confirmPassengerButton("确认", &passengerInfoDialog);
    layout.addWidget(&confirmPassengerButton);

    connect(&confirmPassengerButton, &QPushButton::clicked, &passengerInfoDialog, [&]() {
        String user = current_login_user.getPhoneNumber();
        String name = nameInput.text().toStdString().c_str();
        String id = idInput.text().toStdString().c_str();
        Passenger passenger(name, id);

        QVector<Order> orders;
        double totalPrice = 0.0;

        for (int i = 0; i < segmentCount; ++i) {
            const Ticket *ticket = tickets[i];
            CabinType cabinType = selectedCabins[i];
            Flight *flight = ticket->getFlight();
            FlightTicketDetail *ticketDetail = ticket->getFlightTicketDetail();
            double price = ticketDetail->getCabinPrice(cabinType);

            // 创建订单
            Order order(flight->getFlightName(), flight->getairRoute(), user, passenger, cabinType,
                        price, ticketDetail->getFlightDate(), "NULL");
            orders.push_back(order);
            totalPrice += price;
        }

        // 显示订单页面
        QDialog orderDialog(&passengerInfoDialog);
        orderDialog.setWindowTitle("订单确认");
        orderDialog.setMinimumSize(400, 300);
        QVBoxLayout orderLayout(&orderDialog);

        for (int i = 0; i < orders.size(); ++i) {
            const Order &order = orders[i];
            QString cabinStr;
            switch(selectedCabins[i]) {
                case FirstClass:
                    cabinStr = "头等舱";
                    break;
                case BusinessClass:
                    cabinStr = "商务舱";
                    break;
                case EconomyClass:
                    cabinStr = "经济舱";
                    break;
                default:
                    cabinStr = "未知舱位";
            }

            QLabel *orderInfoLabel = new QLabel(QString("订单 %1:\n订单号: %2\n乘客: %3\n航班: %4\n舱位: %5\n费用: %6")
                                                .arg(i + 1)
                                                .arg(order.getOrderNumber().c_str())
                                                .arg(nameInput.text())
                                                .arg(order.getFlightNumber().c_str())
                                                .arg(cabinStr)
                                                .arg(order.getPrice()), &orderDialog);
            orderLayout.addWidget(orderInfoLabel);
        }

        QLabel totalLabel(QString("总费用: %1").arg(totalPrice), &orderDialog);
        orderLayout.addWidget(&totalLabel);

        QPushButton payButton("支付", &orderDialog);
        orderLayout.addWidget(&payButton);

        connect(&payButton, &QPushButton::clicked, &orderDialog, [&]() {
            bool success = true;
            for (const Order &order : orders) {
                if (!buyTicket(order)) {
                    success = false;
                    break;
                }
            }

            if (success) {
                QMessageBox::information(this, "订票成功", "联程航班订票成功！");
                orderDialog.accept();
                passengerInfoDialog.accept();
            } else {
                QMessageBox::warning(this, "订票失败", "订票失败，请重试！");
            }
        });

        orderDialog.exec();
    });

    passengerInfoDialog.exec();
}

// void TicketBookingWindow::handleConnectingTicketPurchase(const Ticket **tickets, const CabinType *selectedCabins, int segmentCount, QWidget *parent) {
//     QDialog passengerInfoDialog(parent);
//     passengerInfoDialog.setWindowTitle("乘车人信息");
//     passengerInfoDialog.setMinimumSize(300, 200);
//     QVBoxLayout layout(&passengerInfoDialog);

//     QLineEdit nameInput(&passengerInfoDialog);
//     nameInput.setPlaceholderText("请输入乘客姓名");
//     layout.addWidget(&nameInput);

//     QLineEdit idInput(&passengerInfoDialog);
//     idInput.setPlaceholderText("请输入乘客身份证号");
//     layout.addWidget(&idInput);

//     QPushButton confirmPassengerButton("确认", &passengerInfoDialog);
//     layout.addWidget(&confirmPassengerButton);

//     connect(&confirmPassengerButton, &QPushButton::clicked, &passengerInfoDialog, [&]() {
//         String user = current_login_user.getPhoneNumber();
//         String name = nameInput.text().toStdString().c_str();
//         String id = idInput.text().toStdString().c_str();
//         Passenger passenger(name, id);

//         bool success = true;

//         for (int i = 0; i < segmentCount; ++i) {
//             const Ticket *ticket = tickets[i];
//             CabinType cabinType = selectedCabins[i];

//             if (!buyTicket(*ticket, cabinType, passenger)) {
//                 success = false;
//                 break;
//             }
//         }

//         if (success) {
//             QMessageBox::information(this, "订票成功", "订票成功！");
//             passengerInfoDialog.accept();
//         } else {
//             QMessageBox::warning(this, "订票失败", "订票失败，请勿重复购票！");
//         }
//     });

//     passengerInfoDialog.exec();
// }

TicketItem::TicketItem(const Ticket &ticket, QWidget *parent)
    : QWidget(parent), ticket(ticket) {
    setupUI();
}

Ticket& TicketItem::getTicket() {
    return ticket;
}

void TicketItem::setupUI() {
    QHBoxLayout *layout = new QHBoxLayout(this);

    QLabel *flightNumberLabel = new QLabel(ticket.getFlight()->getFlightName().c_str(), this);
    QLabel *airlineLabel = new QLabel(ticket.getFlight()->getAirline().c_str(), this);
    QLabel *departureTimeLabel = new QLabel(ticket.getDepartureDateTime().getTime().toString().c_str(), this);
    QLabel *arrivalTimeLabel = new QLabel(ticket.getArrivalDateTime().getTime().toString().c_str(), this);
    QLabel *departureAirportLabel = new QLabel(ticket.getFlight()->getDepartureAirport().getName().c_str(), this);
    QLabel *arrivalAirportLabel = new QLabel(ticket.getFlight()->getArrivalAirport().getName().c_str(), this);
    QLabel *priceLabel = new QLabel(QString("¥%1 起").arg(ticket.getFlightTicketDetail()->getCabinPrice(EconomyClass)), this);

    QString arrivalInfo = "";
    int daysDifference = ticket.getArrivalDateTime().getDate().getDay() - ticket.getDepartureDateTime().getDate().getDay();
    if (daysDifference > 0)
        arrivalInfo = QString(" (+%1天)").arg(daysDifference);

    arrivalTimeLabel->setText(arrivalTimeLabel->text() + arrivalInfo);

    layout->addWidget(flightNumberLabel);
    layout->addWidget(airlineLabel);
    layout->addWidget(departureTimeLabel);
    layout->addWidget(departureAirportLabel);
    layout->addWidget(arrivalTimeLabel);
    layout->addWidget(arrivalAirportLabel);
    layout->addWidget(priceLabel);
    setLayout(layout);
}

ConnectingTicketItem::ConnectingTicketItem(const ConnectingTicket &ticket, QWidget *parent)
    : QWidget(parent), connectingTicket(ticket) {
    setupUI();
}

ConnectingTicket& ConnectingTicketItem::getConnectingTicket() {
    return connectingTicket;
}

void ConnectingTicketItem::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *headerLabel = new QLabel("联程航班详情", this);
    mainLayout->addWidget(headerLabel);

    int startDay = connectingTicket.getDepartureDateTime().getDate().getDay();
    for (int i = 0; i < connectingTicket.getTickets().size(); ++i) {
        const Ticket& ticket = connectingTicket.getTickets().getElementAt(i);

        QHBoxLayout *segmentLayout = new QHBoxLayout();

        QLabel *flightNumberLabel = new QLabel(ticket.getFlight()->getFlightName().c_str(), this);
        QLabel *airlineLabel = new QLabel(ticket.getFlight()->getAirline().c_str(), this);
        QLabel *departureTimeLabel = new QLabel(ticket.getDepartureDateTime().getTime().toString().c_str(), this);
        QLabel *arrivalTimeLabel = new QLabel(ticket.getArrivalDateTime().getTime().toString().c_str(), this);
        QLabel *departureAirportLabel = new QLabel(ticket.getFlight()->getDepartureAirport().getName().c_str(), this);
        QLabel *arrivalAirportLabel = new QLabel(ticket.getFlight()->getArrivalAirport().getName().c_str(), this);
        QLabel *priceLabel = new QLabel(QString("¥%1 起").arg(ticket.getFlightTicketDetail()->getCabinPrice(EconomyClass)), this);

        QString arrivalInfo = "";
        if (ticket.getArrivalDateTime().getDate() > ticket.getDepartureDateTime().getDate()) {
            int daysDifference = ticket.getArrivalDateTime().getDate().getDay() - startDay;
            arrivalInfo = QString(" (+%1天)").arg(daysDifference);
        }
        arrivalTimeLabel->setText(arrivalTimeLabel->text() + arrivalInfo);

        segmentLayout->addWidget(flightNumberLabel);
        segmentLayout->addWidget(airlineLabel);
        segmentLayout->addWidget(departureTimeLabel);
        segmentLayout->addWidget(departureAirportLabel);
        segmentLayout->addWidget(arrivalTimeLabel);
        segmentLayout->addWidget(arrivalAirportLabel);
        segmentLayout->addWidget(priceLabel);

        mainLayout->addLayout(segmentLayout);

        if (i < connectingTicket.getTickets().size() - 1) {
            const Ticket& nextTicket = connectingTicket.getTickets().getElementAt(i + 1);
            Time awaitTime = nextTicket.getDepartureDateTime() - ticket.getArrivalDateTime();
            QLabel *layoverLabel = new QLabel(QString("换乘等待: %1 小时 %2 分钟").arg(awaitTime.getHours())
                                                                               .arg(awaitTime.getMinutes())
                                                                               , this);
            mainLayout->addWidget(layoverLabel);
        }

        mainLayout->addSpacing(10);
    }

    Time  duration = connectingTicket.getDuration();
    QLabel *totalDurationLabel = new QLabel(QString("总时长: %1 小时 %2 分钟").arg(duration.getHours())
                                                                            .arg(duration.getMinutes())
                                                                            , this);
    QLabel *totalPriceLabel = new QLabel(QString("总价格: ¥%1 起").arg(connectingTicket.getTotalPrice(EconomyClass)), this);

    mainLayout->addWidget(totalDurationLabel);
    mainLayout->addWidget(totalPriceLabel);
    setLayout(mainLayout);
}
