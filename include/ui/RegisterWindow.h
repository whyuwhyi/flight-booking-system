#pragma once

#include <QWidget>

class QLineEdit;
class QPushButton;

class RegisterWindow : public QWidget {
    Q_OBJECT

public:
    RegisterWindow(QWidget *parent = nullptr);

signals:
    void showLoginWindow();

private slots:
    void onRegisterClicked();

private:
    QLineEdit *lineEditPhoneNumber;
    QLineEdit *lineEditPassword;
    QLineEdit *lineEditRepeatPassword;
    QPushButton *buttonRegister;
    QPushButton *buttonBack;

    void setupUI();
    void setupConnections();
};
