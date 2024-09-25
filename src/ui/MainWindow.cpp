#include <ui/MainWindow.h>
#include <User/UserServer.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    loadUserFromFile(user_list, user_file);
    stackedWidget = new QStackedWidget(this);
    loginWindow = new LoginWindow(this);
    registerWindow = new RegisterWindow(this);

    stackedWidget->addWidget(loginWindow);
    stackedWidget->addWidget(registerWindow);
    
    setCentralWidget(stackedWidget);
    connect(loginWindow, &LoginWindow::showRegisterWindow, [this]() {
        stackedWidget->setCurrentWidget(registerWindow);
    });
    connect(registerWindow, &RegisterWindow::showLoginWindow, [this]() {
    stackedWidget->setCurrentWidget(loginWindow);
});


}
