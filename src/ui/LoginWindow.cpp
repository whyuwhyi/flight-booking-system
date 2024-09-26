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



// 登录窗口的UI设置
void LoginWindow::setupUI() {
    setWindowTitle("天地航空");
    setFixedSize(1000, 800);

    QLabel *labelPhoneNumber = new QLabel("手机号码:", this);
    QLabel *labelPassword = new QLabel("密码:", this);

    lineEditPhoneNumber = new QLineEdit(this);
    lineEditPassword = new QLineEdit(this);
    lineEditPassword->setEchoMode(QLineEdit::Password);
    lineEditPhoneNumber->setFixedHeight(40);  // 设置手机号码输入框的高度
    lineEditPassword->setFixedHeight(40);  // 设置密码输入框的高度
    lineEditPhoneNumber->setFixedWidth(300); // 设置手机号码输入框的宽度
    lineEditPassword->setFixedWidth(300);     // 设置密码输入框的宽度


    // 添加显示密码的图标
    QPushButton *togglePasswordVisibility = new QPushButton(this);
    togglePasswordVisibility->setIcon(QIcon(":/icons/eye_closed.png")); // 初始为闭眼图标
    togglePasswordVisibility->setFixedSize(30, 30);
    togglePasswordVisibility->setStyleSheet("border: none; background: transparent;");

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

    buttonLogin = new QPushButton("登录", this);
    registerButton = new QPushButton("注册", this);
    
    // 设置按钮样式
    buttonLogin->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
    registerButton->setStyleSheet("background-color: #FF5722; color: white; font-weight: bold;");
    
    // 设置占位符
    lineEditPhoneNumber->setPlaceholderText("11位手机号码");
    lineEditPassword->setPlaceholderText("密码");

    // 自动登录复选框
    autoLoginCheckBox = new QCheckBox("自动登录", this);
    autoLoginCheckBox->setStyleSheet("font-size: 14px;");

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(labelPhoneNumber, lineEditPhoneNumber);
    formLayout->addRow(labelPassword, lineEditPassword);
    
    // 添加图标按钮到布局
    QHBoxLayout *passwordLayout = new QHBoxLayout();
    passwordLayout->addWidget(lineEditPassword);
    passwordLayout->addWidget(togglePasswordVisibility);
    formLayout->addRow(labelPassword, passwordLayout);


    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(autoLoginCheckBox);
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
            // 如果勾选了自动登录，则调用写入用户信息的函数
            if (autoLoginCheckBox->isChecked()) {
                writeLocalUserToFile(temp_user, local_user_file);
            }
            return;
        }
        userPointer = userPointer->getNext();
    }

    QMessageBox::warning(this, "登录失败", "账号或密码错误！");

}
