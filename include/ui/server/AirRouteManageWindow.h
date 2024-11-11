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
#include <FlightSystem/AirRoute.h>

class AirRouteManageBackend : public QObject {
    Q_OBJECT
public:
    AirRouteManageBackend(QObject *parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE void receiveRouteData(const QVariantList &routePoints, double routeLengthInKm) {
        emit routeDataReceived(routePoints, routeLengthInKm);
    }
    Q_INVOKABLE void receiveAirRouteData(const QString &airport1, const QString &airport2, double lat1, double lng1, double lat2, double lng2) {
        emit airRouteDataReceived(airport1, airport2, lat1, lng1, lat2, lng2);
    }
    Q_INVOKABLE void requestAirRouteData() {
        emit airRouteDataRequested();
    }

signals:
    void routeDataReceived(const QVariantList &routePoints, double routeLengthInKm);
    void airRouteDataReceived(const QString &airport1, const QString &airport2, double lat1, double lng1, double lat2, double lng2);
    void airRouteDataRequested();
};

class AirRouteItem;

class AirRouteManageWindow : public QWidget {
    Q_OBJECT

public:
    explicit AirRouteManageWindow(QWidget *parent = nullptr);

private:
    void setupUI();
    void setupConnections();
    void openAddAirRouteWindow();
    QDialog* createAddAirRouteDialog();
    QLineEdit* createLineEdit(QWidget* parent, const QString& placeholder);
    QComboBox* createComboBox(QWidget* parent, const QString& defaultText);
    void populateAirportComboBoxes();
    void openMapSearchWindow(const QString &airport1, const QString &airport2);
    QDialog* createMapSearchDialog(const QString &airport1, const QString &airport2);
    void setupBackendConnections(class AirRouteManageBackend* backend, const QString &airport1, const QString &airport2);
    void handleRouteData(const QVariantList &routePoints, double routeLengthInKm);
    void addAirRouteItem(const AirRoute& airRoute);
    void onDeleteAirRoute(AirRouteItem *item);

    QVBoxLayout *mainLayout;
    QListWidget *airRouteListWidget;
    QPushButton *addAirRouteButton;
    QLineEdit *airRouteNameLineEdit;
    QComboBox *airport1ComboBox;
    QComboBox *airport2ComboBox;
};

class AirRouteItem : public QListWidgetItem {
public:
    AirRouteItem(const AirRoute &airRoute, QListWidget *parent = nullptr);
    QPushButton* getDeleteButton();
    QString getAirlineName();

private:
    QLabel *nameLabel;
    QLabel *airport1Label;
    QLabel *airport2Label;
    QPushButton *deleteButton;
};