#pragma once

#include <QWidget>

class Sidebar : public QWidget {
public:
    Sidebar(QWidget *parent = nullptr);

private:
    void setupUI();
};
