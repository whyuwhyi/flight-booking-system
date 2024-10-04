#include <QStackedWidget>
#include <ui/LoginWindow.h>
#include <ui/RegisterWindow.h>
#include <ui/MainWindow.h>
#include <ui/WindowManager.h>
#include <data/datamanage.h>
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
}

bool WindowManager::localLogin() {
    User temp_user;
    loadUserFromFile(user_map);
    if(loadLocalUserFromFile(temp_user)) {
        User* user_node = user_map.get(temp_user.getPhoneNumber());
        if(user_node != NULL && user_node->getPassword() == temp_user.getPassword()) {
            std::cout << "Local Login Success!" << std::endl;
                return true;
        }
    }
    return false;
}
