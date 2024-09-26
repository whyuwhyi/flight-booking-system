#include <ui/WindowManager.h>
#include <User/UserServer.h>
#include <iostream>

WindowManager::WindowManager(QWidget *parent)
    : QStackedWidget(parent) {

    mainWindow = new MainWindow(this);

    if (localLogin()) {
        addWidget(mainWindow);
        setCurrentWidget(mainWindow);
    }
    else {
        loginWindow = new LoginWindow(this);
        registerWindow = new RegisterWindow(this);

        addWidget(loginWindow);
        addWidget(registerWindow);
        addWidget(mainWindow);
        setCurrentWidget(loginWindow);

        connect(loginWindow, &LoginWindow::loginSuccess, this, &WindowManager::showMainWindow);
        connect(loginWindow, &LoginWindow::registerRequested, this, &WindowManager::showRegisterWindow);
        connect(registerWindow, &RegisterWindow::loginRequested, this, &WindowManager::showLoginWindow);
    }
}


void WindowManager::showLoginWindow() {
    setCurrentWidget(loginWindow);
}

void WindowManager::showRegisterWindow() {
    setCurrentWidget(registerWindow);
}

void WindowManager::showMainWindow() {
    setCurrentWidget(mainWindow);
}

bool WindowManager::localLogin() {
    User temp_user;
    loadUserFromFile(user_list, user_file);
    loadLocalUserFromFile(temp_user, local_user_file);
    Link<User>* userPointer = user_list.getHead();

    while(userPointer != NULL) {
        if(userPointer->getElement() == temp_user){
            std::cout << "Local Login Success!" << std::endl;
            return false;
        }
        userPointer = userPointer->getNext();
    }

    return false;
}
