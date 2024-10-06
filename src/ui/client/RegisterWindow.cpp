#include <ui//client/RegisterWindow.h>
#include <data/datamanage.h>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <String/String.h>
#include <QMessageBox>
#include <QRegularExpression>

RegisterWindow::RegisterWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupConnections();
}


void RegisterWindow::setupUI() {
    setWindowTitle("注册");
    setFixedSize(1000, 800);

    QLabel *labelPhoneNumber = new QLabel("手机号码:", this);
    QLabel *labelPassword = new QLabel("密码:", this);
    QLabel *labelConfirmPassword = new QLabel("确认密码:", this);

    phoneNumberLineEdit = new QLineEdit(this);
    passwordLineEdit = new QLineEdit(this);
    confirmPasswordLineEdit = new QLineEdit(this);
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordLineEdit->setEchoMode(QLineEdit::Password);

    phoneNumberLineEdit->setPlaceholderText("请输入11位手机号码");
    passwordLineEdit->setPlaceholderText("请输入密码");
    confirmPasswordLineEdit->setPlaceholderText("请再次输入密码");

    phoneNumberLineEdit->setFixedHeight(40);
    passwordLineEdit->setFixedHeight(40);
    confirmPasswordLineEdit->setFixedHeight(40);

    phoneNumberLineEdit->setFixedWidth(300);
    passwordLineEdit->setFixedWidth(300);
    confirmPasswordLineEdit->setFixedWidth(300);

    togglePasswordVisibility = new QPushButton(this);
    togglePasswordVisibility->setIcon(QIcon(":/icons/eye_closed.svg"));
    togglePasswordVisibility->setFixedSize(30, 30);
    togglePasswordVisibility->setStyleSheet("border: none; background: transparent;");

    toggleConfirmPasswordVisibility = new QPushButton(this);
    toggleConfirmPasswordVisibility->setIcon(QIcon(":/icons/eye_closed.svg"));
    toggleConfirmPasswordVisibility->setFixedSize(30, 30);
    toggleConfirmPasswordVisibility->setStyleSheet("border: none; background: transparent;");

    registerButton = new QPushButton("注册", this);
    backButton = new QPushButton("返回登录", this);

    registerButton->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
    backButton->setStyleSheet("background-color: #FF5722; color: white; font-weight: bold;");
    phoneNumberLineEdit->setStyleSheet("font-size: 14px; padding: 6px;");
    passwordLineEdit->setStyleSheet("font-size: 14px; padding: 6px;");
    confirmPasswordLineEdit->setStyleSheet("font-size: 14px; padding: 6px;");

    QHBoxLayout *confirmPasswordLayout = new QHBoxLayout();
    confirmPasswordLayout->addWidget(confirmPasswordLineEdit);
    confirmPasswordLayout->addWidget(toggleConfirmPasswordVisibility);

    QHBoxLayout *passwordLayout = new QHBoxLayout();
    passwordLayout->addWidget(passwordLineEdit);
    passwordLayout->addWidget(togglePasswordVisibility);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(labelPhoneNumber, phoneNumberLineEdit);
    formLayout->addRow(labelPassword, passwordLayout);
    formLayout->addRow(labelConfirmPassword, confirmPasswordLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(registerButton);
    mainLayout->addWidget(backButton);

    QWidget *centeredWidget = new QWidget(this);
    centeredWidget->setFixedSize(400, 250);
    centeredWidget->setLayout(mainLayout);

    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->addWidget(centeredWidget, 0, Qt::AlignCenter);
    setLayout(outerLayout);
}


void RegisterWindow::setupConnections() {
    connect(registerButton, &QPushButton::clicked, this, &RegisterWindow::onRegisterClicked);
    connect(backButton, &QPushButton::clicked, [this]() {
        this->close();
        emit loginRequested();
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

    connect(toggleConfirmPasswordVisibility, &QPushButton::clicked, this, [this]() {
        if (confirmPasswordLineEdit->echoMode() == QLineEdit::Password) {
            confirmPasswordLineEdit->setEchoMode(QLineEdit::Normal);
            toggleConfirmPasswordVisibility->setIcon(QIcon(":/icons/eye_open.svg"));
        } else {
            confirmPasswordLineEdit->setEchoMode(QLineEdit::Password);
            toggleConfirmPasswordVisibility->setIcon(QIcon(":/icons/eye_closed.svg"));
        }
    });
}

void RegisterWindow::onRegisterClicked() {
    String phoneNumber = phoneNumberLineEdit->text().toStdString().c_str();
    String password = passwordLineEdit->text().toStdString().c_str();
    String confirmPassword = confirmPasswordLineEdit->text().toStdString().c_str();

    QRegularExpression phoneRegex("^1[3-9]\\d{9}$");
    QRegularExpression passwordRegex("^(?=.*[A-Za-z])(?=.*\\d)(?=.*[!@#$%^&*])[A-Za-z\\d!@#$%^&*]{8,16}$");

    if(user_map.find(phoneNumber) != NULL) {
        QMessageBox::warning(this, "注册失败", "该手机号已被注册。");
            return;
    }

    if (!phoneRegex.match(phoneNumber.c_str()).hasMatch()) {
        QMessageBox::warning(this, "注册失败", "手机号格式不正确，请输入11位中国手机号。");
        return;
    }
    
    if (!(password == confirmPassword)) {
        QMessageBox::warning(this, "注册失败", "两次输入密码不匹配。");
        return;
    }

    if (!passwordRegex.match(password.c_str()).hasMatch()) {
        QMessageBox::warning(this, "注册失败", "密码格式不正确，必须为8-16位字母、数字和特殊符号组合,且至少包含一个特殊字符。");
        return;
    }

    addUser(User(phoneNumber, password));

    QMessageBox::information(this, "注册成功", "注册成功！");

    phoneNumberLineEdit->clear();
    passwordLineEdit->clear();
    confirmPasswordLineEdit->clear();
}
