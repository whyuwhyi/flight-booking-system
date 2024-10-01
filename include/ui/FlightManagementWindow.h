#pragma once

#include <QWidget>

class FlightManageWindow : public QWidget {
    Q_OBJECT

public:
    FlightManageWindow(QWidget *parent = nullptr);
    ~FlightManageWindow();

private:
    void setupUI();
    void setupConnections();
};
