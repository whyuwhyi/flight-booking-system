#include <ui/LoginWindow.h>
#include <ui/MainWindow.h>
#include <ui/RegisterWindow.h>
#include <User/UserServer.h>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

#include <iostream>

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupConnections();
}

void LoginWindow::setupUI() {
    setWindowTitle("天地航空");
    setFixedSize(1000, 800);

    QLabel *labelPhoneNumber = new QLabel("手机号码:", this);
    QLabel *labelPassword = new QLabel("密码:", this);

    lineEditPhoneNumber = new QLineEdit(this);
    lineEditPassword = new QLineEdit(this);
    lineEditPassword->setEchoMode(QLineEdit::Password);

    buttonLogin = new QPushButton("登录", this);
    registerButton = new QPushButton("注册", this);

    buttonLogin->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
    registerButton->setStyleSheet("background-color: #FF5722; color: white; font-weight: bold;");
    lineEditPhoneNumber->setPlaceholderText("11位手机号码");
    lineEditPassword->setPlaceholderText("密码");

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(labelPhoneNumber, lineEditPhoneNumber);
    formLayout->addRow(labelPassword, lineEditPassword);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonLogin);
    mainLayout->addWidget(registerButton);

    QWidget *centeredWidget = new QWidget(this);
    centeredWidget->setFixedSize(400, 300);
    centeredWidget->setLayout(mainLayout);

    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->addWidget(centeredWidget, 0, Qt::AlignCenter);
    setLayout(outerLayout);
}

void LoginWindow::setupConnections() {
    connect(buttonLogin, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(registerButton, &QPushButton::clicked, [this]() {
        this->close();
        emit registerRequested();
    });
}

void LoginWindow::onLoginClicked() {
    String phoneNumber = lineEditPhoneNumber->text().toStdString().c_str();
    String password = lineEditPassword->text().toStdString().c_str();
    User temp_user(phoneNumber, password);
    Link<User>* userPointer = user_list.getHead();

    while(userPointer != NULL) {
        if(userPointer->getElement() == temp_user){
            std::cout << "Login Success" << std::endl;
            emit loginSuccess();
            return;
        }
        userPointer = userPointer->getNext();
    }

    QMessageBox::warning(this, "登录失败", "账号或密码错误！");

}
