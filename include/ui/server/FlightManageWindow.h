#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QComboBox>
#include <QTimeEdit>
#include <QMessageBox>
#include <FlightSystem/Flight.h>
#include <FlightSystem/AirplaneModel.h>
#include <FlightSystem/Airport.h>
#include <FlightSystem/Airline.h>

// 独立的 FlightItem 类，用于表示航班项
class FlightItem : public QListWidgetItem {
private:
    QLabel *flightNameLabel;
    QLabel *airlineLabel;
    QLabel *departureCityLabel;
    QLabel *arrivalCityLabel;
    QLabel *departureTimeLabel;
    QPushButton *deleteButton;
    QPushButton *manageButton;

public:
    FlightItem(const Flight &flight, QListWidget *parent = nullptr);
    QPushButton* getDeleteButton();
    QPushButton* getManageButton();
    String getFlightName();

    void setupFlightItemUI(const Flight &flight, QListWidget *parent);
};

// 独立的 FlightScheduleItem 类，用于表示航班计划项
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
    QLabel* getFirstClassPriceLabel();
    QLabel* getBusinessClassPriceLabel();
    QLabel* getEconomyClassPriceLabel();
    Date getFlightDate();
    void setupFlightScheduleItemUI(const FlightTicketDetail &detail, QListWidget *parent);
    void setFlightDetails(const FlightTicketDetail &detail);
};

class FlightManageWindow : public QWidget {
    Q_OBJECT

public:
    explicit FlightManageWindow(QWidget *parent = nullptr);

private:
    // UI setup and connections
    void setupUI();
    void setupConnections();
    void loadFlights();

    // Dialogs
    void openAddFlightDialog();
    void openAddDateDialog();
    void setupAddFlightDialog(QDialog *addFlightDialog);
    void confirmAddFlight(const QString& flightName, const QString& airline, const QString& airplaneModel,const QString& routeName, const QString& departureAirport, const QString& arrivalAirport, const QTime& departureTime,const QTime& costTime, double initialFirstClassPrice, double initialBusinessClassPrice, double initialEconomyClassPrice, QDialog* dialog);

    // Flight management
    void setupManageFlightDialog(QDialog *dialog, FlightItem *item);
    void addFlightItem(const Flight &flight);
    void connectFlightItemActions(FlightItem* item);
    void onDeleteFlight(FlightItem* item);
    void onManageFlight(FlightItem* item);

    // Flight Schedule management
    void addFlightScheduleItem(const FlightTicketDetail &detail);
    void connectFlightScheduleItemActions(FlightScheduleItem* item);
    void onDeleteFlightScheduleItem(FlightScheduleItem* item);
    void onManageFlightScheduleItem(FlightScheduleItem* item);
    void updateFlightSchedule(FlightScheduleItem* item, QLineEdit* firstClassPriceEdit, QLineEdit* businessClassPriceEdit, QLineEdit* economyClassPriceEdit);

    // Utility functions
    bool validateFlightInput(const QString& flightName, const QString& airline, const QString& airplaneModel, const QString& routeName, const QString& departureAirport, const QString& arrivalAirport, double initialFirstClassPrice, double initialBusinessClassPrice, double initialEconomyClassPrice) const;
    Flight createFlight(const QString& flightName, const QString& airline, const QString& airplaneModel, const QString& routeName, const QString& departureAirportName, const QString& arrivalAirportName, const QTime& departureTime, const QTime& costTime, double initialFirstClassPrice, double initialBusinessClassPrice, double initialEconomyClassPrice);
    bool confirmDelete(const QString &title, const QString &message);
    void deleteFlightItem(FlightItem* item);
    void deleteFlightScheduleItem(FlightScheduleItem* item);

    // Helper functions for UI creation
    QLineEdit* createLineEdit(QWidget* parent, const QString& placeholder);
    QComboBox* createComboBox(QWidget* parent, const QString& defaultText);
    template <typename Key, typename Value>
    void populateComboBox(QComboBox* comboBox, const Map<Key, Value>& modelMap);
    QPushButton* createSwapButton(QWidget* parent, QLineEdit* departureAirportLineEdit, QLineEdit* arrivalAirportLineEdit);
    QTimeEdit* createTimeEdit(QWidget* parent, const QTime& time);

    // Widgets and layouts
    QVBoxLayout *mainLayout;
    QListWidget *flightListWidget;
    QListWidget *FlightScheduleWidget;
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

    Flight *flight = nullptr;
};
