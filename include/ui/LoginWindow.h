#pragma once

#include <QWidget>


class QLineEdit;
class QPushButton;
class QCheckBox;

class LoginWindow : public QWidget {
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);

signals:
    void registerRequested();
    void loginSuccess();

private slots:
    void onLoginClicked();

private:
    QLineEdit *lineEditPhoneNumber;
    QLineEdit *lineEditPassword;
    QPushButton *buttonLogin;
    QPushButton *registerButton;
    QCheckBox *autoLoginCheckBox;

    void setupUI();
    void setupConnections();
};
