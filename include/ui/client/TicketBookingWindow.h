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
    QLabel *departureAirportLabel;
    QLabel *arrivalAirportLabel;
    QLabel *airplaneModelLabel;
    const ConnectingTicket& connectingTicket;
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
    void onFlightItemClicked(QListWidgetItem *item);
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
    void addConnectingTicketItem(const ConnectingTicket &connectingTicket);

    QHBoxLayout* createSelectionLayout();
    QListWidget* createFlightListWidget();
    void setupComboBox(QComboBox *comboBox);
    void setupDateEdit(QDateEdit *dateEdit);
    String getSelectedCity(QComboBox *comboBox);
    Date getSelectedDate();
    bool validateCities(const String &departureCity, const String &arrivalCity);
    void queryConnectingFlights(const String &departureCity, const String &arrivalCity, const Date &date);
    QWidget* createFlightInfoLabels(const Ticket &ticket, QWidget *parent);
    QLabel* createConnectingFlightInfoLabel(int segmentNumber, QWidget *parent);
    QGridLayout* createCabinSelectionLayout(const Ticket &ticket, int segmentIndex, LinkedList<CabinType> &selectedCabins, QWidget *parent);
    
    void addCabinLayoutHeaders(QGridLayout *layout, QWidget *parent);
    void handleConnectingTicketPurchase(const ConnectingTicket &connectingTicket, const LinkedList<CabinType> &selectedCabins); 
};
