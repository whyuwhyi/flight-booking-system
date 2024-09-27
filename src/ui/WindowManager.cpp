#include <QStackedWidget>
#include <ui/LoginWindow.h>
#include <ui/RegisterWindow.h>
#include <ui/MainWindow.h>
#include <ui/WindowManager.h>
#include <User/UserServer.h>
#include <iostream>



WindowManager::WindowManager(QWidget *parent) {

    mainWindow = new MainWindow();

    if (localLogin()) {
        showMainWindow();
    }
    else {
        stackedWidget = new QStackedWidget(parent);
        loginWindow = new LoginWindow();
        registerWindow = new RegisterWindow();

        stackedWidget->addWidget(loginWindow);
        stackedWidget->addWidget(registerWindow);
        stackedWidget->show();
        stackedWidget->setCurrentWidget(loginWindow);
        connect(loginWindow, &LoginWindow::loginSuccess, this, &WindowManager::showMainWindow);
        connect(loginWindow, &LoginWindow::registerRequested, this, &WindowManager::showRegisterWindow);
        connect(registerWindow, &RegisterWindow::loginRequested, this, &WindowManager::showLoginWindow);
    }
}

WindowManager::~WindowManager() {}


void WindowManager::showLoginWindow() {
    stackedWidget->setCurrentWidget(loginWindow);
}

void WindowManager::showRegisterWindow() {
    stackedWidget->setCurrentWidget(registerWindow);
}

void WindowManager::showMainWindow() {
    delete loginWindow;
    delete registerWindow;
    delete stackedWidget;
    loginWindow = NULL;
    registerWindow = NULL;
    stackedWidget = NULL;
    mainWindow->show();
    user_list.destroy();
}

bool WindowManager::localLogin() {
    User temp_user;
    loadUserFromFile(user_list, user_file);
    loadLocalUserFromFile(temp_user, local_user_file);
    Link<User>* userPointer = user_list.getHead();

    while (userPointer != NULL) {
        if (userPointer->getElement() == temp_user) {
            std::cout << "Local Login Success!" << std::endl;
            return true;
        }
        userPointer = userPointer->getNext();
    }
    return false;
}
