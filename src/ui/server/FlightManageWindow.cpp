#include <ui/server/FlightManageWindow.h>
#include <file/fileManage.h>
#include <QMessageBox>
#include <QFormLayout>
#include <QTimeEdit>
#include <QDateEdit>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QScrollArea>
#include <QDoubleSpinBox>

FlightManageWindow::FlightManageWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupConnections();
    populateList();
}

void FlightManageWindow::setupUI() {
    mainLayout = new QVBoxLayout(this);

    flightListWidget = new QListWidget(this);
    flightListWidget->setStyleSheet("border: 1px solid #ccc; background-color: #f9f9f9;");
    mainLayout->addWidget(flightListWidget);

    addFlightButton = new QPushButton("添加航班", this);
    addFlightButton->setStyleSheet("background-color: #4CAF50; color: white; font-size: 14px; padding: 8px;");
    mainLayout->addWidget(addFlightButton);

    setLayout(mainLayout);
}

void FlightManageWindow::setupConnections() {
    connect(addFlightButton, &QPushButton::clicked, this, &FlightManageWindow::openAddFlightDialog);
}

void FlightManageWindow::populateList() {
    flightListWidget->clear();
    flight_map.traverse([this](const Flight &flight) { addFlightItem(flight); });
}

void FlightManageWindow::openAddFlightDialog() {
    QDialog dialog;
    dialog.setWindowTitle("添加航班");
    dialog.setFixedSize(400, 700);
    dialog.setStyleSheet("background-color: #f5f5f5; border-radius: 10px;");

    QFormLayout formLayout(&dialog);

    QLineEdit *flightNameLineEdit = createLineEdit(&dialog, "请输入航班名称");
    QLineEdit *airlineLineEdit = createLineEdit(&dialog, "请输入航空公司");
    QComboBox *airplaneModelComboBox = createComboBox(&dialog, "选择机型");
    populateComboBox(airplaneModelComboBox, airplane_model_map);

    QComboBox *routeComboBox = createComboBox(&dialog, "选择航线");
    populateComboBox(routeComboBox, air_route_map);

    QLineEdit *departureAirportLineEdit = createLineEdit(&dialog, "");
    QLineEdit *arrivalAirportLineEdit = createLineEdit(&dialog, "");
    QPushButton *swapAirportsButton = createSwapButton(&dialog, departureAirportLineEdit, arrivalAirportLineEdit);

    QTimeEdit *departureTimeEdit = createTimeEdit(&dialog, QTime::currentTime());
    QTimeEdit *costTimeEdit = createTimeEdit(&dialog, QTime(0, 0, 0));

    QDoubleSpinBox *initialFirstClassPriceSpinBox = createPriceSpinBox(&dialog);
    QDoubleSpinBox *initialBusinessClassPriceSpinBox = createPriceSpinBox(&dialog);
    QDoubleSpinBox *initialEconomyClassPriceSpinBox = createPriceSpinBox(&dialog);

    QPushButton confirmButton("确认添加航班", &dialog);
    confirmButton.setStyleSheet("background-color: #4CAF50; color: white; font-size: 14px; padding: 8px;");
    formLayout.addRow("航班名称", flightNameLineEdit);
    formLayout.addRow("航空公司", airlineLineEdit);
    formLayout.addRow("机型", airplaneModelComboBox);
    formLayout.addRow("航线", routeComboBox);
    formLayout.addRow("出发机场", departureAirportLineEdit);
    formLayout.addRow("到达机场", arrivalAirportLineEdit);
    formLayout.addRow("交换机场", swapAirportsButton);
    formLayout.addRow("出发时间", departureTimeEdit);
    formLayout.addRow("飞行时长", costTimeEdit);
    formLayout.addRow("头等舱初始价格", initialFirstClassPriceSpinBox);
    formLayout.addRow("商务舱初始价格", initialBusinessClassPriceSpinBox);
    formLayout.addRow("经济舱初始价格", initialEconomyClassPriceSpinBox);
    formLayout.addWidget(&confirmButton);

    connect(routeComboBox, &QComboBox::currentIndexChanged, this, [=](int index) {
        if (index >= 1) {
            QString routeName = routeComboBox->itemText(index);
            AirRoute* airline = air_route_map.find(routeName.toStdString().c_str());
            if (airline) {
                departureAirportLineEdit->setText(airline->getAirport1().c_str());
                arrivalAirportLineEdit->setText(airline->getAirport2().c_str());
            }
        } else {
            departureAirportLineEdit->clear();
            arrivalAirportLineEdit->clear();
        }
    });

    connect(&confirmButton, &QPushButton::clicked, this, [&]() {
        confirmAddFlight(flightNameLineEdit->text(), airlineLineEdit->text(), airplaneModelComboBox->currentText(),
                         routeComboBox->currentText(), departureAirportLineEdit->text(), arrivalAirportLineEdit->text(),
                         departureTimeEdit->time(), costTimeEdit->time(), initialFirstClassPriceSpinBox->value(),
                         initialBusinessClassPriceSpinBox->value(), initialEconomyClassPriceSpinBox->value(), &dialog);
    });

    dialog.exec();
}

void FlightManageWindow::confirmAddFlight(const QString& flightNumber, const QString& airline, const QString& airplaneModel,
                                          const QString& routeName, const QString& departureAirport, const QString& arrivalAirport,
                                          const QTime& departureTime, const QTime& costTime, double initialFirstClassPrice,
                                          double initialBusinessClassPrice, double initialEconomyClassPrice, QDialog* dialog) {
    if (validateFlightInput(flightNumber, airline, airplaneModel, routeName, departureAirport, arrivalAirport,
                            initialFirstClassPrice, initialBusinessClassPrice, initialEconomyClassPrice)) {
        Flight flight = createFlight(flightNumber, airline, airplaneModel, routeName, departureAirport, arrivalAirport,
                                     departureTime, costTime, initialFirstClassPrice, initialBusinessClassPrice, initialEconomyClassPrice);
        if (addElementToMap(flight_map, flight, FLIGHTS_PATH.c_str())) {
            String dirPath = FLIGHTS_DIR + flight.getFlightName();
            if (!createDirectory(dirPath.c_str())) {
                QMessageBox::warning(this, "错误", "无法创建航班目录。");
            }
            addFlightItem(flight);
            dialog->accept();
        } else {
            QMessageBox::warning(this, "错误", "此航班已存在！");
        }
    } else {
        QMessageBox::warning(this, "错误", "所有字段都必须填写且有效。");
    }
}

bool FlightManageWindow::validateFlightInput(const QString& flightNumber, const QString& airline, const QString& airplaneModel,
                                             const QString& routeName, const QString& departureAirport, const QString& arrivalAirport,
                                             double initialFirstClassPrice, double initialBusinessClassPrice, double initialEconomyClassPrice) const {
    return !flightNumber.isEmpty() && !airline.isEmpty() && airplaneModel != "选择机型" && routeName != "选择航线" &&
           !departureAirport.isEmpty() && !arrivalAirport.isEmpty() &&
           initialFirstClassPrice > 0 && initialBusinessClassPrice > 0 && initialEconomyClassPrice > 0;
}

Flight FlightManageWindow::createFlight(const QString& flightNumber, const QString& airline, const QString& airplaneModel,
                                        const QString& routeName, const QString& departureAirportName, const QString& arrivalAirportName,
                                        const QTime& departureTime, const QTime& costTime, double initialFirstClassPrice,
                                        double initialBusinessClassPrice, double initialEconomyClassPrice) {
    Airport* departureAirport = airport_map.find(departureAirportName.toStdString().c_str());
    Airport* arrivalAirport = airport_map.find(arrivalAirportName.toStdString().c_str());
    Time departureTimeObj(departureTime.hour(), departureTime.minute(), departureTime.second());
    Time costTimeObj(costTime.hour(), costTime.minute(), costTime.second());
    AirplaneModel* airplane_model = airplane_model_map.find(airplaneModel.toStdString().c_str());

    Flight flight(flightNumber.toStdString().c_str(), airline.toStdString().c_str(), airplane_model->getName(),
                  *departureAirport, *arrivalAirport, routeName.toStdString().c_str(), departureTimeObj, costTimeObj);
    flight.setCabin(FirstClass, airplane_model->getCabin(FirstClass));
    flight.setCabin(BusinessClass, airplane_model->getCabin(BusinessClass));
    flight.setCabin(EconomyClass, airplane_model->getCabin(EconomyClass));
    flight.setInitialPrice(FirstClass, initialFirstClassPrice);
    flight.setInitialPrice(BusinessClass, initialBusinessClassPrice);
    flight.setInitialPrice(EconomyClass, initialEconomyClassPrice);

    return flight;
}

void FlightManageWindow::addFlightItem(const Flight &flight) {
    FlightItem *item = new FlightItem(flight, flightListWidget);
    flightListWidget->addItem(item);
    connectFlightItemActions(item);
}

void FlightManageWindow::connectFlightItemActions(FlightItem* item) {
    connect(item->getDeleteButton(), &QPushButton::clicked, this, [this, item]() {
        onDeleteFlight(item);
    });
    connect(item->getManageButton(), &QPushButton::clicked, this, [this, item]() {
        onManageFlight(item);
    });
}

void FlightManageWindow::onDeleteFlight(FlightItem *item) {
    if (confirmDelete("确认删除", "确实要删除此航班吗？")) {
        deleteFlightItem(item);
    }
}

void FlightManageWindow::onManageFlight(FlightItem *item) {
    QDialog dialog;
    dialog.setWindowTitle("航班管理");
    dialog.setFixedSize(800, 800);
    dialog.setStyleSheet("background-color: #f5f5f5; border-radius: 10px;");

    QVBoxLayout layout(&dialog);
    QScrollArea scrollArea(&dialog);
    scrollArea.setWidgetResizable(true);
    QWidget contentWidget;
    QVBoxLayout contentLayout(&contentWidget);
    scrollArea.setWidget(&contentWidget);

    FlightScheduleWidget = new QListWidget(&contentWidget);
    FlightScheduleWidget->setStyleSheet("border: 1px solid #ccc; background-color: #f9f9f9;");
    contentLayout.addWidget(FlightScheduleWidget);

    QPushButton addDateButton("添加出行计划", &dialog);
    addDateButton.setStyleSheet("background-color: #4CAF50; color: white; font-size: 14px; padding: 8px;");

    layout.addWidget(&scrollArea);
    layout.addWidget(&addDateButton);

    dialog.setLayout(&layout);

    flight = flight_map.find(item->getFlightName());
    if (flight) {
        flight->getFlightSchedule().traverse([this](const FlightTicketDetail &ticketInfo) {
            addFlightScheduleItem(ticketInfo);
        });
    }

    connect(&addDateButton, &QPushButton::clicked, this, [this]() {
        openAddDateDialog();
    });

    dialog.exec();
}

void FlightManageWindow::addFlightScheduleItem(const FlightTicketDetail &detail) {
    FlightScheduleItem *scheduleItem = new FlightScheduleItem(detail, FlightScheduleWidget);
    FlightScheduleWidget->addItem(scheduleItem);
    connectFlightScheduleItemActions(scheduleItem);
}

void FlightManageWindow::connectFlightScheduleItemActions(FlightScheduleItem* item) {
    connect(item->getDeleteButton(), &QPushButton::clicked, this, [this, item]() {
        onDeleteFlightScheduleItem(item);
    });
    connect(item->getEditButton(), &QPushButton::clicked, this, [this, item]() {
        onManageFlightScheduleItem(item);
    });
}

void FlightManageWindow::onDeleteFlightScheduleItem(FlightScheduleItem* item) {
    if (confirmDelete("确认删除", "确实要删除此航班计划吗？")) {
        deleteFlightScheduleItem(item);
    }
}

void FlightManageWindow::onManageFlightScheduleItem(FlightScheduleItem* item) {
    QDialog dialog;
    dialog.setWindowTitle("管理航班计划");
    dialog.setFixedSize(400, 300);
    dialog.setStyleSheet("background-color: #f5f5f5; border-radius: 10px;");

    QVBoxLayout layout(&dialog);
    QLabel dateLabel(QString("航班日期: %1").arg(item->getFlightDate().toString().c_str()), &dialog);
    dateLabel.setStyleSheet("font-size: 16px; font-weight: bold;");
    layout.addWidget(&dateLabel);

    QDoubleSpinBox *firstClassPriceEdit = createPriceSpinBox(&dialog);
    firstClassPriceEdit->setValue(item->getFirstClassPriceLabel()->text().remove("价格: ").toDouble());

    QDoubleSpinBox *businessClassPriceEdit = createPriceSpinBox(&dialog);
    businessClassPriceEdit->setValue(item->getBusinessClassPriceLabel()->text().remove("价格: ").toDouble());

    QDoubleSpinBox *economyClassPriceEdit = createPriceSpinBox(&dialog);
    economyClassPriceEdit->setValue(item->getEconomyClassPriceLabel()->text().remove("价格: ").toDouble());

    layout.addWidget(new QLabel("头等舱票价: ", &dialog));
    layout.addWidget(firstClassPriceEdit);
    layout.addWidget(new QLabel("商务舱票价: ", &dialog));
    layout.addWidget(businessClassPriceEdit);
    layout.addWidget(new QLabel("经济舱票价: ", &dialog));
    layout.addWidget(economyClassPriceEdit);

    QPushButton saveButton("保存", &dialog);
    saveButton.setStyleSheet("background-color: #4CAF50; color: white; font-size: 14px; padding: 8px;");
    layout.addWidget(&saveButton);

    connect(&saveButton, &QPushButton::clicked, [this, item, firstClassPriceEdit, businessClassPriceEdit, economyClassPriceEdit, &dialog]() {
        updateFlightSchedule(item, firstClassPriceEdit, businessClassPriceEdit, economyClassPriceEdit);
        dialog.accept();
    });

    dialog.exec();
}

void FlightManageWindow::openAddDateDialog() {
    QDialog dialog(this);
    dialog.setWindowTitle("添加出行计划");
    dialog.setFixedSize(400, 300);
    dialog.setStyleSheet("background-color: #f5f5f5; border-radius: 10px;");

    QVBoxLayout layout(&dialog);
    QDateEdit dateEdit(&dialog);
    dateEdit.setDisplayFormat("yyyy-MM-dd");
    dateEdit.setDate(QDate::currentDate());
    dateEdit.setStyleSheet("font-size: 14px; padding: 5px;");

    QDoubleSpinBox *firstClassPriceEdit = createPriceSpinBox(&dialog);
    firstClassPriceEdit->setToolTip("头等舱价格");

    QDoubleSpinBox *businessClassPriceEdit = createPriceSpinBox(&dialog);
    businessClassPriceEdit->setToolTip("商务舱价格");

    QDoubleSpinBox *economyClassPriceEdit = createPriceSpinBox(&dialog);
    economyClassPriceEdit->setToolTip("经济舱价格");

    QPushButton confirmButton("确认", &dialog);
    confirmButton.setStyleSheet("background-color: #4CAF50; color: white; font-size: 14px; padding: 8px;");
    layout.addWidget(&dateEdit);
    layout.addWidget(firstClassPriceEdit);
    layout.addWidget(businessClassPriceEdit);
    layout.addWidget(economyClassPriceEdit);
    layout.addWidget(&confirmButton);

    connect(&confirmButton, &QPushButton::clicked, this, [&]() {
        QDate selectedDate = dateEdit.date();
        double firstClassPrice = firstClassPriceEdit->value();
        double businessClassPrice = businessClassPriceEdit->value();
        double economyClassPrice = economyClassPriceEdit->value();
        addDate(selectedDate, firstClassPrice, businessClassPrice, economyClassPrice);
        dialog.accept();
    });

    dialog.exec();
}

void FlightManageWindow::addDate(const QDate &date, double firstClassPrice, double businessClassPrice, double economyClassPrice) {
    Date flightDate = Date::fromString(date.toString("yyyy-MM-dd").toStdString().c_str());
    FlightTicketDetail detail(firstClassPrice, businessClassPrice, economyClassPrice, flight->getCabin(FirstClass).getPassengerCapacity(),
                            flight->getCabin(BusinessClass).getPassengerCapacity(), flight->getCabin(EconomyClass).getPassengerCapacity(), flightDate);
    if (flight) {
        if (flight->addFlightSchedule(detail)) {
            addFlightScheduleItem(detail);
        } else {
            QMessageBox::warning(this, "错误", "此航班计划已存在。");
        }
    }
}

void FlightManageWindow::updateFlightSchedule(FlightScheduleItem* item, QDoubleSpinBox* firstClassPriceEdit, QDoubleSpinBox* businessClassPriceEdit, QDoubleSpinBox* economyClassPriceEdit) {
    double firstClassPrice = firstClassPriceEdit->value();
    double businessClassPrice = businessClassPriceEdit->value();
    double economyClassPrice = economyClassPriceEdit->value();

    if (flight) {
        FlightTicketDetail* detail = flight->getFlightSchedule().find(item->getFlightDate());
        if (detail) {
            detail->setCabinPrice(FirstClass, firstClassPrice);
            detail->setCabinPrice(BusinessClass, businessClassPrice);
            detail->setCabinPrice(EconomyClass, economyClassPrice);
            item->setFlightDetails(*detail);
            if (writeMapToFile(flight_map, FLIGHTS_PATH.c_str())) {
                return;
            }
        }
    }
    QMessageBox::warning(this, "错误", "更新航班计划失败。");
}

bool FlightManageWindow::confirmDelete(const QString &title, const QString &message) {
    return QMessageBox::question(this, title, message, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes;
}

void FlightManageWindow::deleteFlightItem(FlightItem *item) {
    int row = flightListWidget->row(item);
    if (row != -1) {
        String key = item->getFlightName();
        if (deleteElementInMap(flight_map, key, FLIGHTS_PATH.c_str())) {
            String dirPath = FLIGHTS_DIR + key;
            if (!removeDirectory(dirPath.c_str())) {
                QMessageBox::warning(this, "错误", "无法删除航班目录。");
            }
            delete flightListWidget->takeItem(row);
        } else {
            QMessageBox::warning(this, "错误", "无法删除航班。");
        }
    }
}

void FlightManageWindow::deleteFlightScheduleItem(FlightScheduleItem* item) {
    if (flight) {
        flight->removeFlightSchedule(item->getFlightDate());
    }
    delete FlightScheduleWidget->takeItem(FlightScheduleWidget->row(item));
}

QLineEdit* FlightManageWindow::createLineEdit(QWidget* parent, const QString& placeholder) {
    QLineEdit* lineEdit = new QLineEdit(parent);
    lineEdit->setPlaceholderText(placeholder);
    lineEdit->setStyleSheet("font-size: 14px; padding: 5px; border: 1px solid #ccc; border-radius: 5px;");
    return lineEdit;
}

QComboBox* FlightManageWindow::createComboBox(QWidget* parent, const QString& defaultText) {
    QComboBox* comboBox = new QComboBox(parent);
    comboBox->addItem(defaultText);
    comboBox->setStyleSheet("font-size: 14px; padding: 5px; border: 1px solid #ccc; border-radius: 5px;");
    return comboBox;
}

template<typename Key, typename Value>
void FlightManageWindow::populateComboBox(QComboBox* comboBox, const Map<Key, Value>& map) {
    map.traverse([comboBox](const Value& value) {
        comboBox->addItem(value.getName().c_str());
    });
}

QPushButton* FlightManageWindow::createSwapButton(QWidget* parent, QLineEdit* departureAirportLineEdit, QLineEdit* arrivalAirportLineEdit) {
    QPushButton* swapButton = new QPushButton("交换机场", parent);
    swapButton->setStyleSheet("background-color: #2196F3; color: white; font-size: 14px; padding: 5px;");
    connect(swapButton, &QPushButton::clicked, this, [=]() {
        QString temp = departureAirportLineEdit->text();
        departureAirportLineEdit->setText(arrivalAirportLineEdit->text());
        arrivalAirportLineEdit->setText(temp);
    });
    return swapButton;
}

QTimeEdit* FlightManageWindow::createTimeEdit(QWidget* parent, const QTime& time) {
    QTimeEdit* timeEdit = new QTimeEdit(parent);
    timeEdit->setDisplayFormat("HH:mm:ss");
    timeEdit->setTime(time);
    timeEdit->setStyleSheet("font-size: 14px; padding: 5px;");
    return timeEdit;
}

QDoubleSpinBox* FlightManageWindow::createPriceSpinBox(QWidget* parent) {
    QDoubleSpinBox* spinBox = new QDoubleSpinBox(parent);
    spinBox->setRange(0, 1000000);
    spinBox->setPrefix("¥");
    spinBox->setDecimals(2);
    spinBox->setStyleSheet("font-size: 14px; padding: 5px;");
    return spinBox;
}

// FlightItem Implementation
FlightItem::FlightItem(const Flight &flight, QListWidget *parent)
    : QListWidgetItem(parent) {
    setupFlightItemUI(flight, parent);
}

void FlightItem::setupFlightItemUI(const Flight &flight, QListWidget *parent) {
    flightNameLabel = new QLabel(flight.getFlightName().c_str(), parent);
    airlineLabel = new QLabel(flight.getAirline().c_str(), parent);
    departureCityLabel = new QLabel(flight.getDepartureAirport().getName().c_str(), parent);
    arrivalCityLabel = new QLabel(flight.getArrivalAirport().getName().c_str(), parent);
    deleteButton = new QPushButton("删除", parent);
    manageButton = new QPushButton("管理", parent);

    flightNameLabel->setFixedWidth(100);
    airlineLabel->setFixedWidth(80);
    departureCityLabel->setFixedWidth(200);
    arrivalCityLabel->setFixedWidth(200);
    deleteButton->setFixedWidth(60);
    manageButton->setFixedWidth(60);

    deleteButton->setStyleSheet("background-color: #f44336; color: white; padding: 5px;");
    manageButton->setStyleSheet("background-color: #2196F3; color: white; padding: 5px;");

    QWidget *itemWidget = new QWidget(parent);
    QHBoxLayout *layout = new QHBoxLayout(itemWidget);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(flightNameLabel);
    layout->addWidget(airlineLabel);
    layout->addWidget(departureCityLabel);
    layout->addWidget(arrivalCityLabel);
    layout->addWidget(deleteButton);
    layout->addWidget(manageButton);

    itemWidget->setLayout(layout);
    itemWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setSizeHint(itemWidget->sizeHint());
    parent->setItemWidget(this, itemWidget);
}

QPushButton* FlightItem::getDeleteButton() {
    return deleteButton;
}

QPushButton* FlightItem::getManageButton() {
    return manageButton;
}

String FlightItem::getFlightName() {
    return flightNameLabel->text().toStdString().c_str();
}

// FlightScheduleItem Implementation
FlightScheduleItem::FlightScheduleItem(const FlightTicketDetail &detail, QListWidget *parent)
    : QListWidgetItem(parent) {
    setupFlightScheduleItemUI(detail, parent);
}

void FlightScheduleItem::setupFlightScheduleItemUI(const FlightTicketDetail &detail, QListWidget *parent) {
    dateLabel = new QLabel(detail.getFlightDate().toString().c_str(), parent);
    firstClassPriceLabel = new QLabel(QString("价格: %1").arg(detail.getCabinPrice(FirstClass)), parent);
    firstClassTicketsLabel = new QLabel(QString("余票: %1").arg(detail.getRemainingTickets(FirstClass)), parent);
    businessClassPriceLabel = new QLabel(QString("价格: %1").arg(detail.getCabinPrice(BusinessClass)), parent);
    businessClassTicketsLabel = new QLabel(QString("余票: %1").arg(detail.getRemainingTickets(BusinessClass)), parent);
    economyClassPriceLabel = new QLabel(QString("价格: %1").arg(detail.getCabinPrice(EconomyClass)), parent);
    economyClassTicketsLabel = new QLabel(QString("余票: %1").arg(detail.getRemainingTickets(EconomyClass)), parent);

    deleteButton = new QPushButton("删除", parent);
    editButton = new QPushButton("管理", parent);

    deleteButton->setStyleSheet("background-color: #f44336; color: white; padding: 5px;");
    editButton->setStyleSheet("background-color: #2196F3; color: white; padding: 5px;");

    QWidget *itemWidget = new QWidget(parent);
    QHBoxLayout *layout = new QHBoxLayout(itemWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(5);

    layout->addWidget(dateLabel);
    layout->addWidget(firstClassPriceLabel);
    layout->addWidget(firstClassTicketsLabel);
    layout->addWidget(businessClassPriceLabel);
    layout->addWidget(businessClassTicketsLabel);
    layout->addWidget(economyClassPriceLabel);
    layout->addWidget(economyClassTicketsLabel);
    layout->addWidget(deleteButton);
    layout->addWidget(editButton);

    itemWidget->setLayout(layout);
    itemWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    setSizeHint(itemWidget->sizeHint());
    parent->setItemWidget(this, itemWidget);
}

QLabel* FlightScheduleItem::getFirstClassPriceLabel() {
    return firstClassPriceLabel;
}

QLabel* FlightScheduleItem::getBusinessClassPriceLabel() {
    return businessClassPriceLabel;
}

QLabel* FlightScheduleItem::getEconomyClassPriceLabel() {
    return economyClassPriceLabel;
}

QPushButton* FlightScheduleItem::getDeleteButton() {
    return deleteButton;
}

QPushButton* FlightScheduleItem::getEditButton() {
    return editButton;
}

Date FlightScheduleItem::getFlightDate() {
    return Date::fromString(dateLabel->text().toStdString().c_str());
}

void FlightScheduleItem::setFlightDetails(const FlightTicketDetail &detail) {
    firstClassPriceLabel->setText(QString("价格: %1").arg(detail.getCabinPrice(FirstClass)));
    firstClassTicketsLabel->setText(QString("余票: %1").arg(detail.getRemainingTickets(FirstClass)));
    businessClassPriceLabel->setText(QString("价格: %1").arg(detail.getCabinPrice(BusinessClass)));
    businessClassTicketsLabel->setText(QString("余票: %1").arg(detail.getRemainingTickets(BusinessClass)));
    economyClassPriceLabel->setText(QString("价格: %1").arg(detail.getCabinPrice(EconomyClass)));
    economyClassTicketsLabel->setText(QString("余票: %1").arg(detail.getRemainingTickets(EconomyClass)));
}
