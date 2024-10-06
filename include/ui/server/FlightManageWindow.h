#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QComboBox>
#include <QTimeEdit>
#include <QPushButton>
#include <FlightSystem/Flight.h>

class FlightManageWindow : public QWidget {
    Q_OBJECT

public:
    FlightManageWindow(QWidget *parent = nullptr);

private:
    void setupUI();
    void setupConnections();

    QLineEdit *flightNameLineEdit;
    QLineEdit *airlineLineEdit;
    QComboBox *airplaneModelComboBox;
    QComboBox *routeComboBox;
    QTimeEdit *departureTimeEdit;
    QTimeEdit *costTimeEdit;
    QPushButton *addFlightButton;
    QPushButton *swapAirportsButton;
    QLabel *departureAirportLabel;
    QLabel *arrivalAirportLabel;
    QListWidget *flightListWidget;
    QVBoxLayout *mainLayout;
    Airport *departureAirport = nullptr;
    Airport *arrivalAirport = nullptr;

    class FlightItem : public QListWidgetItem {
    private:
        QLabel *flightNameLabel;
        QLabel *airlineLabel;
        QLabel *departureCityLabel;
        QLabel *arrivalCityLabel;
        QLabel *departureTimeLabel;
        QLabel *firstClassPriceLabel;
        QLabel *businessClassPriceLabel;
        QLabel *economyClassPriceLabel;
        QPushButton *deleteButton;
        QPushButton *manageButton;
    public:
        FlightItem(const Flight &flight, QListWidget *parent = nullptr);
        QPushButton* getDeleteButton();
        QPushButton* getManageButton();
        String getFlightName();
        void setCabinPrices(double firstClassPrice, double businessClassPrice, double economyClassPrice);
    };

    void addFlightItem(const Flight &flight);
    void onDeleteFlight(FlightItem* item);
    void onManageFlight(FlightItem* item);
    void onSwapAirports();

    class FlightScheduleItem : public QListWidgetItem {
    private:
        QLabel *dateLabel;
        QLabel *firstClassPriceLabel;
        QLabel *firstClassTicketsLabel;
        QLabel *businessClassPriceLabel;
        QLabel *businessClassTicketsLabel;
        QLabel *economyClassPriceLabel;
        QLabel *economyClassTicketsLabel;
        QPushButton *deleteButton;
        QPushButton *editButton;
    public:
        FlightScheduleItem(const FlightTicketDetail &detail, QListWidget *parent = nullptr);
        QPushButton* getDeleteButton();
        QPushButton* getEditButton();
        Date getFlightDate();
        void setFlightDetails(const FlightTicketDetail &detail);
    };
};;