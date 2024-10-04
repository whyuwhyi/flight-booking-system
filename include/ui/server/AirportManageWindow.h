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

class AirportManageWindow : public QWidget {
    Q_OBJECT

private slots:
    void handleAirportData(const QString &name, const QString &country, const QString &city, double latitude, double longitude);

private:
    QLineEdit *searchLineEdit;
    QPushButton *addAirportButton;
    QListWidget *airportListWidget;
    QVBoxLayout *mainLayout;

    class AirportItem : public QListWidgetItem {
    private:
        QLabel *nameLabel;
        QLabel *countryLabel;
        QLabel *cityLabel;
        QPushButton *deleteButton;
    public:
        AirportItem(const Airport &airport, QListWidget *parent = nullptr);
        QPushButton* getDeleteButton();
        String getAirportName();
    };

    void onDeleteAirport(AirportItem* item);
    void addAirportItem(const Airport &airport);
    void openMapSearchWindow();
public:
    AirportManageWindow(QWidget *parent = nullptr);
    void setupUI();
    void setupConnections();
};
