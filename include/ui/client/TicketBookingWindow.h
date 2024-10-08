#pragma once

#include <QWidget>
#include <QComboBox>
#include <QDateEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>

class TicketBookingWindow : public QWidget {
    Q_OBJECT

private slots:
    void onSearchButtonClicked();
    void onBookButtonClicked();

private:
    QComboBox *departureCityComboBox;
    QComboBox *arrivalCityComboBox;
    QDateEdit *dateEdit;
    QPushButton *searchButton;
    QPushButton *bookButton;
    QListWidget *flightListWidget;

    void setupUI();
    void setupConnections();
    void populateFlightList();

public:
    TicketBookingWindow(QWidget *parent = nullptr);
};