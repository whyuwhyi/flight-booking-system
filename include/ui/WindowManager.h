#pragma once

#include <QStackedWidget>
#include <ui/LoginWindow.h>
#include <ui/RegisterWindow.h>
#include <ui/MainWindow.h>


class WindowManager : public QStackedWidget {
    Q_OBJECT

public:
    WindowManager(QWidget *parent = nullptr);

    void showLoginWindow();
    void showRegisterWindow();
    void showMainWindow();
    bool localLogin();

private:
    LoginWindow *loginWindow;
    RegisterWindow *registerWindow;
    MainWindow *mainWindow;
};
