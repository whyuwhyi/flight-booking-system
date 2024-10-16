#include <ui/client/LoginWindow.h>
#include <QLabel>
#include <data/datamanage.h>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QIcon>
#include <iostream>

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent) {
    loadUserFromFile(user_map);
    setupUI();
    setupConnections();
    checkAutoLogin();
}

void LoginWindow::setupUI() {
    setWindowTitle("航空系统");
    setFixedSize(1200, 800);

    QLabel *phoneNumberLabel = new QLabel("手机号码:", this);
    QLabel *passwordLabel = new QLabel("密码:", this);

    phoneNumberLineEdit = new QLineEdit(this);
    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    phoneNumberLineEdit->setFixedHeight(40);
    passwordLineEdit->setFixedHeight(40);
    phoneNumberLineEdit->setFixedWidth(300);
    passwordLineEdit->setFixedWidth(300);

    phoneNumberLineEdit->setPlaceholderText("11位手机号码");
    passwordLineEdit->setPlaceholderText("密码");

    togglePasswordVisibilityButton = new QPushButton(this);
    togglePasswordVisibilityButton->setIcon(QIcon(":/icons/eye_closed.svg"));
    togglePasswordVisibilityButton->setFixedSize(30, 30);
    togglePasswordVisibilityButton->setStyleSheet("border: none; background: transparent;");

    loginButton = new QPushButton("登录", this);
    registerButton = new QPushButton("注册", this);
    loginButton->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
    registerButton->setStyleSheet("background-color: #FF5722; color: white; font-weight: bold;");

    autoLoginCheckBox = new QCheckBox("自动登录", this);
    autoLoginCheckBox->setStyleSheet("font-size: 14px;");

    QHBoxLayout *passwordLayout = new QHBoxLayout;
    passwordLayout->addWidget(passwordLineEdit);
    passwordLayout->addWidget(togglePasswordVisibilityButton);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(phoneNumberLabel, phoneNumberLineEdit);
    formLayout->addRow(passwordLabel, passwordLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(autoLoginCheckBox);
    mainLayout->addWidget(loginButton);
    mainLayout->addWidget(registerButton);

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
        emit registerRequested();
    });
    connect(togglePasswordVisibilityButton, &QPushButton::clicked, this, &LoginWindow::togglePasswordVisibility);
}

void LoginWindow::onLoginClicked() {
    QString phoneNumber = phoneNumberLineEdit->text();
    QString password = passwordLineEdit->text();

    User* user_node = user_map.find(phoneNumber.toStdString().c_str());

    if (user_node != nullptr && password == QString::fromStdString(user_node->getPassword().c_str())) {
        if (autoLoginCheckBox->isChecked()) {
            current_login_user = User(phoneNumber.toStdString().c_str(), password.toStdString().c_str());
            writeLocalUserToFile(current_login_user);
        }
        emit loginSuccess();
        return;
    }

    QMessageBox::warning(this, "登录失败", "账号或密码错误！");
}

void LoginWindow::togglePasswordVisibility() {
    if (passwordLineEdit->echoMode() == QLineEdit::Password) {
        passwordLineEdit->setEchoMode(QLineEdit::Normal);
        togglePasswordVisibilityButton->setIcon(QIcon(":/icons/eye_open.svg"));
    } else {
        passwordLineEdit->setEchoMode(QLineEdit::Password);
        togglePasswordVisibilityButton->setIcon(QIcon(":/icons/eye_closed.svg"));
    }
}

void LoginWindow::checkAutoLogin() {
    if (loadLocalUserFromFile(current_login_user)) {
        User* user_node = user_map.find(current_login_user.getPhoneNumber());
        if (user_node != nullptr && user_node->getPassword() == current_login_user.getPassword()) {
            phoneNumberLineEdit->setText(QString::fromStdString(current_login_user.getPhoneNumber().c_str()));
            passwordLineEdit->setText(QString::fromStdString(current_login_user.getPassword().c_str()));
            autoLoginCheckBox->setChecked(true);
            emit loginSuccess();
        }
    }
}