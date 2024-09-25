#include <ui/RegisterWindow.h>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>

RegisterWindow::RegisterWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupConnections();
}

void RegisterWindow::setupUI() {
    setWindowTitle("注册");
    setFixedSize(1000, 800);

    QLabel *labelPhoneNumber = new QLabel("手机号码:", this);
    QLabel *labelPassword = new QLabel("密码:", this);
    QLabel *labelRepeatPassword = new QLabel("确认密码:", this);

    lineEditPhoneNumber = new QLineEdit(this);
    lineEditPassword = new QLineEdit(this);
    lineEditRepeatPassword = new QLineEdit(this);
    lineEditPassword->setEchoMode(QLineEdit::Password);
    lineEditRepeatPassword->setEchoMode(QLineEdit::Password);

    lineEditPhoneNumber->setPlaceholderText("请输入11位手机号码");
    lineEditPassword->setPlaceholderText("请输入密码");
    lineEditRepeatPassword->setPlaceholderText("请再次输入密码");

    buttonRegister = new QPushButton("注册", this);
    buttonBack = new QPushButton("返回登录", this);

    buttonRegister->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
    buttonBack->setStyleSheet("background-color: #FF5722; color: white; font-weight: bold;");
    lineEditPhoneNumber->setStyleSheet("font-size: 14px; padding: 6px;");
    lineEditPassword->setStyleSheet("font-size: 14px; padding: 6px;");
    lineEditRepeatPassword->setStyleSheet("font-size: 14px; padding: 6px;");

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(labelPhoneNumber, lineEditPhoneNumber);
    formLayout->addRow(labelPassword, lineEditPassword);
    formLayout->addRow(labelRepeatPassword, lineEditRepeatPassword);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonRegister);
    mainLayout->addWidget(buttonBack);

    QWidget *centeredWidget = new QWidget(this);
    centeredWidget->setFixedSize(400, 300);
    centeredWidget->setLayout(mainLayout);

    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->addWidget(centeredWidget, 0, Qt::AlignCenter);
    setLayout(outerLayout);
}


void RegisterWindow::setupConnections() {
    connect(buttonRegister, &QPushButton::clicked, this, &RegisterWindow::onRegisterClicked);
    connect(buttonBack, &QPushButton::clicked, [this]() {
        emit showLoginWindow();
    });
}

void RegisterWindow::onRegisterClicked() {
}

