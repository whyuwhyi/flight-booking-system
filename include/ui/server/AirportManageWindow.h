#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLabel>
#include <QWebChannel>
#include <FlightSystem/Airport.h>

class AirportManageBackend : public QObject {
    Q_OBJECT
public:
    AirportManageBackend(QObject *parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE void receiveAirportData(const QString &name, const QString &country, const QString &city, double latitude, double longitude) {
        emit airportDataReceived(name, country, city, latitude, longitude);
    }

signals:
    void airportDataReceived(const QString &name, const QString &country, const QString &city, double latitude, double longitude);
};

class AirportItem : public QListWidgetItem {
public:
    AirportItem(const Airport &airport, QListWidget *parent = nullptr);
    QPushButton* getDeleteButton();
    String getAirportName();

private:
    QLabel *nameLabel;
    QLabel *countryLabel;
    QLabel *cityLabel;
    QPushButton *deleteButton;
};

class AirportManageWindow : public QWidget {
    Q_OBJECT

public:
    explicit AirportManageWindow(QWidget *parent = nullptr);

private:
    void setupUI();
    void setupConnections();
    void openMapSearchWindow();
    void handleAirportData(const QString &name, const QString &country, const QString &city, double latitude, double longitude);
    void addAirportItem(const Airport& airport);
    void onDeleteAirport(AirportItem *item);

    QVBoxLayout *mainLayout;
    QListWidget *airportListWidget;
    QPushButton *addAirportButton;
};

