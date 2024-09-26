#include <QStackedWidget>
#include <ui/LoginWindow.h>
#include <ui/RegisterWindow.h>
#include <ui/MainWindow.h>
#include <User/UserServer.h>
#include <iostream>

class WindowManager : public QObject {
public:
    WindowManager(QWidget *parent = nullptr);
    ~WindowManager();

private:
    QStackedWidget *stackedWidget = NULL;
    LoginWindow *loginWindow = NULL;
    RegisterWindow *registerWindow = NULL;
    MainWindow *mainWindow = NULL;

    void showLoginWindow();
    void showRegisterWindow();
    void showMainWindow();
    bool localLogin();
};