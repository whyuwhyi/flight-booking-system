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
#include <QCheckBox>
#include <QHBoxLayout>
#include <QPixmap>
#include <QLabel>
#include <iostream>

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupConnections();
}

void LoginWindow::setupUI() {
    setWindowTitle("航空系统");
    setFixedSize(1000, 800);

    QLabel *phoneNumberLabel = new QLabel("手机号码:", this);
    QLabel *PasswordLabel = new QLabel("密码:", this);

    phoneNumberLineEdit = new QLineEdit(this);
    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    phoneNumberLineEdit->setFixedHeight(40);
    passwordLineEdit->setFixedHeight(40);
    phoneNumberLineEdit->setFixedWidth(300);
    passwordLineEdit->setFixedWidth(300);

    togglePasswordVisibility = new QPushButton(this);
    togglePasswordVisibility->setIcon(QIcon(":/icons/eye_closed.svg"));
    togglePasswordVisibility->setFixedSize(30, 30);
    togglePasswordVisibility->setStyleSheet("border: none; background: transparent;");

    loginButton = new QPushButton("登录", this);
    registerButton = new QPushButton("注册", this);
    
    loginButton->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
    registerButton->setStyleSheet("background-color: #FF5722; color: white; font-weight: bold;");
    
    phoneNumberLineEdit->setPlaceholderText("11位手机号码");
    passwordLineEdit->setPlaceholderText("密码");

    autoLoginCheckBox = new QCheckBox("自动登录", this);
    autoLoginCheckBox->setStyleSheet("font-size: 14px;");

    QHBoxLayout *passwordLayout = new QHBoxLayout;
    passwordLayout->addWidget(passwordLineEdit);
    passwordLayout->addWidget(togglePasswordVisibility);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(phoneNumberLabel, phoneNumberLineEdit);
    formLayout->addRow(PasswordLabel, passwordLayout);
    

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(autoLoginCheckBox);
    mainLayout->addWidget(loginButton);
    mainLayout->addWidget(registerButton);
    mainLayout->addWidget(togglePasswordVisibility);

    QWidget *centeredWidget = new QWidget(this);
    centeredWidget->setFixedSize(400, 250);
    centeredWidget->setLayout(mainLayout);

    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->addWidget(centeredWidget, 0, Qt::AlignCenter);
    setLayout(outerLayout);
}


void LoginWindow::setupConnections() {
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(registerButton, &QPushButton::clicked, [this]() {
        this->close();
        emit registerRequested();
    });

    connect(togglePasswordVisibility, &QPushButton::clicked, this, [this]() {
        if (passwordLineEdit->echoMode() == QLineEdit::Password) {
            passwordLineEdit->setEchoMode(QLineEdit::Normal);
            togglePasswordVisibility->setIcon(QIcon(":/icons/eye_open.svg"));
        } else {
            passwordLineEdit->setEchoMode(QLineEdit::Password);
            togglePasswordVisibility->setIcon(QIcon(":/icons/eye_closed.svg"));
        }
    });

}

void LoginWindow::onLoginClicked() {
    String phoneNumber = phoneNumberLineEdit->text().toStdString().c_str();
    String password = passwordLineEdit->text().toStdString().c_str();
    User temp_user(phoneNumber, password);
    Link<User>* userPointer = user_list.getHead();

    while(userPointer != NULL) {
        if(userPointer->getElement() == temp_user){
            std::cout << "Login Success!" << std::endl;
            if (autoLoginCheckBox->isChecked()) {
                std::cout << "Save the user information." << std::endl;
                writeLocalUserToFile(temp_user, local_path + "local-user.txt");
            }
            emit loginSuccess();
            return;
        }
        userPointer = userPointer->getNext();
    }

    QMessageBox::warning(this, "登录失败", "账号或密码错误！");

}
