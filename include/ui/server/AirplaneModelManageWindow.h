#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QComboBox>
#include <QMessageBox>
#include <FlightSystem/AirplaneModel.h>

class AirplaneModelItem : public QListWidgetItem {
public:
    AirplaneModelItem(const AirplaneModel &model, QListWidget *parent = nullptr);
    QPushButton* getDeleteButton();
    String getModelName();

private:
    QLabel *nameLabel;
    QLabel *capacityLabel;
    QPushButton *deleteButton;
};

class AirplaneModelManageWindow : public QWidget {
    Q_OBJECT

public:
    explicit AirplaneModelManageWindow(QWidget *parent = nullptr);

private:
    void setupUI();
    void setupConnections();
    void openAddModelDialog();
    QLineEdit* createLineEdit(QWidget* parent, const QString& placeholder);
    bool validateModelInput();
    void addModelItem(const AirplaneModel &model);
    void onDeleteModel(AirplaneModelItem *item);

    QVBoxLayout *mainLayout;
    QListWidget *modelListWidget;
    QPushButton *addModelButton;
    QLineEdit *modelNameLineEdit;
    QLineEdit *firstClassRowsLineEdit;
    QLineEdit *firstClassColumnsLineEdit;
    QLineEdit *businessClassRowsLineEdit;
    QLineEdit *businessClassColumnsLineEdit;
    QLineEdit *economyClassRowsLineEdit;
    QLineEdit *economyClassColumnsLineEdit;
};

