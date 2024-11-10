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
#include <QRadioButton>
#include <QLineEdit>

class TicketItem : public QWidget {
    Q_OBJECT
public:
    TicketItem(const Ticket &ticket, QWidget *parent = nullptr);
    Ticket& getTicket();

private:
    QLabel *flightNumberLabel;
    QLabel *departureTimeLabel;
    QLabel *arrivalTimeLabel;
    QLabel *priceLabel;
    QLabel *departureAirportLabel;
    QLabel *arrivalAirportLabel;
    QLabel *airplaneModelLabel;
    Ticket ticket;
    QHBoxLayout *layout;

    void setupUI();
};

class ConnectingTicketItem : public QWidget {
    Q_OBJECT
public:
    ConnectingTicketItem(const ConnectingTicket &ticket, QWidget *parent = nullptr);
    ConnectingTicket& getConnectingTicket();

private:
    QLabel *totalPriceLabel;
    QLabel *totalDurationLabel;
    QLabel *departureDateLabel;
    QLabel *arrivalDateLabel;
    QLabel *departureAirportLabel;
    QLabel *arrivalAirportLabel;
    QLabel *airplaneModelLabel;
    ConnectingTicket connectingTicket;
    QVBoxLayout *mainLayout;
    QVBoxLayout *ticketsLayout;

    void setupUI();
};

class TicketBookingWindow : public QWidget {
    Q_OBJECT

public:
    TicketBookingWindow(QWidget *parent = nullptr);

private slots:
    void onSearchButtonClicked();
    void onOrdertItemClicked(Ticket &ticket);
    void onConnectingTicketItemClicked(ConnectingTicket &connectingTicket);
    void onFlightItemClicked(QListWidgetItem *item);

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

    QHBoxLayout* createSelectionLayout();
    QListWidget* createFlightListWidget();
    void setupComboBox(QComboBox *comboBox);
    void setupDateEdit(QDateEdit *dateEdit);
    String getSelectedCity(QComboBox *comboBox);
    Date getSelectedDate();
    bool validateCities(const String &departureCity, const String &arrivalCity);
    void queryDirectFlights(const String &departureCity, const String &arrivalCity, const Date &date);
    void queryConnectingFlights(const String &departureCity, const String &arrivalCity, const Date &date);
    QDialog* createDetailWindow(const QString &title);
    QWidget* createFlightInfoLabels(const Ticket &ticket, QWidget *parent);
    QLabel* createConnectingFlightInfoLabel(int segmentNumber, QWidget *parent);
    QGridLayout* createCabinSelectionLayout(const Ticket &ticket, QWidget *parent);
    QGridLayout* createCabinSelectionLayout(const Ticket &ticket, int segmentIndex, QWidget *parent, CabinType *selectedCabins);
    void addCabinLayoutHeaders(QGridLayout *layout, QWidget *parent);
    void addCabinRow(QGridLayout *layout, const Ticket &ticket, const QString &cabinTypeStr, CabinType cabinType, const QString &discount, QPushButton *&bookButton, QWidget *parent);
    void handleTicketPurchase(const Ticket &ticket, CabinType cabinType, QWidget *parent);
    void handleConnectingTicketPurchase(const Ticket **tickets, const CabinType *selectedCabins, int segmentCount, QWidget *parent); 
};
