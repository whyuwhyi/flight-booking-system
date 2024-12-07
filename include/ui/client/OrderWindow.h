#pragma once

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialog>
#include <QComboBox>
#include <QDateEdit>
#include <FlightSystem/Order.h>
#include <FlightSystem/Ticket.h>
#include <vector>

class OrderItem : public QWidget {
    Q_OBJECT

public:
    explicit OrderItem(const Order &order, QWidget *parent = nullptr);
    const Order& getOrder() const;

signals:
    void selectSeat(const Order &order, int segmentIndex, const Ticket &ticket);
    void orderMeal(const Order &order, int segmentIndex, const Ticket &ticket);
    void refundTicket(const Order &order);
    void changeTicket(const Order &order);

private:
    void setupUI();
    void setupConnections();

    const Order& order;

    std::vector<QLabel*> flightNumberLabels;
    std::vector<QLabel*> dateLabels;
    std::vector<QLabel*> departureCityLabels;
    std::vector<QLabel*> arrivalCityLabels;
    std::vector<QLabel*> departureTimeLabels;
    std::vector<QLabel*> arrivalTimeLabels;
    std::vector<QLabel*> seatNumLabels;
    std::vector<QLabel*> mealLabels;

    QLabel *statusLabel;

    std::vector<QPushButton*> selectSeatButtons;
    std::vector<QPushButton*> orderMealButtons;

    QPushButton *refundTicketButton;
    QPushButton *changeTicketButton;
};

class OrderWindow : public QWidget {
    Q_OBJECT

public:
    explicit OrderWindow(QWidget *parent = nullptr);
    void refreshOrderList();

private slots:
    void onOrderItemClicked(QListWidgetItem *item);
    void handleSelectSeat(const Order &order, int segmentIndex, const Ticket &ticket);
    void handleOrderMeal(const Order &order, int segmentIndex, const Ticket &ticket);
    void handleRefundTicket(const Order &order);
    void handleChangeTicket(const Order &order);

private:
    void setupUI();
    void populateOrderList();
    void addOrderItem(const Order &order);
    double calculateRefundFee(double price, const QDateTime &departureDateTime);

    QListWidget *orderListWidget;
};
