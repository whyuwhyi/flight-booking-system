#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <FlightSystem/AirplaneModel.h>

class AirplaneModelManageWindow : public QWidget {
    Q_OBJECT

public:
    AirplaneModelManageWindow(QWidget *parent = nullptr);
    ~AirplaneModelManageWindow();

private:
    void setupUI();
    void setupConnections();

    QLineEdit *modelNameLineEdit;
    QLineEdit *firstClassRowsLineEdit;
    QLineEdit *firstClassColumnsLineEdit;
    QLineEdit *businessClassRowsLineEdit;
    QLineEdit *businessClassColumnsLineEdit;
    QLineEdit *economyClassRowsLineEdit;
    QLineEdit *economyClassColumnsLineEdit;
    QPushButton *addModelButton;
    QListWidget *modelListWidget;
    QVBoxLayout *mainLayout;

    class AirplaneModelItem : public QListWidgetItem {
    private:
        QLabel *nameLabel;
        QLabel *capacityLabel;
        QPushButton *deleteButton;
    public:
        AirplaneModelItem(const AirplaneModel &model, QListWidget *parent = nullptr);
        QPushButton* getDeleteButton();
        String getModelName();
    };

    void addModelItem(const AirplaneModel &model);
    void onDeleteModel(AirplaneModelItem* item);
};