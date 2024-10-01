#pragma once

#include <QWidget>
#include <LinkedList/LinkedList.h>
#include <FlightSystem/Airport.h>

class QLabel;
class QPushButton;
class QLineEdit;
class QListWidget;

class AirportManagementWindow : public QWidget {
    Q_OBJECT

public:
    AirportManagementWindow(QWidget *parent = nullptr);
    ~AirportManagementWindow();

private:
    QLabel *infoLabel;
    QPushButton *addButton;
    QPushButton *modifyButton;
    QPushButton *deleteButton;
    QLineEdit *nameInput;
    QLineEdit *countryInput;
    QLineEdit *cityInput;
    QListWidget *airportListWidget;

    void setupUI();
    void setupConnections();
    void refreshAirportList();
};
