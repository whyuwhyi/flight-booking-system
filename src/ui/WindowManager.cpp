#include <QStackedWidget>
#include <ui/LoginWindow.h>
#include <ui/RegisterWindow.h>
#include <ui/MainWindow.h>
#include <ui/WindowManager.h>
#include <User/UserServer.h>
#include <iostream>



WindowManager::WindowManager(QWidget *parent) { }

void WindowManager::initFrame() {
    if (localLogin()) {
        mainWindow = new MainWindow();
        mainWindow->show();
    }
    else {
        stackedWidget = new QStackedWidget();
        loginWindow = new LoginWindow(stackedWidget);
        registerWindow = new RegisterWindow(stackedWidget);

        stackedWidget->addWidget(loginWindow);
        stackedWidget->addWidget(registerWindow);

        stackedWidget->setCurrentWidget(loginWindow);
        stackedWidget->show();
        
        connect(loginWindow, &LoginWindow::loginSuccess, this, &WindowManager::showMainWindow);
        connect(loginWindow, &LoginWindow::registerRequested, this, &WindowManager::showRegisterWindow);
        connect(registerWindow, &RegisterWindow::loginRequested, this, &WindowManager::showLoginWindow);
    }
}

WindowManager::~WindowManager() {
    delete mainWindow;
    delete stackedWidget;
}


void WindowManager::showLoginWindow() {
    stackedWidget->setCurrentWidget(loginWindow);
}

void WindowManager::showRegisterWindow() {
    stackedWidget->setCurrentWidget(registerWindow);
}

void WindowManager::showMainWindow() {
    mainWindow = new MainWindow();
    mainWindow->show();
    delete stackedWidget;
    stackedWidget = NULL;
    user_list.destroy();
}

bool WindowManager::localLogin() {
    User temp_user;
    loadUserFromFile(user_list, data_path + "users/users.txt");
    loadLocalUserFromFile(temp_user, local_path + "local-user.txt");
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
