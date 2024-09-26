#include <ui/RegisterWindow.h>
#include <User/UserServer.h>
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

    lineEditPhoneNumber = new QLineEdit(this);
    lineEditPassword = new QLineEdit(this);
    lineEditConfirmPassword = new QLineEdit(this);
    lineEditPassword->setEchoMode(QLineEdit::Password);
    lineEditConfirmPassword->setEchoMode(QLineEdit::Password);

    lineEditPhoneNumber->setPlaceholderText("请输入11位手机号码");
    lineEditPassword->setPlaceholderText("请输入密码");
    lineEditConfirmPassword->setPlaceholderText("请再次输入密码");

    lineEditPhoneNumber->setFixedHeight(40);
    lineEditPassword->setFixedHeight(40);
    lineEditConfirmPassword->setFixedHeight(40);

    lineEditPhoneNumber->setFixedWidth(300);
    lineEditPassword->setFixedWidth(300);
    lineEditConfirmPassword->setFixedWidth(300);

    // 添加显示密码的图标
    QPushButton *togglePasswordVisibility = new QPushButton(this);
    togglePasswordVisibility->setIcon(QIcon(":/icons/eye_closed.png")); // 初始为闭眼图标
    togglePasswordVisibility->setFixedSize(30, 30);
    togglePasswordVisibility->setStyleSheet("border: none; background: transparent;");

    QPushButton *toggleConfirmPasswordVisibility = new QPushButton(this);
    toggleConfirmPasswordVisibility->setIcon(QIcon(":/icons/eye_closed.png")); // 初始为闭眼图标
    toggleConfirmPasswordVisibility->setFixedSize(30, 30);
    toggleConfirmPasswordVisibility->setStyleSheet("border: none; background: transparent;");

    // 点击按钮切换密码可见性
    connect(togglePasswordVisibility, &QPushButton::clicked, this, [this, togglePasswordVisibility]() {
        if (lineEditPassword->echoMode() == QLineEdit::Password) {
            lineEditPassword->setEchoMode(QLineEdit::Normal);
            togglePasswordVisibility->setIcon(QIcon(":/icons/eye_open.svg")); // 开眼图标
        } else {
            lineEditPassword->setEchoMode(QLineEdit::Password);
            togglePasswordVisibility->setIcon(QIcon(":/icons/eye_closed.svg")); // 闭眼图标
        }
    });

    connect(toggleConfirmPasswordVisibility, &QPushButton::clicked, this, [this, toggleConfirmPasswordVisibility]() {
        if (lineEditConfirmPassword->echoMode() == QLineEdit::Password) {
            lineEditConfirmPassword->setEchoMode(QLineEdit::Normal);
            toggleConfirmPasswordVisibility->setIcon(QIcon(":/icons/eye_open.svg")); // 开眼图标
        } else {
            lineEditConfirmPassword->setEchoMode(QLineEdit::Password);
            toggleConfirmPasswordVisibility->setIcon(QIcon(":/icons/eye_closed.svg")); // 闭眼图标
        }
    });

    buttonRegister = new QPushButton("注册", this);
    buttonBack = new QPushButton("返回登录", this);

    buttonRegister->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
    buttonBack->setStyleSheet("background-color: #FF5722; color: white; font-weight: bold;");
    lineEditPhoneNumber->setStyleSheet("font-size: 14px; padding: 6px;");
    lineEditPassword->setStyleSheet("font-size: 14px; padding: 6px;");
    lineEditConfirmPassword->setStyleSheet("font-size: 14px; padding: 6px;");

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(labelPhoneNumber, lineEditPhoneNumber);

    // 密码输入框和眼睛图标布局
    QHBoxLayout *passwordLayout = new QHBoxLayout();
    passwordLayout->addWidget(lineEditPassword);
    passwordLayout->addWidget(togglePasswordVisibility);
    formLayout->addRow(labelPassword, passwordLayout);

    // 确认密码输入框和眼睛图标布局
    QHBoxLayout *confirmPasswordLayout = new QHBoxLayout();
    confirmPasswordLayout->addWidget(lineEditConfirmPassword);
    confirmPasswordLayout->addWidget(toggleConfirmPasswordVisibility);
    formLayout->addRow(labelConfirmPassword, confirmPasswordLayout);

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
        this->close();
        emit loginRequested();
    });
}

void RegisterWindow::onRegisterClicked() {
    String phoneNumber = lineEditPhoneNumber->text().toStdString().c_str();
    String password = lineEditPassword->text().toStdString().c_str();
    String confirmPassword = lineEditConfirmPassword->text().toStdString().c_str();

    QRegularExpression phoneRegex("^1[3-9]\\d{9}$");
    QRegularExpression passwordRegex("^(?=.*[A-Za-z])(?=.*\\d)(?=.*[!@#$%^&*])[A-Za-z\\d!@#$%^&*]{8,16}$");

    Link<User>* userPointer = user_list.getHead();

    while (userPointer != NULL) {
        if (userPointer->getElement().getPhoneNumber() == phoneNumber) {
            QMessageBox::warning(this, "注册失败", "该手机号已被注册。");
            return;
        }
        userPointer = userPointer->getNext();
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



    User newUser(phoneNumber, password);
    user_list.append(newUser);

    QMessageBox::information(this, "注册成功", "注册成功！");

    writeUserToFile(user_list, user_file);

    lineEditPhoneNumber->clear();
    lineEditPassword->clear();
    lineEditConfirmPassword->clear();
}
