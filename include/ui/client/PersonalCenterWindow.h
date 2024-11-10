#pragma once

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QScrollArea>
#include <FlightSystem/Order.h>
#include <FlightSystem/Ticket.h>
#include <FlightSystem/Flight.h>
#include <FlightSystem/Passenger.h>
#include <FlightSystem/Airport.h>
#include <ui/client/TicketBookingWindow.h>
#include <String/String.h>
#include <file/fileManage.h>

class OrderItem : public QWidget {
    Q_OBJECT

public:
    explicit OrderItem(const Order &order, QWidget *parent = nullptr);

signals:
    void selectSeat(const Order &order, const Flight* flight);
    void orderMeal(const Order &order);
    void refundTicket(const Order &order);
    void changeTicket(const Order &order);

private:
    void setupUI();
    void setupConnecttions();

    QLabel *flightNumberLabel;
    QLabel *dateLabel;
    QLabel *departureCityLabel;
    QLabel *arrivalCityLabel;
    QLabel *departureTimeLabel;
    QLabel *arrivalTimeLabel;
    QLabel *seatNumLabel;
    QLabel *statusLabel;

    QPushButton *selectSeatButton;
    QPushButton *orderMealButton;
    QPushButton *refundTicketButton;
    QPushButton *changeTicketButton;
    
    Order order;
    Flight *flight;
    FlightTicketDetail *flightTicketDetail;
};

class PersonalCenterWindow : public QWidget {
    Q_OBJECT

public:
    explicit PersonalCenterWindow(QWidget *parent = nullptr);
    void refreshOrderList();

private slots:
    void onOrdertItemClicked(QListWidgetItem *item);
    void handleSelectSeat(const Order &order, const Flight* flight);
    void handleOrderMeal(const Order &order);
    void handleRefundTicket(const Order &order);
    void handleChangeTicket(const Order &order);

private:
    void setupUI();
    void populateOrderList();
    void addOrderItem(const Order &order);
    QDialog* createDetailWindow(const QString &title);

    QListWidget *ticketListWidget;
};
