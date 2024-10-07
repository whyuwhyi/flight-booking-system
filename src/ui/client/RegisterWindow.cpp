#include <ui/client/RegisterWindow.h>
#include <data/datamanage.h>
#include <QLabel>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QIcon>
#include <QRegularExpression>

RegisterWindow::RegisterWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupConnections();
}

void RegisterWindow::setupUI() {
    setWindowTitle("注册");
    setFixedSize(1200, 800);

    QLabel *phoneNumberLabel = new QLabel("手机号码:", this);
    QLabel *passwordLabel = new QLabel("密码:", this);
    QLabel *confirmPasswordLabel = new QLabel("确认密码:", this);

    phoneNumberLineEdit = new QLineEdit(this);
    passwordLineEdit = new QLineEdit(this);
    confirmPasswordLineEdit = new QLineEdit(this);
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordLineEdit->setEchoMode(QLineEdit::Password);
    phoneNumberLineEdit->setFixedHeight(40);
    passwordLineEdit->setFixedHeight(40);
    confirmPasswordLineEdit->setFixedHeight(40);
    phoneNumberLineEdit->setFixedWidth(300);
    passwordLineEdit->setFixedWidth(300);
    confirmPasswordLineEdit->setFixedWidth(300);

    phoneNumberLineEdit->setPlaceholderText("11位手机号码");
    passwordLineEdit->setPlaceholderText("密码");
    confirmPasswordLineEdit->setPlaceholderText("确认密码");

    togglePasswordVisibilityButton = new QPushButton(this);
    togglePasswordVisibilityButton->setIcon(QIcon(":/icons/eye_closed.svg"));
    togglePasswordVisibilityButton->setFixedSize(30, 30);
    togglePasswordVisibilityButton->setStyleSheet("border: none; background: transparent;");

    toggleConfirmPasswordVisibilityButton = new QPushButton(this);
    toggleConfirmPasswordVisibilityButton->setIcon(QIcon(":/icons/eye_closed.svg"));
    toggleConfirmPasswordVisibilityButton->setFixedSize(30, 30);
    toggleConfirmPasswordVisibilityButton->setStyleSheet("border: none; background: transparent;");

    registerButton = new QPushButton("注册", this);
    backButton = new QPushButton("返回登录", this);
    registerButton->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
    backButton->setStyleSheet("background-color: #FF5722; color: white; font-weight: bold;");

    QHBoxLayout *passwordLayout = new QHBoxLayout;
    passwordLayout->addWidget(passwordLineEdit);
    passwordLayout->addWidget(togglePasswordVisibilityButton);

    QHBoxLayout *confirmPasswordLayout = new QHBoxLayout;
    confirmPasswordLayout->addWidget(confirmPasswordLineEdit);
    confirmPasswordLayout->addWidget(toggleConfirmPasswordVisibilityButton);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(phoneNumberLabel, phoneNumberLineEdit);
    formLayout->addRow(passwordLabel, passwordLayout);
    formLayout->addRow(confirmPasswordLabel, confirmPasswordLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(registerButton);
    mainLayout->addWidget(backButton);

    QWidget *centeredWidget = new QWidget(this);
    centeredWidget->setFixedSize(400, 300);
    centeredWidget->setLayout(mainLayout);

    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->addWidget(centeredWidget, 0, Qt::AlignCenter);
    setLayout(outerLayout);
}

void RegisterWindow::setupConnections() {
    connect(registerButton, &QPushButton::clicked, this, &RegisterWindow::onRegisterClicked);
    connect(backButton, &QPushButton::clicked, [this]() {
        emit loginRequested();
    });
    connect(togglePasswordVisibilityButton, &QPushButton::clicked, this, &RegisterWindow::togglePasswordVisibility);
    connect(toggleConfirmPasswordVisibilityButton, &QPushButton::clicked, this, &RegisterWindow::toggleConfirmPasswordVisibility);
}

void RegisterWindow::onRegisterClicked() {
    QString phoneNumber = phoneNumberLineEdit->text();
    QString password = passwordLineEdit->text();
    QString confirmPassword = confirmPasswordLineEdit->text();

    QRegularExpression phoneRegex("^1[3-9]\\d{9}$");
    QRegularExpression passwordRegex("^(?=.*[A-Za-z])(?=.*\\d)(?=.*[!@#$%^&*])[A-Za-z\\d!@#$%^&*]{8,16}$");

    if (!phoneRegex.match(phoneNumber).hasMatch()) {
        QMessageBox::warning(this, "注册失败", "手机号格式不正确，请输入11位中国手机号。");
        return;
    }

    if (password != confirmPassword) {
        QMessageBox::warning(this, "注册失败", "两次输入密码不匹配。");
        return;
    }

    if (!passwordRegex.match(password).hasMatch()) {
        QMessageBox::warning(this, "注册失败", "密码格式不正确，必须为8-16位字母、数字和特殊符号组合，且至少包含一个特殊字符。");
        return;
    }

    if (addUser(User(phoneNumber.toStdString().c_str(), password.toStdString().c_str()))) {
        QMessageBox::information(this, "注册成功", "注册成功！");
    }
    else {
        QMessageBox::warning(this, "注册失败", "该手机号已注册。");
    }


    phoneNumberLineEdit->clear();
    passwordLineEdit->clear();
    confirmPasswordLineEdit->clear();
}

void RegisterWindow::togglePasswordVisibility() {
    if (passwordLineEdit->echoMode() == QLineEdit::Password) {
        passwordLineEdit->setEchoMode(QLineEdit::Normal);
        togglePasswordVisibilityButton->setIcon(QIcon(":/icons/eye_open.svg"));
    } else {
        passwordLineEdit->setEchoMode(QLineEdit::Password);
        togglePasswordVisibilityButton->setIcon(QIcon(":/icons/eye_closed.svg"));
    }
}

void RegisterWindow::toggleConfirmPasswordVisibility() {
    if (confirmPasswordLineEdit->echoMode() == QLineEdit::Password) {
        confirmPasswordLineEdit->setEchoMode(QLineEdit::Normal);
        toggleConfirmPasswordVisibilityButton->setIcon(QIcon(":/icons/eye_open.svg"));
    } else {
        confirmPasswordLineEdit->setEchoMode(QLineEdit::Password);
        toggleConfirmPasswordVisibilityButton->setIcon(QIcon(":/icons/eye_closed.svg"));
    }
}