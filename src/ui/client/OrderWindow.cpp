#include <ui/client/OrderWindow.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QScrollArea>
#include <QButtonGroup>
#include <QRadioButton>
#include <QListWidgetItem>
#include <QDateTime>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QDateEdit>
#include <QString>
#include <FlightSystem/Flight.h>
#include <FlightSystem/Ticket.h>
#include <FlightSystem/Passenger.h>
#include <file/fileManage.h>
#include <functional>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>

OrderItem::OrderItem(const Order &order, QWidget *parent)
    : QWidget(parent), order(order) {
    setupUI();
    setupConnections();
}

void OrderItem::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    int segmentCount = order.getNumberOfSegments();
    const LinkedList<OrderInfo>& orderInfos = order.getOrderInfos();
    QString buttonStyle = "QPushButton { background-color: #66b3ff; color: white; border-radius: 5px; padding: 5px; }"
                          "QPushButton:disabled { background-color: #cccccc; color: #888888; }";

    for (int i = 0; i < segmentCount; ++i) {
        const OrderInfo& orderInfo = orderInfos.getElementAt(i);
        const Ticket& ticket = orderInfo.getTicket();

        QLabel *flightNumberLabel = new QLabel("<b>航班号:</b> " + QString::fromUtf8(orderInfo.getFlightNumber().c_str()), this);
        
        QHBoxLayout *departureArrivalLayout = new QHBoxLayout();
        QLabel *departureLabel = new QLabel(QString("<b>出发:</b> %1 (%2)")
            .arg(QString::fromUtf8(ticket.getFlight()->getDepartureAirport().getName().c_str()))
            .arg(ticket.getDepartureDateTime().toString().c_str()), this);

        QLabel *arrivalLabel = new QLabel(QString("<b>到达:</b> %1 (%2)")
            .arg(QString::fromUtf8(ticket.getFlight()->getArrivalAirport().getName().c_str()))
            .arg(ticket.getArrivalDateTime().toString().c_str()), this);
        
        departureArrivalLayout->addWidget(departureLabel);
        departureArrivalLayout->addWidget(arrivalLabel);
        
        QHBoxLayout *seatMealLayout = new QHBoxLayout();
        QLabel *seatNumLabel = new QLabel("<b>座位号:</b> " + QString::fromUtf8(orderInfo.getSeatNum().c_str()), this);

        QString mealStr;
        switch (orderInfo.getMeal()) {
            case NO_MEAL: mealStr = "无"; break;
            case WESTERN: mealStr = "西餐"; break;
            case CHINESE: mealStr = "中餐"; break;
            case VEGETARIAN: mealStr = "素食"; break;
            default: mealStr = "未知";
        }
        QLabel *mealLabel = new QLabel("<b>餐食:</b> " + mealStr, this);
        seatMealLayout->addWidget(seatNumLabel);
        seatMealLayout->addSpacing(20);
        seatMealLayout->addWidget(mealLabel);

        QPushButton *selectSeatButton = new QPushButton("选座", this);
        QPushButton *orderMealButton = new QPushButton("订餐", this);
        selectSeatButton->setStyleSheet(buttonStyle);
        orderMealButton->setStyleSheet(buttonStyle);
        selectSeatButtons.push_back(selectSeatButton);
        orderMealButtons.push_back(orderMealButton);

        QHBoxLayout *actionLayout = new QHBoxLayout();
        actionLayout->addLayout(seatMealLayout);
        actionLayout->addStretch();
        actionLayout->addWidget(selectSeatButton);
        actionLayout->addWidget(orderMealButton);

        QFrame *segmentFrame = new QFrame(this);
        segmentFrame->setFrameShape(QFrame::StyledPanel);
        segmentFrame->setStyleSheet("QFrame { background-color: #f0f8ff; border-radius: 8px; padding: 10px; }");

        QVBoxLayout *segmentLayout = new QVBoxLayout(segmentFrame);
        segmentLayout->addWidget(flightNumberLabel);
        segmentLayout->addLayout(departureArrivalLayout);
        segmentLayout->addLayout(actionLayout);

        mainLayout->addWidget(segmentFrame);
        mainLayout->addSpacing(15);
    }

    QString statusStr;
    switch (order.getStatus()) {
        case BOOKED: statusStr = "已订票"; break;
        case CANCELED: statusStr = "已取消"; break;
        case CHECKED_IN: statusStr = "已登机"; break;
        case REFUNDED: statusStr = "已退票"; break;
        default: statusStr = "未知状态";
    }
    statusLabel = new QLabel("<b>订单状态:</b> " + statusStr, this);

    refundTicketButton = new QPushButton("退票", this);
    changeTicketButton = new QPushButton("改签", this);


    refundTicketButton->setStyleSheet(buttonStyle);
    changeTicketButton->setStyleSheet(buttonStyle);

    QHBoxLayout *statusAndButtonsLayout = new QHBoxLayout();
    statusAndButtonsLayout->addWidget(statusLabel);
    statusAndButtonsLayout->addStretch();
    statusAndButtonsLayout->addWidget(refundTicketButton);
    statusAndButtonsLayout->addWidget(changeTicketButton);

    mainLayout->addLayout(statusAndButtonsLayout);

    setLayout(mainLayout);
}

void OrderItem::setupConnections() {

    for (int i = 0; i < selectSeatButtons.size(); ++i) {

        if (order.getSeatNum(i) != "NULL") {
            selectSeatButtons[i]->setEnabled(false);
        }

        if (order.getMeal(i) != NO_MEAL) {
            orderMealButtons[i]->setEnabled(false);
        }

        connect(selectSeatButtons[i], &QPushButton::clicked, this, [this, i]() {
            const Ticket &ticket = order.getTicket(i);
            emit selectSeat(order, i, ticket);
        });

        connect(orderMealButtons[i], &QPushButton::clicked, this, [this, i]() {
            const Ticket &ticket = order.getTicket(i);
            emit orderMeal(order, i, ticket);
        });
    }


    if (order.getStatus() == REFUNDED) {
        for (auto &button : selectSeatButtons) {
            button->setEnabled(false);
        }
        for (auto &button : orderMealButtons) {
            button->setEnabled(false);
        }
        refundTicketButton->setEnabled(false);
        changeTicketButton->setEnabled(false);
    }


    connect(refundTicketButton, &QPushButton::clicked, this, [this]() {
        emit refundTicket(order);
    });
    connect(changeTicketButton, &QPushButton::clicked, this, [this]() {
        emit changeTicket(order);
    });
}


OrderWindow::OrderWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
    populateOrderList();
}

void OrderWindow::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    orderListWidget = new QListWidget(this);
    orderListWidget->setStyleSheet(
        "QListWidget {"
        "   font-size: 16px;"
        "}"
        "QListWidget::item {"
        "   background-color: lightblue;"
        "   border: 1px solid lightblue;"
        "   border-radius: 10px;"
        "   padding: 5px;"
        "}"
    );
    orderListWidget->setSpacing(5);
    orderListWidget->setUniformItemSizes(false);
    orderListWidget->setSelectionMode(QAbstractItemView::NoSelection);
    orderListWidget->setFocusPolicy(Qt::NoFocus);
    connect(orderListWidget, &QListWidget::itemClicked, this, &OrderWindow::onOrderItemClicked);

    mainLayout->addWidget(orderListWidget);
    setLayout(mainLayout);
}

void OrderWindow::populateOrderList() {
    orderListWidget->clear();

    order_map.traverse([this](const Order &order) {
        if (order.getStatus() == CANCELED || order.getStatus() == REFUNDED) {
            return;
        }
        addOrderItem(order);
    });

    order_map.traverse([this](const Order &order) {
        if (order.getStatus() == CANCELED || order.getStatus() == REFUNDED) {
            addOrderItem(order);
        }
    });
}

void OrderWindow::refreshOrderList() {
    populateOrderList();
}

void OrderWindow::addOrderItem(const Order &order) {
    OrderItem *itemWidget = new OrderItem(order);

    connect(itemWidget, &OrderItem::selectSeat, this, &OrderWindow::handleSelectSeat);
    connect(itemWidget, &OrderItem::orderMeal, this, &OrderWindow::handleOrderMeal);
    connect(itemWidget, &OrderItem::refundTicket, this, &OrderWindow::handleRefundTicket);
    connect(itemWidget, &OrderItem::changeTicket, this, &OrderWindow::handleChangeTicket);

    QListWidgetItem *listItem = new QListWidgetItem(orderListWidget);
    listItem->setSizeHint(itemWidget->sizeHint());

    orderListWidget->addItem(listItem);
    orderListWidget->setItemWidget(listItem, itemWidget);
}

void OrderWindow::onOrderItemClicked(QListWidgetItem *item) {

}

void OrderWindow::handleSelectSeat(const Order &order, int segmentIndex, const Ticket &ticket) {
    QDialog dialog;
    dialog.setWindowTitle("选座");
    dialog.setFixedSize(600, 500);
    dialog.setStyleSheet("background-color: #f5f5f5; border-radius: 10px;");

    QVBoxLayout mainLayout(&dialog);

    QLabel infoLabel("请选择座位:", &dialog);
    infoLabel.setAlignment(Qt::AlignCenter);
    infoLabel.setStyleSheet("font-size: 16px; color: #333; margin: 10px;");
    mainLayout.addWidget(&infoLabel);

    AirplaneModel *airplaneModel = airplane_model_map.find(ticket.getFlight()->getAirplaneModel());
    if (!airplaneModel) {
        QMessageBox::warning(&dialog, "错误", "飞机型号不存在");
        return;
    }

    const OrderInfo &orderInfo = order.getOrderInfos().getElementAt(segmentIndex);
    CabinType cabinType = orderInfo.getCabinType();
    Cabin cabin = airplaneModel->getCabin(cabinType);

    QGridLayout seatLayout;

    int rows = cabin.getRows();
    int columns = cabin.getColumns();

    QString cabinPrefix;
    switch (cabinType) {
        case EconomyClass: cabinPrefix = "E"; break;
        case BusinessClass: cabinPrefix = "B"; break;
        case FirstClass: cabinPrefix = "F"; break;
        default: cabinPrefix = "";
    }

    QMap<QString, QPushButton*> seatButtons;

    QString availableSeatImage = ":/icons/seat_available.png";
    QString selectedSeatImage = ":/icons/seat_selected.png";
    QString occupiedSeatImage = ":/icons/seat_occupied.png";

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            QString seatNum = cabinPrefix + QString::number(i + 1) + QChar('A' + j);

            QVBoxLayout *seatButtonLayout = new QVBoxLayout();
            QLabel *seatLabel = new QLabel(seatNum, &dialog);
            seatLabel->setAlignment(Qt::AlignCenter);
            QPushButton *seatButton = new QPushButton(&dialog);
            seatButton->setFixedSize(40, 40);
            seatButton->setStyleSheet(QString("QPushButton { border-image: url(%1); }").arg(availableSeatImage));
            seatButton->setProperty("seatNum", seatNum);

            seatButtonLayout->addWidget(seatLabel);
            seatButtonLayout->addWidget(seatButton);
            seatLayout.addLayout(seatButtonLayout, i, j);

            seatButtons[seatNum] = seatButton;

            connect(seatButton, &QPushButton::clicked, &dialog, [=, &seatButtons]() {
                for (auto &button : seatButtons) {
                    button->setStyleSheet(QString("QPushButton { border-image: url(%1); }").arg(availableSeatImage));
                }
                seatButton->setStyleSheet(QString("QPushButton { border-image: url(%1); }").arg(selectedSeatImage));
            });
        }
    }

    PassengerMap passengerMap([](const Passenger &passenger) { return passenger.getIdNumber(); });
    String passengerFileName = FLIGHTS_DIR + orderInfo.getFlightNumber() + "/" + orderInfo.getDate().toString() + ".txt";

    if (!loadMapFromFile(passengerMap, passengerFileName.c_str())) {
        QMessageBox::warning(&dialog, "错误", "加载乘客信息失败");
        return;
    }

    passengerMap.traverse([&seatButtons, &occupiedSeatImage](const Passenger &passenger) {
        String seatNumStr = passenger.getSeatNum();
        if (seatNumStr == "NULL" || seatNumStr == "") return;
        QString seatNum = QString::fromUtf8(seatNumStr.c_str());
        if (seatButtons.contains(seatNum)) {
            QPushButton *seatButton = seatButtons[seatNum];
            seatButton->setStyleSheet(QString("QPushButton { border-image: url(%1); }").arg(occupiedSeatImage));
            seatButton->setEnabled(false);
        }
    });

    QWidget seatWidget(&dialog);
    seatWidget.setLayout(&seatLayout);

    QScrollArea scrollArea(&dialog);
    scrollArea.setWidget(&seatWidget);
    scrollArea.setWidgetResizable(true);
    scrollArea.setStyleSheet("border: none; background: transparent;");
    mainLayout.addWidget(&scrollArea);

    QPushButton confirmButton("确认", &dialog);
    confirmButton.setStyleSheet("background-color: #4CAF50; color: white; font-size: 14px; padding: 8px 16px; border-radius: 5px;");
    mainLayout.addWidget(&confirmButton);

    connect(&confirmButton, &QPushButton::clicked, &dialog, [&]() {
        QString selectedSeatNum;
        for (auto it = seatButtons.begin(); it != seatButtons.end(); ++it) {
            if (it.value()->styleSheet().contains(selectedSeatImage)) {
                selectedSeatNum = it.key();
                break;
            }
        }

        if (!selectedSeatNum.isEmpty()) {
            if (chooseSeat(order, segmentIndex, selectedSeatNum.toStdString().c_str())) {
                QMessageBox::information(&dialog, "成功", "选座成功！");
                dialog.accept();
                refreshOrderList();
            } else {
                QMessageBox::warning(&dialog, "错误", "选座失败");
            }
        } else {
            QMessageBox::warning(&dialog, "错误", "请选择座位");
        }
    });

    dialog.exec();
}

void OrderWindow::handleOrderMeal(const Order &order, int segmentIndex, const Ticket &ticket) {
    QDialog dialog;
    dialog.setWindowTitle("选餐");
    dialog.setFixedSize(350, 300);
    dialog.setStyleSheet("background-color: #f5f5f5; border-radius: 10px;");

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *infoLabel = new QLabel("请选择餐食:", &dialog);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin-bottom: 20px;");
    layout->addWidget(infoLabel);

    QStringList meals = {"西餐", "中餐", "清真"};
    QButtonGroup *mealGroup = new QButtonGroup(&dialog);
    QVBoxLayout *mealLayout = new QVBoxLayout();

    for (int i = 0; i < meals.size(); ++i) {
        QRadioButton *mealButton = new QRadioButton(meals[i], &dialog);
        mealButton->setStyleSheet("font-size: 16px; padding: 5px;");
        mealGroup->addButton(mealButton, i);
        mealLayout->addWidget(mealButton);
    }
    layout->addLayout(mealLayout);

    QPushButton *confirmButton = new QPushButton("确认", &dialog);
    confirmButton->setStyleSheet("background-color: #4CAF50; color: white; font-size: 16px; padding: 10px; border-radius: 5px;");
    layout->addWidget(confirmButton, 0, Qt::AlignCenter);

    connect(confirmButton, &QPushButton::clicked, this, [&]() {
        QAbstractButton *checkedButton = mealGroup->checkedButton();
        if (checkedButton) {
            Meal meal = static_cast<Meal>(mealGroup->id(checkedButton) + 1);
            if (buyMeal(order, segmentIndex, meal)) {
                QMessageBox::information(&dialog, "成功", "订餐成功！");
                dialog.accept();
                refreshOrderList();
            } else {
                QMessageBox::warning(&dialog, "错误", "订餐失败");
            }
        } else {
            QMessageBox::warning(&dialog, "错误", "请选择餐食");
        }
    });

    dialog.exec();
}

void OrderWindow::handleRefundTicket(const Order &order) {
    QDialog refundDialog;
    refundDialog.setWindowTitle("退票确认");
    refundDialog.setFixedSize(450, 350);
    refundDialog.setStyleSheet("background-color: #f5f5f5; border-radius: 10px;");

    QVBoxLayout mainLayout(&refundDialog);

    QScrollArea scrollArea(&refundDialog);
    scrollArea.setWidgetResizable(true);
    scrollArea.setStyleSheet("border: none;");

    QWidget scrollWidget;
    QVBoxLayout scrollLayout(&scrollWidget);

    double totalRefundFee = 0.0;
    double totalPrice = 0.0;
    QString refundDetails;

    for (int i = 0; i < order.getNumberOfSegments(); ++i) {
        const OrderInfo &orderInfo = order.getOrderInfos().getElementAt(i);
        Flight *flight = flight_map.find(orderInfo.getFlightNumber());

        if (!flight) {
            QMessageBox::warning(this, "错误", "无法找到航班信息");
            return;
        }

        DateTime departureDateTime = DateTime(orderInfo.getDate(), flight->getDepartureTime()) + flight->getCostTime();
        QDateTime datetime = QDateTime::fromString(departureDateTime.toString().c_str(), "yyyy-MM-dd HH:mm:ss");

        double refundFee = calculateRefundFee(orderInfo.getPrice(), datetime);
        totalRefundFee += refundFee;
        totalPrice += orderInfo.getPrice();

        refundDetails += QString("航段 %1:\n").arg(i + 1);
        refundDetails += QString("航班号: %1\n").arg(orderInfo.getFlightNumber().c_str());
        refundDetails += QString("票价: ¥%1\n").arg(static_cast<double>(orderInfo.getPrice()), 0, 'f', 2);
        refundDetails += QString("退票手续费: ¥%1\n").arg(static_cast<double>(refundFee), 0, 'f', 2);
        refundDetails += QString("实际退款金额: ¥%1\n\n").arg(static_cast<double>(orderInfo.getPrice() - refundFee), 0, 'f', 2);
    }

    QLabel refundInfoLabel(refundDetails, &scrollWidget);
    refundInfoLabel.setStyleSheet("font-size: 14px; color: #333;");
    refundInfoLabel.setAlignment(Qt::AlignLeft);
    refundInfoLabel.setWordWrap(true);
    scrollLayout.addWidget(&refundInfoLabel);

    QLabel tipLabel(
        "温馨提示：\n根据航空公司退票规定，航班起飞前7天以上免费退票，\n"
        "7天内退票收取票价5%的手续费，\n48小时内退票收取票价10%的手续费，\n"
        "4小时内退票收取票价15%的手续费。\n请确认是否继续退票。",
        &scrollWidget);
    tipLabel.setStyleSheet("font-size: 12px; color: #666;");
    tipLabel.setAlignment(Qt::AlignLeft);
    tipLabel.setWordWrap(true);
    tipLabel.setMargin(10);
    scrollLayout.addWidget(&tipLabel);

    scrollWidget.setLayout(&scrollLayout);
    scrollArea.setWidget(&scrollWidget);
    mainLayout.addWidget(&scrollArea);

    QHBoxLayout buttonLayout;
    QPushButton confirmButton("确认退票", &refundDialog);
    confirmButton.setStyleSheet(
        "background-color: #4CAF50; color: white; font-size: 14px; padding: 8px 16px; border-radius: 5px;");
    QPushButton cancelButton("取消", &refundDialog);
    cancelButton.setStyleSheet(
        "background-color: #f44336; color: white; font-size: 14px; padding: 8px 16px; border-radius: 5px;");
    buttonLayout.addWidget(&confirmButton);
    buttonLayout.addWidget(&cancelButton);
    mainLayout.addLayout(&buttonLayout);

    connect(&confirmButton, &QPushButton::clicked, &refundDialog, [&]() {
        if (refundTicket(order)) {
            QMessageBox::information(this, "退票成功", "退票成功！已扣除手续费。");
            refreshOrderList();
            refundDialog.accept();
        } else {
            QMessageBox::warning(this, "退票失败", "退票失败，请稍后重试。");
        }
    });

    connect(&cancelButton, &QPushButton::clicked, &refundDialog, &QDialog::reject);

    refundDialog.exec();
}


void OrderWindow::handleChangeTicket(const Order &order) {
    QDialog dialog;
    dialog.setWindowTitle("改签");
    dialog.setFixedSize(450, 400);
    dialog.setStyleSheet("background-color: #f5f5f5; border-radius: 10px;");

    QVBoxLayout layout(&dialog);

    QLabel infoLabel("请选择新的出发日期:\n注意:改签只支持更改出行日期\n如果想要更改其他信息\n请退票后重新购买", &dialog);
    infoLabel.setAlignment(Qt::AlignCenter);
    infoLabel.setStyleSheet("font-size: 14px; color: #333; margin: 10px;");
    layout.addWidget(&infoLabel);

    QComboBox dateComboBox(&dialog);
    layout.addWidget(&dateComboBox);

    const LinkedList<OrderInfo>& orderInfos = order.getOrderInfos();
    if (orderInfos.isEmpty()) {
        QMessageBox::warning(this, "错误", "订单中没有航段信息");
        return;
    }

    const OrderInfo& firstOrderInfo = orderInfos.getElementAt(0);
    const Ticket& firstTicket = firstOrderInfo.getTicket();
    Flight* firstFlight = firstTicket.getFlight();
    if (!firstFlight) {
        QMessageBox::warning(this, "错误", "无法找到第一程航班信息");
        return;
    }

    const FlightScheduleMap& flightScheduleMap = firstFlight->getFlightSchedule();

    LinkedList<int> dateDifferences;
    Date firstSegmentDate = firstOrderInfo.getDate();
    orderInfos.traverse([&](const OrderInfo& orderInfo) {
        int difference = orderInfo.getDate() - firstSegmentDate;
        dateDifferences.append(difference);
    });

    LinkedList<Date> availableFirstDates;

    flightScheduleMap.traverse([&](const FlightTicketDetail& flightDetail) {
        bool allSegmentsAvailable = true;
        Date firstDate = flightDetail.getFlightDate();

        LinkedList<Date> segmentDates;
        dateDifferences.traverse([&](int difference) {
            Date segmentDate = firstDate + difference;
            segmentDates.append(segmentDate);
        });

        int segmentIndex = 0;
        orderInfos.traverse([&](const OrderInfo& orderInfo) {
            if (!allSegmentsAvailable) return;

            const Ticket& ticket = orderInfo.getTicket();
            Flight* flight = ticket.getFlight();
            CabinType cabinType = orderInfo.getCabinType();
            Date segmentDate = segmentDates.getElementAt(segmentIndex);

            if (!flight) {
                allSegmentsAvailable = false;
                return;
            }

            FlightTicketDetail* ticketDetail = flight->getFlightTicketDetail(segmentDate);
            if (!ticketDetail || ticketDetail->getRemainingTickets(cabinType) <= 0) {
                allSegmentsAvailable = false;
                return;
            }

            segmentIndex++;
        });

        if (allSegmentsAvailable) {
            availableFirstDates.append(firstDate);
            dateComboBox.addItem(QString::fromUtf8(firstDate.toString().c_str()));
        }
    });

    if (dateComboBox.count() == 0) {
        QMessageBox::warning(this, "提示", "没有符合条件的日期");
        return;
    }

    QPushButton confirmButton("确认", &dialog);
    confirmButton.setStyleSheet("background-color: #4CAF50; color: white; font-size: 14px; padding: 8px 16px; border-radius: 5px;");
    layout.addWidget(&confirmButton);

    connect(&confirmButton, &QPushButton::clicked, this, [&]() {
        if (dateComboBox.currentIndex() >= 0) {
            QString selectedDateStr = dateComboBox.currentText();
            Date selectedFirstDate = Date::fromString(selectedDateStr.toStdString().c_str());

            LinkedList<Date> newSegmentDates;
            dateDifferences.traverse([&](int difference) {
                newSegmentDates.append(selectedFirstDate + difference);
            });

            Order newOrder(current_login_user.getPhoneNumber());
            LinkedList<OrderInfo> newOrderInfos;
            double totalOriginalPrice = 0.0;
            double totalNewPrice = 0.0;

            int segmentIndex = 0;
            bool errorOccurred = false;
            orderInfos.traverse([&](const OrderInfo& orderInfo) {
                if (errorOccurred) return;

                OrderInfo newOrderInfo = orderInfo;
                Date newDate = newSegmentDates.getElementAt(segmentIndex);
                newOrderInfo.setDate(newDate);

                Flight* flight = orderInfo.getTicket().getFlight();
                if (!flight) {
                    QMessageBox::warning(&dialog, "错误", "无法找到航班信息");
                    errorOccurred = true;
                    return;
                }

                FlightTicketDetail* newTicketDetail = flight->getFlightTicketDetail(newDate);
                if (!newTicketDetail || newTicketDetail->getRemainingTickets(orderInfo.getCabinType()) <= 0) {
                    QMessageBox::warning(&dialog, "错误", QString("航段 %1 在新日期没有余票")
                                                        .arg(segmentIndex + 1));
                    errorOccurred = true;
                    return;
                }

                double originalPrice = newOrderInfo.getPrice();
                double newPrice = newTicketDetail->getCabinPrice(orderInfo.getCabinType());
                totalOriginalPrice += originalPrice;
                totalNewPrice += newPrice;

                newOrderInfo.setPrice(newPrice);
                newOrderInfo.setTicket(Ticket(flight, newTicketDetail));
                newOrderInfos.append(newOrderInfo);

                segmentIndex++;
            });

            if (errorOccurred) return;

            newOrder.setOrderInfos(std::move(newOrderInfos));

            double priceDifference = totalNewPrice - totalOriginalPrice;
            QString priceDifferenceMessage = (priceDifference > 0)
                                             ? QString("改签后需要补差价: %1").arg(priceDifference)
                                             : QString("改签后将退还差价: %1").arg(-priceDifference);

            int ret = QMessageBox::question(&dialog, "确认改签",
                                            QString("原总票价: %1\n新总票价: %2\n%3\n是否确认改签？")
                                                .arg(totalOriginalPrice)
                                                .arg(totalNewPrice)
                                                .arg(priceDifferenceMessage),
                                            QMessageBox::Yes | QMessageBox::No);

            if (ret == QMessageBox::Yes) {
                if (changeTicket(order, newOrder)) {
                    QMessageBox::information(&dialog, "成功", "改签成功！");
                    dialog.accept();
                    refreshOrderList();
                } else {
                    QMessageBox::warning(&dialog, "错误", "改签失败");
                }
            }
        } else {
            QMessageBox::warning(&dialog, "错误", "请选择日期");
        }
    });

    dialog.exec();
}

double OrderWindow::calculateRefundFee(double price, const QDateTime &departureDateTime) {
    double feePercentage = 0.0;

    QDateTime now = QDateTime::currentDateTime();
    int hoursToFlight = now.secsTo(departureDateTime) / 3600;

    if (hoursToFlight > 168) {
        feePercentage = 0.0;
    } else if (hoursToFlight > 48) {
        feePercentage = 0.05;
    } else if (hoursToFlight > 4) {
        feePercentage = 0.10;
    } else {
        feePercentage = 0.15;
    }

    return price * feePercentage;
}
