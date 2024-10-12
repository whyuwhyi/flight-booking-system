#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class RegisterWindow : public QWidget {
    Q_OBJECT

public:
    RegisterWindow(QWidget *parent = nullptr);

signals:
    void loginRequested();

private slots:
    void onRegisterClicked();
    void togglePasswordVisibility();
    void toggleConfirmPasswordVisibility();

private:
    void setupUI();
    void setupConnections();

    QLineEdit *phoneNumberLineEdit;
    QLineEdit *passwordLineEdit;
    QLineEdit *confirmPasswordLineEdit;
    QPushButton *registerButton;
    QPushButton *togglePasswordVisibilityButton;
    QPushButton *toggleConfirmPasswordVisibilityButton;
    QPushButton *backButton;
};