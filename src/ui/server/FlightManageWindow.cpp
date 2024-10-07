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
    flight_map.traverse([this](const Flight& flight){ addFlightItem(flight); });
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
    connect(addFlightButton, &QPushButton::clicked, this, [this]() {
        QDialog *addFlightDialog = new QDialog(this);
        addFlightDialog->setWindowTitle("添加航班");

        flightNameLineEdit = new QLineEdit(addFlightDialog);
        flightNameLineEdit->setPlaceholderText("请输入航班名称");

        airlineLineEdit = new QLineEdit(addFlightDialog);
        airlineLineEdit->setPlaceholderText("请输入航空公司");

        airplaneModelComboBox = new QComboBox(addFlightDialog);
        airplaneModelComboBox->addItem("选择机型");
        airplane_model_map.traverse([this](const AirplaneModel& model) {
            airplaneModelComboBox->addItem(QString::fromStdString(model.getName().c_str()));
        });

        routeComboBox = new QComboBox(addFlightDialog);
        routeComboBox->addItem("选择航线");
        airline_map.traverse([this](const Airline& route) {
            routeComboBox->addItem(QString::fromStdString(route.getName().c_str()));
        });

        QLineEdit *departureAirportLineEdit = new QLineEdit(addFlightDialog);
        QLineEdit *arrivalAirportLineEdit = new QLineEdit(addFlightDialog);
        QPushButton *swapAirportsButton = new QPushButton("交换机场", addFlightDialog);

        connect(routeComboBox, &QComboBox::currentTextChanged, this, [this, departureAirportLineEdit, arrivalAirportLineEdit](const QString &routeName) {
            Airline *airline_p = airline_map.find(routeName.toStdString().c_str());
            if (airline_p) {
                this->departureAirport = airport_map.find(airline_p->getAirport1());
                this->arrivalAirport = airport_map.find(airline_p->getAirport2());
                if (departureAirport && arrivalAirport) {
                    departureAirportLineEdit->setText(QString::fromStdString(departureAirport->getName().c_str()));
                    arrivalAirportLineEdit->setText(QString::fromStdString(arrivalAirport->getName().c_str()));
                }
            }
        });

        connect(swapAirportsButton, &QPushButton::clicked, this, [this, departureAirportLineEdit, arrivalAirportLineEdit]() {
            std::swap(departureAirport, arrivalAirport);
            if (departureAirport && arrivalAirport) {
                departureAirportLineEdit->setText(QString::fromStdString(departureAirport->getName().c_str()));
                arrivalAirportLineEdit->setText(QString::fromStdString(arrivalAirport->getName().c_str()));
            }
        });

        QTimeEdit *departureTimeEdit = new QTimeEdit(addFlightDialog);
        departureTimeEdit->setDisplayFormat("HH:MM:SS");
        departureTimeEdit->setTime(QTime::currentTime());

        QTimeEdit *costTimeEdit = new QTimeEdit(addFlightDialog);
        costTimeEdit->setDisplayFormat("HH:MM:SS");
        costTimeEdit->setTime(QTime(0, 0, 0));

        QPushButton *confirmAddFlightButton = new QPushButton("确认添加航班", addFlightDialog);

        QFormLayout *formLayout = new QFormLayout(addFlightDialog);
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

        connect(confirmAddFlightButton, &QPushButton::clicked, this, [this, addFlightDialog, departureTimeEdit, costTimeEdit]() {
            QString flightName = flightNameLineEdit->text();
            QString airline = airlineLineEdit->text();
            QString airplaneModel = airplaneModelComboBox->currentText();
            QString routeName = routeComboBox->currentText();
            QTime departureTime = departureTimeEdit->time();
            QTime costTime = costTimeEdit->time();

            if (flightName.isEmpty() || airline.isEmpty() || airplaneModel == "选择机型" || routeName == "选择航线" || !departureAirport || !arrivalAirport) {
                QMessageBox::warning(this, "错误", "所有字段都必须填写。");
                return;
            }

            Time departureTimeObj(departureTime.hour(), departureTime.minute(), departureTime.second());
            Time costTimeObj(costTime.hour(), costTime.minute(), costTime.second());
            AirplaneModel* airplane_model = airplane_model_map.find(airplaneModel.toStdString().c_str());

            Flight flight(flightName.toStdString().c_str(), airline.toStdString().c_str(), airplane_model->getName(), *departureAirport, *arrivalAirport, routeName.toStdString().c_str(), departureTimeObj, costTimeObj);
            flight.setCabin(Flight::FirstClass, airplane_model->getCabin(AirplaneModel::FirstClass));
            flight.setCabin(Flight::BusinessClass, airplane_model->getCabin(AirplaneModel::BusinessClass));
            flight.setCabin(Flight::EconomyClass, airplane_model->getCabin(AirplaneModel::EconomyClass));
            addFlight(flight);

            addFlightItem(flight);
            addFlightDialog->accept();
        });

        addFlightDialog->exec();
    });
}

void FlightManageWindow::addFlightItem(const Flight &flight) {
    FlightItem *item = new FlightItem(flight, flightListWidget);
    flightListWidget->addItem(item);
    connect(item->getDeleteButton(), &QPushButton::clicked, this, [this, item]() {
        onDeleteFlight(item);
    });
    connect(item->getManageButton(), &QPushButton::clicked, this, [this, item]() {
        onManageFlight(item);
    });
}

void FlightManageWindow::onDeleteFlight(FlightItem *item) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除", "确实要删除此航班吗？", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        int row = flightListWidget->row(item);
        if (row != -1) {
            deleteFlight(item->getFlightName());
            delete flightListWidget->takeItem(row);
        }
    }
}

void FlightManageWindow::onManageFlight(FlightItem *item) {
    QDialog *manageFlightDialog = new QDialog(this);
    manageFlightDialog->setWindowTitle("航班管理");

    QVBoxLayout *layout = new QVBoxLayout(manageFlightDialog);

    QScrollArea *scrollArea = new QScrollArea(manageFlightDialog);
    scrollArea->setWidgetResizable(true);
    QWidget *contentWidget = new QWidget();
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    scrollArea->setWidget(contentWidget);

    FlightScheduleWidget = new QListWidget(contentWidget);
    contentLayout->addWidget(FlightScheduleWidget);

    QPushButton *addDateButton = new QPushButton("添加出行计划", manageFlightDialog);
    layout->addWidget(scrollArea);
    layout->addWidget(addDateButton);

    manageFlightDialog->setLayout(layout);
    manageFlightDialog->resize(800, 800);

    flight = flight_map.find(item->getFlightName());
    flight->getFlightSchedule().traverse([this](const FlightTicketDetail &ticketInfo) {
        addFlightScheduleItem(ticketInfo);
    });

    connect(addDateButton, &QPushButton::clicked, this, [this]() {
        QDialog *addDateDialog = new QDialog();
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

        connect(confirmButton, &QPushButton::clicked, addDateDialog, [addDateDialog, dateEdit, firstClassPriceEdit, businessClassPriceEdit, economyClassPriceEdit, this]() {
            Date date(dateEdit->date().year(), dateEdit->date().month(), dateEdit->date().day());
            double firstClassPrice = firstClassPriceEdit->text().toDouble();
            double businessClassPrice = businessClassPriceEdit->text().toDouble();
            double economyClassPrice = economyClassPriceEdit->text().toDouble();

            FlightTicketDetail ticketDetail(firstClassPrice, businessClassPrice, economyClassPrice, 0, 0, 0, date);
            ticketDetail.setRemainingTickets(FlightTicketDetail::FirstClass, flight->getCabin(Flight::FirstClass).getPassengerCapacity());
            ticketDetail.setRemainingTickets(FlightTicketDetail::BusinessClass, flight->getCabin(Flight::BusinessClass).getPassengerCapacity());
            ticketDetail.setRemainingTickets(FlightTicketDetail::EconomyClass, flight->getCabin(Flight::EconomyClass).getPassengerCapacity());

            if (flight->addFlightSchedule(ticketDetail)) {
                addFlightScheduleItem(ticketDetail);
                addDateDialog->accept();
            } else {
                QMessageBox::warning(this, "错误", "该日期的航班计划已存在，无法重复添加。");
            }
        });
        addDateDialog->exec();
    });
    manageFlightDialog->exec();
}

void FlightManageWindow::addFlightScheduleItem(const FlightTicketDetail &detail) {
    FlightScheduleItem *scheduleItem = new FlightScheduleItem(detail, FlightScheduleWidget);
    FlightScheduleWidget->addItem(scheduleItem);
    connect(scheduleItem->getDeleteButton(), &QPushButton::clicked, this, [this, scheduleItem]() {
        onDeleteFlightScheduleItem(scheduleItem);
    });
    connect(scheduleItem->getEditButton(), &QPushButton::clicked, this, [this, scheduleItem, detail]() {
        onManageFlightScheduleItem(scheduleItem);
    });
}

void FlightManageWindow::onManageFlightScheduleItem(FlightScheduleItem* item) {
    QDialog *editScheduleDialog = new QDialog(this);
    editScheduleDialog->setWindowTitle("管理航班计划");

    QVBoxLayout *layout = new QVBoxLayout(editScheduleDialog);

    QLabel *dateLabel = new QLabel(QString("航班日期: %1").arg(QString::fromStdString(item->getFlightDate().toString().c_str())), editScheduleDialog);
    layout->addWidget(dateLabel);

    QLineEdit *firstClassPriceEdit = new QLineEdit(editScheduleDialog);
    firstClassPriceEdit->setPlaceholderText("头等舱票价");
    firstClassPriceEdit->setText(item->getFirstClassPriceLabel()->text());

    QLineEdit *businessClassPriceEdit = new QLineEdit(editScheduleDialog);
    businessClassPriceEdit->setPlaceholderText("商务舱票价");
    businessClassPriceEdit->setText(item->getBusinessClassPriceLabel()->text());

    QLineEdit *economyClassPriceEdit = new QLineEdit(editScheduleDialog);
    economyClassPriceEdit->setPlaceholderText("经济舱票价");
    economyClassPriceEdit->setText(item->getEconomyClassPriceLabel()->text());

    layout->addWidget(new QLabel("头等舱票价: ", editScheduleDialog));
    layout->addWidget(firstClassPriceEdit);
    layout->addWidget(new QLabel("商务舱票价: ", editScheduleDialog));
    layout->addWidget(businessClassPriceEdit);
    layout->addWidget(new QLabel("经济舱票价: ", editScheduleDialog));
    layout->addWidget(economyClassPriceEdit);

    QPushButton *saveButton = new QPushButton("保存", editScheduleDialog);
    layout->addWidget(saveButton);

    editScheduleDialog->setLayout(layout);

    connect(saveButton, &QPushButton::clicked, [this, item, firstClassPriceEdit, businessClassPriceEdit, economyClassPriceEdit, editScheduleDialog]() {
        // double firstClassPrice = firstClassPriceEdit->text().toDouble();
        // double businessClassPrice = businessClassPriceEdit->text().toDouble();
        // double economyClassPrice = economyClassPriceEdit->text().toDouble();

        // FlightTicketDetail updatedDetail = item->getFlightDetails();
        // updatedDetail.setCabinPrice(FlightTicketDetail::FirstClass, firstClassPrice);
        // updatedDetail.setCabinPrice(FlightTicketDetail::BusinessClass, businessClassPrice);
        // updatedDetail.setCabinPrice(FlightTicketDetail::EconomyClass, economyClassPrice);

        // if (flight) {
        //     flight->updateFlightSchedule(updatedDetail);
        // }
        // item->setFlightDetails(updatedDetail);
        // editScheduleDialog->accept();
    });

    editScheduleDialog->exec();
}


void FlightManageWindow::onDeleteFlightScheduleItem(FlightScheduleItem* item) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除", "确实要删除此航班计划吗？", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (flight) {
            flight->removeFlightSchedule(item->getFlightDate());
        }
        delete FlightScheduleWidget->takeItem(FlightScheduleWidget->row(item));
    }
}


FlightManageWindow::FlightItem::FlightItem(const Flight &flight, QListWidget *parent)
    : QListWidgetItem(parent) {
    deleteButton = new QPushButton("删除", parent);
    manageButton = new QPushButton("管理", parent);

    flightNameLabel = new QLabel(flight.getFlightName().c_str(), parent);
    airlineLabel = new QLabel(flight.getAirline().c_str(), parent);
    departureCityLabel = new QLabel(flight.getDepartureAirport().getName().c_str(), parent);
    arrivalCityLabel = new QLabel(flight.getArrivalAirport().getName().c_str(), parent);
    departureTimeLabel = new QLabel(flight.getDepartureTime().toString().c_str(), parent);

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

QPushButton* FlightManageWindow::FlightItem::getDeleteButton() {
    return deleteButton;
}

QPushButton* FlightManageWindow::FlightItem::getManageButton() {
    return manageButton;
}

String FlightManageWindow::FlightItem::getFlightName() {
    return flightNameLabel->text().toStdString().c_str();
}

FlightManageWindow::FlightScheduleItem::FlightScheduleItem(const FlightTicketDetail &detail, QListWidget *parent)
    : QListWidgetItem(parent) {
    std::cout << detail;
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

QLabel* FlightManageWindow::FlightScheduleItem::getFirstClassPriceLabel() {
    return firstClassPriceLabel;
}

QLabel* FlightManageWindow::FlightScheduleItem::getBusinessClassPriceLabel() {
    return businessClassPriceLabel;
}

QLabel* FlightManageWindow::FlightScheduleItem::getEconomyClassPriceLabel() {
    return economyClassPriceLabel;
}

QPushButton* FlightManageWindow::FlightScheduleItem::getDeleteButton() {
    return deleteButton;
}

QPushButton* FlightManageWindow::FlightScheduleItem::getEditButton() {
    return editButton;
}

Date FlightManageWindow::FlightScheduleItem::getFlightDate() {
    return Date::fromString(dateLabel->text().toStdString().c_str());
}

void FlightManageWindow::FlightScheduleItem::setFlightDetails(const FlightTicketDetail &detail) {
    firstClassPriceLabel->setText(QString("价格: %1").arg(detail.getCabinPrice(FlightTicketDetail::FirstClass)));
    firstClassTicketsLabel->setText(QString("余票: %1").arg(detail.getRemainingTickets(FlightTicketDetail::FirstClass)));
    businessClassPriceLabel->setText(QString("价格: %1").arg(detail.getCabinPrice(FlightTicketDetail::BusinessClass)));
    businessClassTicketsLabel->setText(QString("余票: %1").arg(detail.getRemainingTickets(FlightTicketDetail::BusinessClass)));
    economyClassPriceLabel->setText(QString("价格: %1").arg(detail.getCabinPrice(FlightTicketDetail::EconomyClass)));
    economyClassTicketsLabel->setText(QString("余票: %1").arg(detail.getRemainingTickets(FlightTicketDetail::EconomyClass)));
}