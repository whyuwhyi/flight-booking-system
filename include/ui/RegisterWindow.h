#pragma once

#include <QWidget>

class QLineEdit;
class QPushButton;

class RegisterWindow : public QWidget {
    Q_OBJECT

public:
    RegisterWindow(QWidget *parent = nullptr);

signals:
    void loginRequested();

private slots:
    void onRegisterClicked();

private:
    QLineEdit *phoneNumberLineEdit;
    QLineEdit *passwordLineEdit;
    QLineEdit *confirmPasswordLineEdit;
    QPushButton *registerButton;
    QPushButton *togglePasswordVisibility;
    QPushButton *toggleConfirmPasswordVisibility;
    QPushButton *backButton;

    void setupUI();
    void setupConnections();
};
