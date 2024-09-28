#pragma once


class QStackedWidget;
class LoginWindow;
class RegisterWindow;
class MainWindow;

class WindowManager : public QObject {
public:
    WindowManager(QWidget *parent = nullptr);
    void initFrame();
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