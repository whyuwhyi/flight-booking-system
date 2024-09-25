#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include <ui/LoginWindow.h>
#include <ui/RegisterWindow.h>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    QStackedWidget *stackedWidget;
    LoginWindow *loginWindow;
    RegisterWindow *registerWindow;
};
