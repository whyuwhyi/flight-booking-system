#pragma once

#include <QWidget>

class AirplaneModelManageWindow : public QWidget {
    Q_OBJECT

public:
    AirplaneModelManageWindow(QWidget *parent = nullptr);
    ~AirplaneModelManageWindow();

private:
    void setupUI();
    void setupConnections();
};
