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
    if (order.getSeatNum() != "NULL")
        selectSeatButton->setEnabled(false);
    
    if (order.getStatus() == REFUNDED) {
        selectSeatButton->setEnabled(false);
        orderMealButton->setEnabled(false);
        refundTicketButton->setEnabled(false);
        changeTicketButton->setEnabled(false);
    }
        
    if (order.getMeal() != NO_MEAL)
        orderMealButton->setEnabled(false);
    
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

    ticketListWidget = new QListWidget(this);
    ticketListWidget->setStyleSheet("font-size: 16px;");
    ticketListWidget->setUniformItemSizes(false);
    ticketListWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ticketListWidget->setFocusPolicy(Qt::NoFocus);
    connect(ticketListWidget, &QListWidget::itemClicked, this, &PersonalCenterWindow::onOrdertItemClicked);

    mainLayout->addWidget(ticketListWidget);
    setLayout(mainLayout);
}

void PersonalCenterWindow::populateOrderList() {
    ticketListWidget->clear();

    order_map.traverse([this](const Order &order) {
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

    QListWidgetItem *listItem = new QListWidgetItem(ticketListWidget);
    listItem->setSizeHint(itemWidget->sizeHint());

    ticketListWidget->addItem(listItem);
    ticketListWidget->setItemWidget(listItem, itemWidget);
}

void PersonalCenterWindow::onOrdertItemClicked(QListWidgetItem *item) {
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
    int ret = QMessageBox::question(this, "退票确认", "确定要退票吗？", QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        if (refundTicket(order)) {
            QMessageBox::information(this, "成功", "退票成功！");
            refreshOrderList();
        } else {
            QMessageBox::warning(this, "错误", "退票失败");
        }
    }
}

void PersonalCenterWindow::handleChangeTicket(const Order &order) {
    QDialog *dialog = createDetailWindow("改签");

    QVBoxLayout *layout = static_cast<QVBoxLayout*>(dialog->property("contentLayout").value<void*>());

    QLabel *infoLabel = new QLabel("请选择新的日期:", dialog);
    layout->addWidget(infoLabel);

    QComboBox *dateComboBox = new QComboBox(dialog);

    const Flight *flight = flight_map.find(order.getFlightNumber());
    if (!flight) {
        QMessageBox::warning(this, "错误", "无法找到航班信息");
        dialog->reject();
        return;
    }

    CabinType cabinType = order.getCabinType();
    const FlightScheduleMap &scheduleMap = flight->getFlightSchedule();
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

            if (changeTicket(order, selectedDate)) {
                QMessageBox::information(dialog, "成功", "改签成功！");
                dialog->accept();
                refreshOrderList();
            } else {
                QMessageBox::warning(dialog, "错误", "改签失败");
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
