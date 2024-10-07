#include <ui/server/FlightManageWindow.h>
#include <data/datamanage.h>
#include <QMessageBox>
#include <QFormLayout>
#include <QDialog>
#include <QTimeEdit>
#include <QDateEdit>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QScrollArea>

FlightManageWindow::FlightManageWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupConnections();
    loadFlights();
}

void FlightManageWindow::setupUI() {
    mainLayout = new QVBoxLayout(this);

    flightListWidget = new QListWidget(this);
    mainLayout->addWidget(flightListWidget);

    addFlightButton = new QPushButton("添加航班", this);
    mainLayout->addWidget(addFlightButton);

    setLayout(mainLayout);
}

void FlightManageWindow::setupConnections() {
    connect(addFlightButton, &QPushButton::clicked, this, &FlightManageWindow::openAddFlightDialog);
}

void FlightManageWindow::loadFlights() {
    flight_map.traverse([this](const Flight& flight) {
        addFlightItem(flight);
    });
}

void FlightManageWindow::openAddFlightDialog() {
    QDialog *addFlightDialog = new QDialog(this);
    addFlightDialog->setWindowTitle("添加航班");
    setupAddFlightDialog(addFlightDialog);
    addFlightDialog->exec();
}

void FlightManageWindow::setupAddFlightDialog(QDialog *addFlightDialog) {
    QFormLayout *formLayout = new QFormLayout(addFlightDialog);

    QLineEdit *flightNameLineEdit = createLineEdit(addFlightDialog, "请输入航班名称");
    QLineEdit *airlineLineEdit = createLineEdit(addFlightDialog, "请输入航空公司");
    QComboBox *airplaneModelComboBox = createComboBox(addFlightDialog, "选择机型");
    populateComboBox(airplaneModelComboBox, airplane_model_map);

    QComboBox *routeComboBox = createComboBox(addFlightDialog, "选择航线");
    populateComboBox(routeComboBox, airline_map);

    QLineEdit *departureAirportLineEdit = createLineEdit(addFlightDialog, "");
    QLineEdit *arrivalAirportLineEdit = createLineEdit(addFlightDialog, "");
    QPushButton *swapAirportsButton = createSwapButton(addFlightDialog, departureAirportLineEdit, arrivalAirportLineEdit);

    QTimeEdit *departureTimeEdit = createTimeEdit(addFlightDialog, QTime::currentTime());
    QTimeEdit *costTimeEdit = createTimeEdit(addFlightDialog, QTime(0, 0, 0));

    QPushButton *confirmAddFlightButton = new QPushButton("确认添加航班", addFlightDialog);
    formLayout->addRow("航班名称", flightNameLineEdit);
    formLayout->addRow("航空公司", airlineLineEdit);
    formLayout->addRow("机型", airplaneModelComboBox);
    formLayout->addRow("航线", routeComboBox);
    formLayout->addRow("出发机场", departureAirportLineEdit);
    formLayout->addRow("到达机场", arrivalAirportLineEdit);
    formLayout->addRow("交换机场", swapAirportsButton);
    formLayout->addRow("出发时间", departureTimeEdit);
    formLayout->addRow("飞行时长", costTimeEdit);
    formLayout->addWidget(confirmAddFlightButton);

    addFlightDialog->setLayout(formLayout);
    addFlightDialog->resize(400, 600);

    connect(routeComboBox, &QComboBox::currentIndexChanged, this, [=](int index) {
        if (index >= 1) {
            QString routeName = routeComboBox->itemText(index);
            Airline* airline = airline_map.find(routeName.toStdString().c_str());
            if (airline) {
                departureAirportLineEdit->setText(QString::fromStdString(airline->getAirport1().c_str()));
                arrivalAirportLineEdit->setText(QString::fromStdString(airline->getAirport2().c_str()));
            }
        }
        else {
            departureAirportLineEdit->clear();
            arrivalAirportLineEdit->clear();
        }
    });
    connect(confirmAddFlightButton, &QPushButton::clicked, this, [=]() {
        confirmAddFlight(flightNameLineEdit->text(), airlineLineEdit->text(), airplaneModelComboBox->currentText(), routeComboBox->currentText(), departureTimeEdit->time(), costTimeEdit->time(), addFlightDialog);
    });
}

void FlightManageWindow::confirmAddFlight(const QString& flightName, const QString& airline, const QString& airplaneModel, const QString& routeName, const QTime& departureTime, const QTime& costTime, QDialog* dialog) {
    if (validateFlightInput(flightName, airline, airplaneModel, routeName)) {
        Flight flight = createFlight(flightName, airline, airplaneModel, routeName, departureTime, costTime);
        addFlight(flight);
        addFlightItem(flight);
        dialog->accept();
    } else {
        QMessageBox::warning(this, "错误", "所有字段都必须填写。");
    }
}

bool FlightManageWindow::validateFlightInput(const QString& flightName, const QString& airline, const QString& airplaneModel, const QString& routeName) const {
    return !flightName.isEmpty() && !airline.isEmpty() && airplaneModel != "选择机型" && routeName != "选择航线" && departureAirport && arrivalAirport;
}

Flight FlightManageWindow::createFlight(const QString& flightName, const QString& airline, const QString& airplaneModel, const QString& routeName, const QTime& departureTime, const QTime& costTime) const {
    Time departureTimeObj(departureTime.hour(), departureTime.minute(), departureTime.second());
    Time costTimeObj(costTime.hour(), costTime.minute(), costTime.second());
    AirplaneModel* airplane_model = airplane_model_map.find(airplaneModel.toStdString().c_str());

    Flight flight(flightName.toStdString().c_str(), airline.toStdString().c_str(), airplane_model->getName(), *departureAirport, *arrivalAirport, routeName.toStdString().c_str(), departureTimeObj, costTimeObj);
    flight.setCabin(Flight::FirstClass, airplane_model->getCabin(AirplaneModel::FirstClass));
    flight.setCabin(Flight::BusinessClass, airplane_model->getCabin(AirplaneModel::BusinessClass));
    flight.setCabin(Flight::EconomyClass, airplane_model->getCabin(AirplaneModel::EconomyClass));

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
    QDialog *manageFlightDialog = new QDialog(this);
    manageFlightDialog->setWindowTitle("航班管理");
    setupManageFlightDialog(manageFlightDialog, item);
    manageFlightDialog->exec();
}

void FlightManageWindow::setupManageFlightDialog(QDialog *dialog, FlightItem *item) {
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    QScrollArea *scrollArea = new QScrollArea(dialog);
    scrollArea->setWidgetResizable(true);
    QWidget *contentWidget = new QWidget();
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    scrollArea->setWidget(contentWidget);

    FlightScheduleWidget = new QListWidget(contentWidget);
    contentLayout->addWidget(FlightScheduleWidget);

    QPushButton *addDateButton = new QPushButton("添加出行计划", dialog);
    layout->addWidget(scrollArea);
    layout->addWidget(addDateButton);

    dialog->setLayout(layout);
    dialog->resize(800, 800);

    flight = flight_map.find(item->getFlightName());
    flight->getFlightSchedule().traverse([this](const FlightTicketDetail &ticketInfo) {
        addFlightScheduleItem(ticketInfo);
    });

    connect(addDateButton, &QPushButton::clicked, this, [this]() {
        openAddDateDialog();
    });
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
    QDialog *editScheduleDialog = new QDialog(this);
    editScheduleDialog->setWindowTitle("管理航班计划");

    QVBoxLayout *layout = new QVBoxLayout(editScheduleDialog);
    QLabel *dateLabel = new QLabel(QString("航班日期: %1").arg(QString::fromStdString(item->getFlightDate().toString().c_str())), editScheduleDialog);
    layout->addWidget(dateLabel);

    QLineEdit *firstClassPriceEdit = createLineEdit(editScheduleDialog, "头等舱票价");
    QLineEdit *businessClassPriceEdit = createLineEdit(editScheduleDialog, "商务舱票价");
    QLineEdit *economyClassPriceEdit = createLineEdit(editScheduleDialog, "经济舱票价");

    firstClassPriceEdit->setText(item->getFirstClassPriceLabel()->text());
    businessClassPriceEdit->setText(item->getBusinessClassPriceLabel()->text());
    economyClassPriceEdit->setText(item->getEconomyClassPriceLabel()->text());

    layout->addWidget(new QLabel("头等舱票价: ", editScheduleDialog));
    layout->addWidget(firstClassPriceEdit);
    layout->addWidget(new QLabel("商务舱票价: ", editScheduleDialog));
    layout->addWidget(businessClassPriceEdit);
    layout->addWidget(new QLabel("经济舱票价: ", editScheduleDialog));
    layout->addWidget(economyClassPriceEdit);

    QPushButton *saveButton = new QPushButton("保存", editScheduleDialog);
    layout->addWidget(saveButton);

    connect(saveButton, &QPushButton::clicked, [this, item, firstClassPriceEdit, businessClassPriceEdit, economyClassPriceEdit, editScheduleDialog]() {
        updateFlightSchedule(item, firstClassPriceEdit, businessClassPriceEdit, economyClassPriceEdit);
        editScheduleDialog->accept();
    });

    editScheduleDialog->setLayout(layout);
    editScheduleDialog->exec();
}

void FlightManageWindow::openAddDateDialog() {
    QDialog *addDateDialog = new QDialog(this);
    addDateDialog->setWindowTitle("添加出行计划");

    QVBoxLayout *layout = new QVBoxLayout(addDateDialog);
    QDateEdit *dateEdit = new QDateEdit(addDateDialog);
    dateEdit->setDisplayFormat("yyyy-MM-dd");
    dateEdit->setDate(QDate::currentDate());

    QLineEdit *firstClassPriceEdit = new QLineEdit(addDateDialog);
    firstClassPriceEdit->setPlaceholderText("头等舱价格");

    QLineEdit *businessClassPriceEdit = new QLineEdit(addDateDialog);
    businessClassPriceEdit->setPlaceholderText("商务舱价格");

    QLineEdit *economyClassPriceEdit = new QLineEdit(addDateDialog);
    economyClassPriceEdit->setPlaceholderText("经济舱价格");

    QPushButton *confirmButton = new QPushButton("确认", addDateDialog);
    layout->addWidget(dateEdit);
    layout->addWidget(firstClassPriceEdit);
    layout->addWidget(businessClassPriceEdit);
    layout->addWidget(economyClassPriceEdit);
    layout->addWidget(confirmButton);

    addDateDialog->setLayout(layout);

    connect(confirmButton, &QPushButton::clicked, this, [=]() {
        QDate selectedDate = dateEdit->date();
        double firstClassPrice = firstClassPriceEdit->text().toDouble();
        double businessClassPrice = businessClassPriceEdit->text().toDouble();
        double economyClassPrice = economyClassPriceEdit->text().toDouble();
        addDateDialog->accept();
    });
    addDateDialog->exec();
}


void FlightManageWindow::updateFlightSchedule(FlightScheduleItem* item, QLineEdit* firstClassPriceEdit, QLineEdit* businessClassPriceEdit, QLineEdit* economyClassPriceEdit) {
    double firstClassPrice = firstClassPriceEdit->text().toDouble();
    double businessClassPrice = businessClassPriceEdit->text().toDouble();
    double economyClassPrice = economyClassPriceEdit->text().toDouble();

    if (flight) {
        FlightTicketDetail* detail = flight->getFlightSchedule().find(item->getFlightDate());
        if (detail) {
            detail->setCabinPrice(FlightTicketDetail::FirstClass, firstClassPrice);
            detail->setCabinPrice(FlightTicketDetail::BusinessClass, businessClassPrice);
            detail->setCabinPrice(FlightTicketDetail::EconomyClass, economyClassPrice);
            item->setFlightDetails(*detail);
            return ;
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
        deleteFlight(item->getFlightName());
        delete flightListWidget->takeItem(row);
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
    return lineEdit;
}

QComboBox* FlightManageWindow::createComboBox(QWidget* parent, const QString& defaultText) {
    QComboBox* comboBox = new QComboBox(parent);
    comboBox->addItem(defaultText);
    return comboBox;
}

template<typename Key, typename Value>
void FlightManageWindow::populateComboBox(QComboBox* comboBox, const Map<Key, Value>& map) {
    map.traverse([comboBox](const Value& value) {
        comboBox->addItem(QString::fromStdString(value.getName().c_str()));
    });
}

QPushButton* FlightManageWindow::createSwapButton(QWidget* parent, QLineEdit* departureAirportLineEdit, QLineEdit* arrivalAirportLineEdit) {
    QPushButton* swapButton = new QPushButton("交换机场", parent);
    connect(swapButton, &QPushButton::clicked, this, [=]() {
        QString temp = departureAirportLineEdit->text();
        departureAirportLineEdit->setText(arrivalAirportLineEdit->text());
        arrivalAirportLineEdit->setText(temp);

        std::swap(departureAirport, arrivalAirport);
    });
    return swapButton;
}

QTimeEdit* FlightManageWindow::createTimeEdit(QWidget* parent, const QTime& time) {
    QTimeEdit* timeEdit = new QTimeEdit(parent);
    timeEdit->setDisplayFormat("HH:mm:ss");
    timeEdit->setTime(time);
    return timeEdit;
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
    departureTimeLabel = new QLabel(flight.getDepartureTime().toString().c_str(), parent);

    deleteButton = new QPushButton("删除", parent);
    manageButton = new QPushButton("管理", parent);

    QWidget *itemWidget = new QWidget(parent);
    QHBoxLayout *layout = new QHBoxLayout(itemWidget);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(flightNameLabel);
    layout->addWidget(airlineLabel);
    layout->addWidget(departureCityLabel);
    layout->addWidget(arrivalCityLabel);
    layout->addWidget(departureTimeLabel);
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
    dateLabel = new QLabel(QString::fromStdString(detail.getFlightDate().toString().c_str()), parent);
    firstClassPriceLabel = new QLabel(QString("价格: %1").arg(detail.getCabinPrice(FlightTicketDetail::FirstClass)), parent);
    firstClassTicketsLabel = new QLabel(QString("余票: %1").arg(detail.getRemainingTickets(FlightTicketDetail::FirstClass)), parent);
    businessClassPriceLabel = new QLabel(QString("价格: %1").arg(detail.getCabinPrice(FlightTicketDetail::BusinessClass)), parent);
    businessClassTicketsLabel = new QLabel(QString("余票: %1").arg(detail.getRemainingTickets(FlightTicketDetail::BusinessClass)), parent);
    economyClassPriceLabel = new QLabel(QString("价格: %1").arg(detail.getCabinPrice(FlightTicketDetail::EconomyClass)), parent);
    economyClassTicketsLabel = new QLabel(QString("余票: %1").arg(detail.getRemainingTickets(FlightTicketDetail::EconomyClass)), parent);

    deleteButton = new QPushButton("删除", parent);
    editButton = new QPushButton("管理", parent);

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
    firstClassPriceLabel->setText(QString("价格: %1").arg(detail.getCabinPrice(FlightTicketDetail::FirstClass)));
    firstClassTicketsLabel->setText(QString("余票: %1").arg(detail.getRemainingTickets(FlightTicketDetail::FirstClass)));
    businessClassPriceLabel->setText(QString("价格: %1").arg(detail.getCabinPrice(FlightTicketDetail::BusinessClass)));
    businessClassTicketsLabel->setText(QString("余票: %1").arg(detail.getRemainingTickets(FlightTicketDetail::BusinessClass)));
    economyClassPriceLabel->setText(QString("价格: %1").arg(detail.getCabinPrice(FlightTicketDetail::EconomyClass)));
    economyClassTicketsLabel->setText(QString("余票: %1").arg(detail.getRemainingTickets(FlightTicketDetail::EconomyClass)));
}
