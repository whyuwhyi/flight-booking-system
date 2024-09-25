#pragma once

#include <QWidget>


class QLineEdit;
class QPushButton;

class LoginWindow : public QWidget {
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);

signals:
    void showRegisterWindow();

private slots:
    void onLoginClicked();

private:
    QLineEdit *lineEditPhoneNumber;
    QLineEdit *lineEditPassword;
    QPushButton *buttonLogin;
    QPushButton *registerButton;

    void setupUI();
    void setupConnections();
};
