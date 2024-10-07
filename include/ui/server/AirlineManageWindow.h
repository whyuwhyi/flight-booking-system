#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLabel>
#include <QComboBox>
#include <QWebChannel>
#include <FlightSystem/Airline.h>

class AirlineManageBackend : public QObject {
    Q_OBJECT
public:
    AirlineManageBackend(QObject *parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE void receiveRouteData(const QVariantList &routePoints, double routeLengthInKm) {
        emit routeDataReceived(routePoints, routeLengthInKm);
    }
    Q_INVOKABLE void receiveAirlineData(const QString &airport1, const QString &airport2, double lat1, double lng1, double lat2, double lng2) {
        emit airlineDataReceived(airport1, airport2, lat1, lng1, lat2, lng2);
    }
    Q_INVOKABLE void requestAirlineData() {
        emit airlineDataRequested();
    }

signals:
    void routeDataReceived(const QVariantList &routePoints, double routeLengthInKm);
    void airlineDataReceived(const QString &airport1, const QString &airport2, double lat1, double lng1, double lat2, double lng2);
    void airlineDataRequested();
};

class AirlineItem;

class AirlineManageWindow : public QWidget {
    Q_OBJECT

public:
    explicit AirlineManageWindow(QWidget *parent = nullptr);

private:
    void setupUI();
    void setupConnections();
    void openAddAirlineWindow();
    QDialog* createAddAirlineDialog();
    QLineEdit* createLineEdit(QWidget* parent, const QString& placeholder);
    void populateAirportComboBoxes();
    void openMapSearchWindow(const QString &airport1, const QString &airport2);
    QDialog* createMapSearchDialog(const QString &airport1, const QString &airport2);
    void setupBackendConnections(class AirlineManageBackend* backend, const QString &airport1, const QString &airport2);
    void handleRouteData(const QVariantList &routePoints, double routeLengthInKm);
    void addAirlineItem(const Airline& airline);
    void onDeleteAirline(AirlineItem *item);

    QVBoxLayout *mainLayout;
    QListWidget *airlineListWidget;
    QPushButton *addAirlineButton;
    QLineEdit *airlineNameLineEdit;
    QComboBox *airport1ComboBox;
    QComboBox *airport2ComboBox;
};

class AirlineItem : public QListWidgetItem {
public:
    AirlineItem(const Airline &airline, QListWidget *parent = nullptr);
    QPushButton* getDeleteButton();
    QString getAirlineName();

private:
    QLabel *nameLabel;
    QLabel *airport1Label;
    QLabel *airport2Label;
    QPushButton *deleteButton;
};