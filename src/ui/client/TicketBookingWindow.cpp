#include <ui/client/TicketBookingWindow.h>
#include <file/fileManage.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QDialog>
#include <QScrollArea>
#include <QButtonGroup>
#include <QRegularExpression>
#include <QRadioButton>

enum SortRule {
    NO_RULE,
    EARLIEST_DEPARTURE,
    EARLIEST_ARRIVAL,
    SHORTEST_DURATION,
    CHEAPEST_PRICE,
    USER_PREFERENCE
};

static LinkedList<ConnectingTicket> connectingFlights;
static enum SortRule sortRule = NO_RULE;
static bool user_pred = false;

static std::function<bool(const ConnectingTicket&, const ConnectingTicket&)> getSortFunction(SortRule rule);
static std::function<bool(const ConnectingTicket&, const ConnectingTicket&)> sortFunction;

ConnectingTicketItem::ConnectingTicketItem(const ConnectingTicket &ticket, QWidget *parent)
    : QWidget(parent), connectingTicket(ticket) {
    setupUI();
}

const ConnectingTicket& ConnectingTicketItem::getConnectingTicket() const {
    return connectingTicket;
}

void ConnectingTicketItem::setupUI() {
    mainLayout = new QVBoxLayout(this);

    int segmentCount = connectingTicket.getNumberOfTickets();

    Date startDay = connectingTicket.getDepartureDateTime().getDate();
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
        int daysDifference = ticket.getArrivalDateTime().getDate() - startDay;
        if (daysDifference > 0)
            arrivalInfo = QString(" (+%1)").arg(daysDifference);

        arrivalTimeLabel->setText(arrivalTimeLabel->text() + arrivalInfo);

        segmentLayout->addWidget(flightNumberLabel);
        segmentLayout->addWidget(airlineLabel);
        segmentLayout->addWidget(departureTimeLabel);
        segmentLayout->addWidget(departureAirportLabel);
        segmentLayout->addWidget(arrivalTimeLabel);
        segmentLayout->addWidget(arrivalAirportLabel);
        segmentLayout->addWidget(priceLabel);

        mainLayout->addLayout(segmentLayout);

        if (segmentCount > 1 && i < segmentCount - 1) {
            const Ticket& nextTicket = connectingTicket.getTickets().getElementAt(i + 1);
            Time awaitTime = nextTicket.getDepartureDateTime() - ticket.getArrivalDateTime();
            QLabel *layoverLabel = new QLabel(QString("换乘等待: %1 小时 %2 分钟").arg(awaitTime.getHours())
                                                                            .arg(awaitTime.getMinutes())
                                                                            , this);
            mainLayout->addWidget(layoverLabel);
        }

        mainLayout->addSpacing(10);
    }

    if (segmentCount > 1) {
        Time  duration = connectingTicket.getDuration();
        QLabel *totalDurationLabel = new QLabel(QString("总时长: %1 小时 %2 分钟").arg(duration.getHours())
                                                                                .arg(duration.getMinutes())
                                                                                , this);
        QLabel *totalPriceLabel = new QLabel(QString("总价格: ¥%1 起").arg(connectingTicket.getTotalPrice(EconomyClass)), this);

        mainLayout->addWidget(totalDurationLabel);
        mainLayout->addWidget(totalPriceLabel);
    }
        

    setLayout(mainLayout);
}


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
    filterComboBox->addItems({"默认", "出发最早", "到达最早", "耗时最短", "价格最低", "用户偏好"});
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
    flight_network.traverseCities([this](const String &city, const CityInfo &cityInfo) {
        if (cityInfo.isAbroad) return;
        QString cityName = QString::fromUtf8(city.c_str());
        departureCityComboBox->addItem(cityName);
        arrivalCityComboBox->addItem(cityName);
    });
    flight_network.traverseCities([this](const String &city, const CityInfo &cityInfo) {
        if (!cityInfo.isAbroad) return;
        QString cityName = QString::fromUtf8(city.c_str());
        departureCityComboBox->addItem(cityName);
        arrivalCityComboBox->addItem(cityName);
    });
}

void TicketBookingWindow::onSearchButtonClicked() {
    QString selectedFilter = filterComboBox->currentText();
    user_pred = false;
    switch (filterComboBox->currentIndex()) {
        case 0: sortRule = NO_RULE; break;
        case 1: sortRule = EARLIEST_DEPARTURE; break;
        case 2: sortRule = EARLIEST_ARRIVAL; break;
        case 3: sortRule = SHORTEST_DURATION; break;
        case 4: sortRule = CHEAPEST_PRICE; break;
        case 5: user_pred = true; break;
        default: sortRule = NO_RULE; break;
    }
    sortFunction = getSortFunction(sortRule);
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
    return departureCity != arrivalCity;
}

void TicketBookingWindow::queryConnectingFlights(const String &departureCity, const String &arrivalCity, const Date &date) {
    connectingFlights = std::move(flight_network.findConnectingFlights(departureCity, arrivalCity, date, 2, user_pred));
    connectingFlights.sort(sortFunction);
    connectingFlights.traverse([this](const ConnectingTicket &connectingTicket) {
        addConnectingTicketItem(connectingTicket);
    });
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
    if (ConnectingTicketItem *connectingTicketItem = qobject_cast<ConnectingTicketItem *>(widget)) {
        onConnectingTicketItemClicked(connectingTicketItem->getConnectingTicket());
    }
}

void TicketBookingWindow::onConnectingTicketItemClicked(const ConnectingTicket &connectingTicket) {
    QDialog detailWindow;
    detailWindow.setWindowTitle("联程航班详情");
    detailWindow.setMinimumSize(500, 400);

    QScrollArea *scrollArea = new QScrollArea(&detailWindow);
    scrollArea->setWidgetResizable(true);
    QWidget *scrollWidget = new QWidget(&detailWindow);
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);

    int segmentCount = connectingTicket.getNumberOfTickets();
    LinkedList<CabinType> selectedCabins;

    for (int i = 0; i < segmentCount; ++i) {
        selectedCabins.append(CabinType::None);
    }

    for (int i = 0; i < segmentCount; ++i) {
        const Ticket &ticket = connectingTicket.getTickets().getElementAt(i);

        if (segmentCount > 1) {
            scrollLayout->addWidget(createConnectingFlightInfoLabel(i + 1, &detailWindow));
        }

        scrollLayout->addWidget(createFlightInfoLabels(ticket, &detailWindow));
        scrollLayout->addLayout(createCabinSelectionLayout(ticket, i, selectedCabins, &detailWindow));
    }

    QPushButton *confirmButton = new QPushButton("确认订购", scrollWidget);
    scrollLayout->addWidget(confirmButton, 0, Qt::AlignCenter);

    scrollWidget->setLayout(scrollLayout);
    scrollArea->setWidget(scrollWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(&detailWindow);
    mainLayout->addWidget(scrollArea);

    detailWindow.setStyleSheet(
        "QDialog {"
        "   background-color: #f8f8f8;"
        "}"
        "QPushButton {"
        "   font-size: 16px;"
        "   padding: 10px 20px;"
        "   background-color: #007BFF;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 5px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #0056b3;"
        "}"
        "QLabel {"
        "   font-size: 14px;"
        "   margin-bottom: 5px;"
        "}"
    );

    connect(confirmButton, &QPushButton::clicked, this, [&]() {
        bool allSelected = true;
        for (int i = 0; i < segmentCount; ++i) {
            if (selectedCabins.getElementAt(i) == CabinType::None) {
                allSelected = false;
                break;
            }
        }

        if (allSelected) {
            handleConnectingTicketPurchase(connectingTicket, selectedCabins);
            detailWindow.accept();
        } else {
            QMessageBox::warning(this, "错误", "请为每一程选择舱位");
        }
    });

    detailWindow.exec();
}


QLabel* TicketBookingWindow::createConnectingFlightInfoLabel(int segmentNumber, QWidget *parent) {
    QLabel *flightInfoLabel = new QLabel(QString("第%1程").arg(segmentNumber), parent);
    flightInfoLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
    return flightInfoLabel;
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

void TicketBookingWindow::addCabinLayoutHeaders(QGridLayout *layout, QWidget *parent) {
    layout->addWidget(new QLabel("仓位", parent), 0, 0);
    layout->addWidget(new QLabel("余票", parent), 0, 1);
    layout->addWidget(new QLabel("价格", parent), 0, 2);
    layout->addWidget(new QLabel("折扣", parent), 0, 3);
}


QGridLayout* TicketBookingWindow::createCabinSelectionLayout(const Ticket &ticket, int segmentIndex, LinkedList<CabinType> &selectedCabins, QWidget *parent) {
    QGridLayout *ticketLayout = new QGridLayout();

    addCabinLayoutHeaders(ticketLayout, parent);

    QButtonGroup *cabinGroup = new QButtonGroup(parent);

    auto addCabinOption = [&](const QString& cabinName, CabinType cabinType, double price, int remainingTickets, double discount, int row) {
        QLabel *cabinLabel = new QLabel(cabinName, parent);
        QLabel *ticketsLabel = new QLabel(QString::number(remainingTickets), parent);
        QLabel *priceLabel = new QLabel(QString::number(price), parent);
        QString discountText = (discount == 10.0) ? "无" : QString::number(discount, 'f', 1) + "折";
        QLabel *discountLabel = new QLabel(discountText, parent);
        QRadioButton *selectButton = new QRadioButton(parent);

        ticketLayout->addWidget(cabinLabel, row, 0);
        ticketLayout->addWidget(ticketsLabel, row, 1);
        ticketLayout->addWidget(priceLabel, row, 2);
        ticketLayout->addWidget(discountLabel, row, 3);
        ticketLayout->addWidget(selectButton, row, 4);

        cabinGroup->addButton(selectButton, static_cast<int>(cabinType));
    };

    int row = 1;

    if (ticket.getFlightTicketDetail()->getRemainingTickets(EconomyClass) > 0) {
        addCabinOption("经济舱", EconomyClass, ticket.getFlightTicketDetail()->getCabinPrice(EconomyClass), 
                       ticket.getFlightTicketDetail()->getRemainingTickets(EconomyClass), 
                       ticket.getCabinDiscount(EconomyClass), row++);
    }

    if (ticket.getFlightTicketDetail()->getRemainingTickets(BusinessClass) > 0) {
        addCabinOption("商务舱", BusinessClass, ticket.getFlightTicketDetail()->getCabinPrice(BusinessClass), 
                       ticket.getFlightTicketDetail()->getRemainingTickets(BusinessClass), 
                       ticket.getCabinDiscount(BusinessClass), row++);
    }

    if (ticket.getFlightTicketDetail()->getRemainingTickets(FirstClass) > 0) {
        addCabinOption("头等舱", FirstClass, ticket.getFlightTicketDetail()->getCabinPrice(FirstClass), 
                       ticket.getFlightTicketDetail()->getRemainingTickets(FirstClass), 
                       ticket.getCabinDiscount(FirstClass), row++);
    }

    if (cabinGroup->buttons().isEmpty()) {
        QLabel *noCabinLabel = new QLabel("无可用舱位", parent);
        ticketLayout->addWidget(noCabinLabel, row, 0, 1, 5);
    } else {
        connect(cabinGroup, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::idClicked), this, [=, &selectedCabins](int id) mutable {
            selectedCabins.setElementAt(segmentIndex, static_cast<CabinType>(id));
        });
    }

    return ticketLayout;
}


void TicketBookingWindow::handleConnectingTicketPurchase(const ConnectingTicket &connectingTicket, const LinkedList<CabinType> &selectedCabins) {
    QDialog passengerInfoDialog;
    passengerInfoDialog.setWindowTitle("乘客信息");
    passengerInfoDialog.setMinimumSize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(&passengerInfoDialog);

    QLabel *nameLabel = new QLabel("乘客姓名:", &passengerInfoDialog);
    QLineEdit *nameInput = new QLineEdit(&passengerInfoDialog);
    nameInput->setPlaceholderText("请输入乘客姓名");
    layout->addWidget(nameLabel);
    layout->addWidget(nameInput);

    QLabel *idLabel = new QLabel("身份证号:", &passengerInfoDialog);
    QLineEdit *idInput = new QLineEdit(&passengerInfoDialog);
    idInput->setPlaceholderText("请输入乘客身份证号");
    layout->addWidget(idLabel);
    layout->addWidget(idInput);

    QPushButton *confirmPassengerButton = new QPushButton("确认", &passengerInfoDialog);
    layout->addWidget(confirmPassengerButton);

    // 界面美化
    passengerInfoDialog.setStyleSheet(
        "QDialog {"
        "   background-color: #f0f0f0;"
        "}"
        "QLabel {"
        "   font-size: 14px;"
        "}"
        "QLineEdit {"
        "   font-size: 14px;"
        "   padding: 5px;"
        "   border: 1px solid #ccc;"
        "   border-radius: 4px;"
        "}"
        "QPushButton {"
        "   font-size: 14px;"
        "   padding: 8px 15px;"
        "   background-color: #4CAF50;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #45a049;"
        "}"
    );

    connect(confirmPassengerButton, &QPushButton::clicked, this, [&]() {
        QString name = nameInput->text().trimmed();
        QString idNumber = idInput->text().trimmed();

        if (name.isEmpty()) {
            QMessageBox::warning(&passengerInfoDialog, "错误", "请输入乘客姓名");
            return;
        }
        QRegularExpression idRegex("^[1-9]\\d{16}[\\dXx]$");
        if (!idRegex.match(idNumber).hasMatch()) {
            QMessageBox::warning(&passengerInfoDialog, "错误", "请输入有效的18位身份证号码");
            return;
        }

        String user = current_login_user.getPhoneNumber();
        Passenger passenger(name.toStdString().c_str(), idNumber.toStdString().c_str());
        Order order(user);
        LinkedList<OrderInfo> orderInfos;
        double totalPrice = 0.0;

        for (int i = 0; i < connectingTicket.getNumberOfTickets(); ++i) {
            const Ticket &ticket = connectingTicket.getTickets().getElementAt(i);
            const CabinType &cabinType = selectedCabins.getElementAt(i);
            Flight *flight = ticket.getFlight();
            FlightTicketDetail *ticketDetail = ticket.getFlightTicketDetail();
            double price = ticketDetail->getCabinPrice(cabinType);
            
            OrderInfo orderInfo(
                flight->getFlightName(),
                flight->getairRoute(),
                flight->getAirplaneModel(),
                flight->getAirline(),
                flight->getDepartureTime().getTimeSlot(),
                cabinType,
                price,
                ticketDetail->getFlightDate(),
                ticket,
                passenger
            );
            orderInfos.append(orderInfo);
            totalPrice += price;
        }
        order.setOrderInfos(std::move(orderInfos));

        QDialog orderDialog(&passengerInfoDialog);
        orderDialog.setWindowTitle("订单确认");
        orderDialog.setMinimumSize(500, 400);

        QScrollArea *scrollArea = new QScrollArea(&orderDialog);
        scrollArea->setWidgetResizable(true);
        QWidget *scrollWidget = new QWidget();
        QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);

        // 显示订单信息（仅一次）
        QLabel *orderHeaderLabel = new QLabel(QString("订单号: %1\n乘客姓名: %2\n身份证号: %3")
                                              .arg(order.getOrderNumber().c_str())
                                              .arg(name)
                                              .arg(idNumber), scrollWidget);
        orderHeaderLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
        scrollLayout->addWidget(orderHeaderLabel);

        for (int i = 0; i < order.getOrderInfos().size(); ++i) {
            const OrderInfo &orderInfo = order.getOrderInfos().getElementAt(i);
            QString cabinStr;
            switch (selectedCabins.getElementAt(i)) {
                case FirstClass: cabinStr = "头等舱"; break;
                case BusinessClass: cabinStr = "商务舱"; break;
                case EconomyClass: cabinStr = "经济舱"; break;
                default: cabinStr = "未知舱位";
            }
            QLabel *orderInfoLabel = new QLabel(QString("航班: %1\n舱位: %2\n费用: %3")
                                                .arg(orderInfo.getFlightNumber().c_str())
                                                .arg(cabinStr)
                                                .arg(orderInfo.getPrice()), scrollWidget);
            orderInfoLabel->setStyleSheet("font-size: 14px;");
            scrollLayout->addWidget(orderInfoLabel);
        }

        QLabel *totalLabel = new QLabel(QString("总费用: %1").arg(totalPrice), scrollWidget);
        totalLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
        scrollLayout->addWidget(totalLabel);

        QPushButton *payButton = new QPushButton("支付", scrollWidget);
        payButton->setFixedWidth(100);
        scrollLayout->addWidget(payButton, 0, Qt::AlignCenter);

        scrollWidget->setLayout(scrollLayout);
        scrollArea->setWidget(scrollWidget);

        QVBoxLayout *orderDialogLayout = new QVBoxLayout(&orderDialog);
        orderDialogLayout->addWidget(scrollArea);

        // 界面美化
        orderDialog.setStyleSheet(
            "QDialog {"
            "   background-color: #f0f0f0;"
            "}"
            "QLabel {"
            "   font-size: 14px;"
            "}"
            "QPushButton {"
            "   font-size: 14px;"
            "   padding: 8px 15px;"
            "   background-color: #4CAF50;"
            "   color: white;"
            "   border: none;"
            "   border-radius: 4px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #45a049;"
            "}"
        );

        connect(payButton, &QPushButton::clicked, &orderDialog, [&]() {
            if (buyTicket(order))
                QMessageBox::information(&orderDialog, "订票成功", "联程航班订票成功！");
            else
                QMessageBox::warning(&orderDialog, "订票失败", "订票失败，请确保没有重复购票或者稍后重试！");
            orderDialog.accept();
            passengerInfoDialog.accept();
        });

        orderDialog.exec();
    });

    passengerInfoDialog.exec();
}

static std::function<bool(const ConnectingTicket&, const ConnectingTicket&)> getSortFunction(SortRule rule) {
    switch (rule) {
        case NO_RULE:
            return [](const ConnectingTicket& a, const ConnectingTicket& b) {
                return a.getNumberOfTickets() < b.getNumberOfTickets();
            };
        case EARLIEST_DEPARTURE:
            return [](const ConnectingTicket& a, const ConnectingTicket& b) {
                return a.getDepartureDateTime() < b.getDepartureDateTime();
            };
        case EARLIEST_ARRIVAL:
            return [](const ConnectingTicket& a, const ConnectingTicket& b) {
                return a.getArrivalDateTime() < b.getArrivalDateTime();
            };
        case SHORTEST_DURATION:
            return [](const ConnectingTicket& a, const ConnectingTicket& b) {
                return a.getDuration() < b.getDuration();
            };
        case CHEAPEST_PRICE:
            return [](const ConnectingTicket& a, const ConnectingTicket& b) {
                return a.getPrice() < b.getPrice();
            };
        default:
            return [](const ConnectingTicket& a, const ConnectingTicket& b) {
                return false;
            };
    }
}