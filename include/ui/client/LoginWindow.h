#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>

class LoginWindow : public QWidget {
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    void checkAutoLogin();

signals:
    void registerRequested();
    void loginSuccess();

private slots:
    void onLoginClicked();
    void togglePasswordVisibility();

private:
    void setupUI();
    void setupConnections();

    QLineEdit *phoneNumberLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QPushButton *togglePasswordVisibilityButton;
    QCheckBox *autoLoginCheckBox;
};