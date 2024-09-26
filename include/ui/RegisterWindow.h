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
    QLineEdit *lineEditPhoneNumber;
    QLineEdit *lineEditPassword;
    QLineEdit *lineEditConfirmPassword;
    QPushButton *buttonRegister;
    QPushButton *buttonBack;

    void setupUI();
    void setupConnections();
};
