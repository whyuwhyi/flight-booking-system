#include <ui/client/PersonalCenterWindow.h>
#include <file/fileManage.h>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QFormLayout>
#include <QScrollArea>


OrderItem::OrderItem(const Order &order, QWidget *parent)
    : QWidget(parent), order(order)
{
    flight = flight_map.find(order.getFlightNumber());
    if (flight) {
        flightTicketDetail = flight->getFlightTicketDetail(order.getDate());
    } else {
        flightTicketDetail = nullptr;
    }

    setupUI();
    setupConnecttions();
}

void OrderItem::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);

    flightNumberLabel = new QLabel("航班号: " + QString::fromUtf8(flight->getFlightName().c_str()), this);
    dateLabel = new QLabel("日期: " + QString(order.getDate().toString().c_str()), this);
    departureCityLabel = new QLabel("出发: " + QString::fromUtf8(flight->getDepartureAirport().getName().c_str()), this);
    arrivalCityLabel = new QLabel("到达: " + QString::fromUtf8(flight->getArrivalAirport().getName().c_str()), this);
    departureTimeLabel = new QLabel("出发时间: " + QString(flight->getDepartureTime().toString().c_str()), this);

    Time arrivalTime = flight->getDepartureTime() + flight->getCostTime();
    int daysDifference = 0;
    while (arrivalTime > Time(24, 0, 0)) {
        arrivalTime -= Time(24, 0, 0);
        daysDifference++;
    }
    QString arrivalTimeStr = QString(arrivalTime.toString().c_str());
    QString dayDif = daysDifference > 0 ? "(+" + QString::number(daysDifference) + ")" : "";
    arrivalTimeStr += dayDif;
    

    arrivalTimeLabel = new QLabel("到达时间: " + arrivalTimeStr, this);
    seatNumLabel = new QLabel("座位号: " + QString::fromUtf8(order.getSeatNum().c_str()), this);
    
    switch (order.getMeal()) {
        case NO_MEAL:
            mealLabel = new QLabel("餐食: 无", this);
            break;
        case WESTERN:
            mealLabel = new QLabel("餐食: 西餐", this);
            break;
        case CHINESE:
            mealLabel = new QLabel("餐食: 中餐", this);
            break;
        case VEGETARIAN:
            mealLabel = new QLabel("餐食: 素食", this);
            break;
    }

    switch (order.getStatus()) {
        case BOOKED:
            statusLabel = new QLabel("状态: 已订票", this);
            break;
        case CANCELED:
            statusLabel = new QLabel("状态: 已取消", this);
            break;
        case CHECKED_IN:
            statusLabel = new QLabel("状态: 已登机", this);
            break;
        case REFUNDED:
            statusLabel = new QLabel("状态: 已退票", this);
            break;
    }
    

    selectSeatButton = new QPushButton("选座", this);
    orderMealButton = new QPushButton("订餐", this);
    refundTicketButton = new QPushButton("退票", this);
    changeTicketButton = new QPushButton("改签", this);

    selectSeatButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #5DADE2;"  // 按钮背景色
        "   color: white;"              // 按钮文字颜色
        "   font-size: 16px;"           // 字体大小
        "   border-radius: 8px;"        // 圆角
        "   padding: 10px 15px;"        // 内边距
        "}"
        "QPushButton:hover {"
        "   background-color: #3498DB;" // 鼠标悬停时的颜色
        "}"
        "QPushButton:pressed {"
        "   background-color: #2980B9;" // 按下时的颜色
        "}"
    );

    orderMealButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #58D68D;"
        "   color: white;"
        "   font-size: 16px;"
        "   border-radius: 8px;"
        "   padding: 10px 15px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2ECC71;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #27AE60;"
        "}"
    );

    refundTicketButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #F1948A;"
        "   color: white;"
        "   font-size: 16px;"
        "   border-radius: 8px;"
        "   padding: 10px 15px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #EC7063;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #CB4335;"
        "}"
    );

    changeTicketButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #F7DC6F;"
        "   color: black;"
        "   font-size: 16px;"
        "   border-radius: 8px;"
        "   padding: 10px 15px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #F4D03F;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #D4AC0D;"
        "}"
    );


    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(selectSeatButton);
    buttonLayout->addWidget(orderMealButton);
    buttonLayout->addWidget(refundTicketButton);
    buttonLayout->addWidget(changeTicketButton);

    layout->addWidget(flightNumberLabel);
    layout->addWidget(dateLabel);
    layout->addWidget(departureCityLabel);
    layout->addWidget(arrivalCityLabel);
    layout->addWidget(departureTimeLabel);
    layout->addWidget(arrivalTimeLabel);
    layout->addWidget(seatNumLabel);
    layout->addWidget(mealLabel);
    layout->addWidget(statusLabel);
    layout->addLayout(buttonLayout);

    setLayout(layout);
}

void OrderItem::setupConnecttions() {
    if (order.getSeatNum() != "NULL") {
        selectSeatButton->setEnabled(false);
        selectSeatButton->setStyleSheet(
            "QPushButton {"
            "   background-color: lightgray;"  // 按钮背景色
            "   color: white;"                 // 按钮文字颜色
            "   font-size: 16px;"              // 字体大小
            "   border-radius: 8px;"           // 圆角
            "   padding: 10px 15px;"           // 内边距
            "}"
        );
    }
    
    if (order.getStatus() == REFUNDED) {
        selectSeatButton->setEnabled(false);
        orderMealButton->setEnabled(false);
        refundTicketButton->setEnabled(false);
        changeTicketButton->setEnabled(false);
        selectSeatButton->setStyleSheet(
            "QPushButton {"
            "   background-color: lightgray;"  // 按钮背景色
            "   color: white;"                 // 按钮文字颜色
            "   font-size: 16px;"              // 字体大小
            "   border-radius: 8px;"           // 圆角
            "   padding: 10px 15px;"           // 内边距
            "}"
        );
         orderMealButton->setStyleSheet(
            "QPushButton {"
            "   background-color: lightgray;"  // 按钮背景色
            "   color: white;"                 // 按钮文字颜色
            "   font-size: 16px;"              // 字体大小
            "   border-radius: 8px;"           // 圆角
            "   padding: 10px 15px;"           // 内边距
            "}"
        );
         refundTicketButton->setStyleSheet(
            "QPushButton {"
            "   background-color: lightgray;"  // 按钮背景色
            "   color: white;"                 // 按钮文字颜色
            "   font-size: 16px;"              // 字体大小
            "   border-radius: 8px;"           // 圆角
            "   padding: 10px 15px;"           // 内边距
            "}"
        );
         changeTicketButton->setStyleSheet(
            "QPushButton {"
            "   background-color: lightgray;"  // 按钮背景色
            "   color: white;"                 // 按钮文字颜色
            "   font-size: 16px;"              // 字体大小
            "   border-radius: 8px;"           // 圆角
            "   padding: 10px 15px;"           // 内边距
            "}"
        );
    }
        
    if (order.getMeal() != NO_MEAL) {
        orderMealButton->setEnabled(false);
        orderMealButton->setStyleSheet(
            "QPushButton {"
            "   background-color: lightgray;"  // 按钮背景色
            "   color: white;"                 // 按钮文字颜色
            "   font-size: 16px;"              // 字体大小
            "   border-radius: 8px;"           // 圆角
            "   padding: 10px 15px;"           // 内边距
            "}"
        );
    }
    
    connect(selectSeatButton, &QPushButton::clicked, this, [=]() {
        emit selectSeat(order, flight);
    });
    connect(orderMealButton, &QPushButton::clicked, this, [=]() {
        emit orderMeal(order);
    });
    connect(refundTicketButton, &QPushButton::clicked, this, [=]() {
        emit refundTicket(order);
    });
    connect(changeTicketButton, &QPushButton::clicked, this, [=]() {
        emit changeTicket(order);
    });
}

PersonalCenterWindow::PersonalCenterWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
    populateOrderList();
}

void PersonalCenterWindow::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("我的订单", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    mainLayout->addWidget(titleLabel);

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
    connect(orderListWidget, &QListWidget::itemClicked, this, &PersonalCenterWindow::onOrderItemClicked);

    mainLayout->addWidget(orderListWidget);
    setLayout(mainLayout);
}

void PersonalCenterWindow::populateOrderList() {
    orderListWidget->clear();

    order_map.traverse([this](const Order &order) {
        if (order.getStatus() == BOOKED)
            addOrderItem(order);
    });

    order_map.traverse([this](const Order &order) {
        if (order.getStatus() != BOOKED)
            addOrderItem(order);
    });
}

void PersonalCenterWindow::refreshOrderList() {
    populateOrderList();
}

void PersonalCenterWindow::addOrderItem(const Order &order) {
    OrderItem *itemWidget = new OrderItem(order);
    
    connect(itemWidget, &OrderItem::selectSeat, this, &PersonalCenterWindow::handleSelectSeat);
    connect(itemWidget, &OrderItem::orderMeal, this, &PersonalCenterWindow::handleOrderMeal);
    connect(itemWidget, &OrderItem::refundTicket, this, &PersonalCenterWindow::handleRefundTicket);
    connect(itemWidget, &OrderItem::changeTicket, this, &PersonalCenterWindow::handleChangeTicket);

    QListWidgetItem *listItem = new QListWidgetItem(orderListWidget);
    listItem->setSizeHint(itemWidget->sizeHint());

    orderListWidget->addItem(listItem);
    orderListWidget->setItemWidget(listItem, itemWidget);
}

void PersonalCenterWindow::onOrderItemClicked(QListWidgetItem *item) {
}

void PersonalCenterWindow::handleSelectSeat(const Order &order, const Flight* flight) {
    AirplaneModel *airplaneModel = airplane_model_map.find(flight->getAirplaneModel());
    Map<String, Order> orderMap([](const Order& order) { return order.getOrderNumber(); });
    String file1Name = FLIGHTS_DIR + order.getFlightNumber() + "/" +
                       order.getDate().toString() + ".txt";

    if (!loadMapFromFile(orderMap, file1Name.c_str())) {
        QMessageBox::warning(this, "错误", "加载订单信息失败");
        return;
    }    

    CabinType cabinType = order.getCabinType();
    if (!airplaneModel) {
        QMessageBox::warning(this, "错误", "飞机型号不存在");
        return;
    }

    Cabin cabin = airplaneModel->getCabin(cabinType);
    QDialog *dialog = createDetailWindow("选座");

    QVBoxLayout *layout = static_cast<QVBoxLayout*>(dialog->property("contentLayout").value<void*>());

    QLabel *infoLabel = new QLabel("请选择座位:", dialog);
    layout->addWidget(infoLabel);

    QButtonGroup *seatGroup = new QButtonGroup(dialog);
    QGridLayout *seatLayout = new QGridLayout();

    int rows = cabin.getRows();
    int columns = cabin.getColumns();

    QString cabinPrefix;
    switch (cabinType) {
        case EconomyClass:
            cabinPrefix = "E";
            break;
        case BusinessClass:
            cabinPrefix = "B";
            break;
        case FirstClass:
            cabinPrefix = "F";
            break;
        default:
            cabinPrefix = "";
            break;
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            QString seatNum = cabinPrefix + QString::number(i + 1) + QChar('A' + j);
            QRadioButton *seatButton = new QRadioButton(seatNum, dialog);

            seatGroup->addButton(seatButton);
            seatLayout->addWidget(seatButton, i, j);
        }
    }

    orderMap.traverse([&seatGroup](const Order &order) {
        if (order.getSeatNum() == "NULL") 
            return;
        QString seatNum = QString::fromUtf8(order.getSeatNum().c_str());
        QAbstractButton *button = nullptr;
        for (auto buttonCandidate : seatGroup->buttons()) {
            if (buttonCandidate->text() == seatNum) {
                button = buttonCandidate;
                break;
            }
        }
        if (button) {
            button->setEnabled(false);
            button->setStyleSheet("color: red;");
        }
    });


    layout->addLayout(seatLayout);

    QPushButton *confirmButton = new QPushButton("确认", dialog);
    layout->addWidget(confirmButton);

    connect(confirmButton, &QPushButton::clicked, this, [=]() {
        QAbstractButton *checkedButton = seatGroup->checkedButton();
        if (checkedButton) {
            String seatNum = checkedButton->text().toStdString().c_str();
            
            if (chooseSeat(order, seatNum)) {
                QMessageBox::information(dialog, "成功", "选座成功！");
                dialog->accept();
                refreshOrderList();
            } else {
                QMessageBox::warning(dialog, "错误", "选座失败");
            }
        } else {
            QMessageBox::warning(dialog, "错误", "请选择座位");
        }
    });

    dialog->exec();
}


void PersonalCenterWindow::handleOrderMeal(const Order &order) {
    QDialog *dialog = createDetailWindow("选餐");

    QVBoxLayout *layout = static_cast<QVBoxLayout*>(dialog->property("contentLayout").value<void*>());

    QLabel *infoLabel = new QLabel("请选择餐食:", dialog);
    layout->addWidget(infoLabel);

    QStringList meals = {"西餐", "中餐", "沙拉"};
    QButtonGroup *mealGroup = new QButtonGroup(dialog);
    QVBoxLayout *mealLayout = new QVBoxLayout();
    for (int i = 0; i < meals.size(); ++i) {
        QRadioButton *mealButton = new QRadioButton(meals[i], dialog);
        mealGroup->addButton(mealButton, i);
        mealLayout->addWidget(mealButton);
    }
    layout->addLayout(mealLayout);

    QPushButton *confirmButton = new QPushButton("确认", dialog);
    layout->addWidget(confirmButton);

    connect(confirmButton, &QPushButton::clicked, this, [=]() {
        QAbstractButton *checkedButton = mealGroup->checkedButton();
        if (checkedButton) {
            Meal meal = static_cast<Meal>(mealGroup->id(checkedButton)+1);

            if (buyMeal(order, meal)) {
                QMessageBox::information(dialog, "成功", "订餐成功！");
                dialog->accept();
                refreshOrderList();
            } else {
                QMessageBox::warning(dialog, "错误", "订餐失败");
            }

        } else {
            QMessageBox::warning(dialog, "错误", "请选择餐食");
        }
    });

    dialog->exec();
}

void PersonalCenterWindow::handleRefundTicket(const Order &order) {
    QDialog refundDialog(this);
    refundDialog.setWindowTitle("退票确认");
    refundDialog.setMinimumSize(400, 300);
    QVBoxLayout layout(&refundDialog);
    
    Flight *flight = flight_map.find(order.getFlightNumber());

    if (!flight) {
        QMessageBox::warning(this, "错误", "无法找到航班信息");
        refundDialog.reject();
        return;
    }

    DateTime departureDateTime = DateTime(order.getDate(), flight->getDepartureTime()) + flight->getCostTime();
    QDateTime datetime =  QDateTime::fromString(departureDateTime.toString().c_str(), "yyyy-MM-dd HH:mm:ss");
    
    double refundFee = calculateRefundFee(order, datetime);
    double refundAmount = order.getPrice() - refundFee;
    
    QString cabinStr;
    switch(order.getCabinType()) {
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

    QLabel refundInfoLabel(QString("订单信息:\n"
                                   "航班号: %1\n"
                                   "舱位: %2\n"
                                   "乘客: %3\n"
                                   "票价: %4\n"
                                   "退票手续费: %5\n"
                                   "实际退款金额: %6")
                           .arg(order.getFlightNumber().c_str())
                           .arg(cabinStr)
                           .arg(order.getPassenger().getName().c_str())
                           .arg(order.getPrice())
                           .arg(refundFee)
                           .arg(refundAmount), &refundDialog);
    layout.addWidget(&refundInfoLabel);

    QLabel tipLabel("温馨提示：\n根据航空公司退票规定，"
                    "航班起飞前7天以上免费退票，\n"
                    "7天内退票收取票价5%的手续费，\n"
                    "48小时内退票收取票价10%的手续费，\n"
                    "4小时内退票收取票价15%的手续费。\n"
                    "请确认是否继续退票。", &refundDialog);
    tipLabel.setWordWrap(true);
    layout.addWidget(&tipLabel);

    QHBoxLayout buttonLayout;
    QPushButton confirmButton("确认退票", &refundDialog);
    QPushButton cancelButton("取消", &refundDialog);
    buttonLayout.addWidget(&confirmButton);
    buttonLayout.addWidget(&cancelButton);
    layout.addLayout(&buttonLayout);

    connect(&confirmButton, &QPushButton::clicked, &refundDialog, [&]() {
        if (!isRefundAllowed(order, datetime)) {
            QMessageBox::warning(&refundDialog, "退票失败", "退票时间已超过规定期限，无法退票。");
            refundDialog.reject();
            return;
        }

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

double PersonalCenterWindow::calculateRefundFee(const Order &order, QDateTime &datetime) {
    double basePrice = order.getPrice();
    double feePercentage = 0.0;

    QDateTime now = QDateTime::currentDateTime();
    int hoursToFlight = now.secsTo(datetime) / 3600;

    if (hoursToFlight > 168) {
        feePercentage = 0.0;
    } else if (hoursToFlight > 48) {
        feePercentage = 0.05;
    } else if (hoursToFlight > 4) {
        feePercentage = 0.10;
    } else {
        feePercentage = 0.15;
    }

    return basePrice * feePercentage;
}

bool PersonalCenterWindow::isRefundAllowed(const Order &order, QDateTime &datetime) {
    QDateTime now = QDateTime::currentDateTime();
    return datetime > now;
}

void PersonalCenterWindow::handleChangeTicket(const Order &order) {
    QDialog *dialog = createDetailWindow("改签");

    QVBoxLayout *layout = static_cast<QVBoxLayout*>(dialog->property("contentLayout").value<void*>());

    QLabel *infoLabel = new QLabel("请选择新的日期:", dialog);
    layout->addWidget(infoLabel);

    QComboBox *dateComboBox = new QComboBox(dialog);

    // 获取航班信息
    Flight *flight = flight_map.find(order.getFlightNumber());
    if (!flight) {
        QMessageBox::warning(this, "错误", "无法找到航班信息");
        dialog->reject();
        return;
    }

    CabinType cabinType = order.getCabinType();
    const FlightScheduleMap &scheduleMap = flight->getFlightSchedule();

    // 遍历航班排班信息，寻找符合条件的日期
    scheduleMap.traverse([&](const FlightTicketDetail &ticketDetail) {
        if (ticketDetail.getRemainingTickets(cabinType) > 0) {
            dateComboBox->addItem(QString::fromUtf8(ticketDetail.getFlightDate().toString().c_str()));
        }
    });

    if (dateComboBox->count() == 0) {
        QMessageBox::warning(dialog, "提示", "没有符合条件的日期");
        dialog->reject();
        return;
    }

    layout->addWidget(dateComboBox);

    QPushButton *confirmButton = new QPushButton("确认", dialog);
    layout->addWidget(confirmButton);

    connect(confirmButton, &QPushButton::clicked, this, [=]() {
        if (dateComboBox->currentIndex() >= 0) {
            QString selectedDateStr = dateComboBox->currentText();
            Date selectedDate = Date::fromString(selectedDateStr.toStdString().c_str());

            // 获取新日期的票价信息
            FlightTicketDetail *newTicketDetail = flight->getFlightTicketDetail(selectedDate);
            if (!newTicketDetail) {
                QMessageBox::warning(dialog, "错误", "无法获取新日期的票价信息");
                return;
            }

            double originalPrice = order.getPrice();
            double newPrice = newTicketDetail->getCabinPrice(cabinType);
            double priceDifference = newPrice - originalPrice;
            QString priceDifferenceMessage;

            // 显示价格差异提示
            if (priceDifference > 0) {
                priceDifferenceMessage = QString("改签后需要补差价: %1").arg(priceDifference);
            } else if (priceDifference < 0) {
                priceDifferenceMessage = QString("改签后将退还差价: %1").arg(-priceDifference);
            } else {
                priceDifferenceMessage = "改签无差价";
            }

            int ret = QMessageBox::question(dialog, "确认改签", QString("原票价: %1\n新票价: %2\n%3\n是否确认改签？")
                                            .arg(originalPrice)
                                            .arg(newPrice)
                                            .arg(priceDifferenceMessage),
                                            QMessageBox::Yes | QMessageBox::No);
            if (ret == QMessageBox::Yes) {
                // 执行改签逻辑
                if (changeTicket(order, selectedDate)) {
                    // // 多退少补处理
                    // if (priceDifference > 0) {
                    //     processPayment(priceDifference);
                    // } else if (priceDifference < 0) {
                    //     processRefund(-priceDifference);
                    // }

                    QMessageBox::information(dialog, "成功", "改签成功！");
                    dialog->accept();
                    refreshOrderList();
                } else {
                    QMessageBox::warning(dialog, "错误", "改签失败");
                }
            }
        } else {
            QMessageBox::warning(dialog, "错误", "请选择日期");
        }
    });

    dialog->exec();
}

QDialog* PersonalCenterWindow::createDetailWindow(const QString &title) {
    QDialog *detailWindow = new QDialog(this);
    detailWindow->setWindowTitle(title);
    detailWindow->setMinimumSize(400, 300);

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
