#pragma once

#include <QListWidgetItem>
#include <QWidget>
#include <QComboBox>
#include <QDateEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QHBoxLayout>
#include <FlightSystem/FlightNetwork.h>
#include <FlightSystem/Flight.h>
#include <FlightSystem/Ticket.h>
#include <String/String.h>

class TicketItem : public QWidget {
    Q_OBJECT
public:
    TicketItem(const Ticket &ticket, QWidget *parent = nullptr);
    const Ticket& getTicket() const;

private:
    QLabel *flightNumberLabel;
    QLabel *departureTimeLabel;
    QLabel *arrivalTimeLabel;
    QLabel *priceLabel;
    QLabel *durationLabel;
    QLabel *dayDifferenceLabel;
    QLabel *departureAirportLabel;
    QLabel *arrivalAirportLabel;
    QLabel *airplaneModelLabel;
    Ticket ticket;

    QWidget *itemWidget;
    QHBoxLayout *layout;

    void setupUI();
};

class ConnectingTicketItem : public QWidget {
    Q_OBJECT
public:
    ConnectingTicketItem(const ConnectingTicket &ticket, QWidget *parent = nullptr);
    const ConnectingTicket& getConnectingTicket() const;

private:
    QLabel *totalPriceLabel;
    QLabel *totalDurationLabel;
    QLabel *departureDateLabel;
    QLabel *arrivalDateLabel;
    QLabel *dayDifferenceLabel;
    QLabel *departureAirportLabel;
    QLabel *arrivalAirportLabel;
    QLabel *airplaneModelLabel;
    ConnectingTicket connectingTicket;

    QWidget *itemWidget;
    QVBoxLayout *mainLayout;
    QVBoxLayout *ticketsLayout;

    void setupUI();
    void addTicketDetails(const Ticket &ticket);
};

class TicketBookingWindow : public QWidget {
    Q_OBJECT

public:
    TicketBookingWindow(QWidget *parent = nullptr);

private slots:
    void onSearchButtonClicked();
    void onTicketItemClicked(const Ticket &ticket);
    void onConnectingTicketItemClicked(const ConnectingTicket &connectingTicket);

private:
    QComboBox *departureCityComboBox;
    QComboBox *arrivalCityComboBox;
    QDateEdit *dateEdit;
    QComboBox *filterComboBox;
    QPushButton *searchButton;
    QListWidget *flightListWidget;

    void setupUI();
    void setupConnections();
    void populateCityComboBoxes();
    void populateFlightList();
    void addTicketItem(const Ticket &ticket);
    void addConnectingTicketItem(const ConnectingTicket &connectingTicket);
    void onFlightItemClicked(QListWidgetItem *item);
};