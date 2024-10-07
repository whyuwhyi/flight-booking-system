#include <ui/server/AirplaneModelManageWindow.h>
#include <data/datamanage.h>
#include <QMessageBox>
#include <QHBoxLayout>

AirplaneModelManageWindow::AirplaneModelManageWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupConnections();
    airplane_model_map.traverse([this](const AirplaneModel& model){ addModelItem(model); });
}

void AirplaneModelManageWindow::setupUI() {
    mainLayout = new QVBoxLayout(this);

    modelListWidget = new QListWidget(this);
    mainLayout->addWidget(modelListWidget);

    addModelButton = new QPushButton("添加机型", this);
    mainLayout->addWidget(addModelButton);

    setLayout(mainLayout);
}

void AirplaneModelManageWindow::setupConnections() {
    connect(addModelButton, &QPushButton::clicked, this, &AirplaneModelManageWindow::openAddModelDialog);
}

void AirplaneModelManageWindow::openAddModelDialog() {
    QDialog *addModelDialog = new QDialog(this);
    addModelDialog->setWindowTitle("添加机型");

    QVBoxLayout *layout = new QVBoxLayout(addModelDialog);
    modelNameLineEdit = createLineEdit(addModelDialog, "请输入机型名称");
    firstClassRowsLineEdit = createLineEdit(addModelDialog, "头等舱行数");
    firstClassColumnsLineEdit = createLineEdit(addModelDialog, "头等舱列数");
    businessClassRowsLineEdit = createLineEdit(addModelDialog, "商务舱行数");
    businessClassColumnsLineEdit = createLineEdit(addModelDialog, "商务舱列数");
    economyClassRowsLineEdit = createLineEdit(addModelDialog, "经济舱行数");
    economyClassColumnsLineEdit = createLineEdit(addModelDialog, "经济舱列数");

    layout->addWidget(modelNameLineEdit);
    layout->addWidget(firstClassRowsLineEdit);
    layout->addWidget(firstClassColumnsLineEdit);
    layout->addWidget(businessClassRowsLineEdit);
    layout->addWidget(businessClassColumnsLineEdit);
    layout->addWidget(economyClassRowsLineEdit);
    layout->addWidget(economyClassColumnsLineEdit);

    QPushButton *confirmAddModelButton = new QPushButton("确认添加机型", addModelDialog);
    layout->addWidget(confirmAddModelButton);
    addModelDialog->setLayout(layout);
    addModelDialog->resize(400, 300);

    connect(confirmAddModelButton, &QPushButton::clicked, this, [this, addModelDialog]() {
        if (validateModelInput()) {
            AirplaneModel model(modelNameLineEdit->text().toStdString().c_str());
            model.setCabin(AirplaneModel::FirstClass, Cabin(firstClassRowsLineEdit->text().toInt(), firstClassColumnsLineEdit->text().toInt()));
            model.setCabin(AirplaneModel::BusinessClass, Cabin(businessClassRowsLineEdit->text().toInt(), businessClassColumnsLineEdit->text().toInt()));
            model.setCabin(AirplaneModel::EconomyClass, Cabin(economyClassRowsLineEdit->text().toInt(), economyClassColumnsLineEdit->text().toInt()));

            if (addAirplaneModel(model)) {
                addModelItem(model);
                addModelDialog->accept();
            } else {
                QMessageBox::warning(this, "错误", "此机型已存在！");
            }
        }
    });

    addModelDialog->exec();
}

QLineEdit* AirplaneModelManageWindow::createLineEdit(QWidget* parent, const QString& placeholder) {
    QLineEdit *lineEdit = new QLineEdit(parent);
    lineEdit->setPlaceholderText(placeholder);
    return lineEdit;
}

bool AirplaneModelManageWindow::validateModelInput() {
    if (modelNameLineEdit->text().isEmpty() ||
        firstClassRowsLineEdit->text().toInt() <= 0 ||
        firstClassColumnsLineEdit->text().toInt() <= 0 ||
        businessClassRowsLineEdit->text().toInt() <= 0 ||
        businessClassColumnsLineEdit->text().toInt() <= 0 ||
        economyClassRowsLineEdit->text().toInt() <= 0 ||
        economyClassColumnsLineEdit->text().toInt() <= 0) {
        QMessageBox::warning(this, "错误", "所有字段都必须填写且大于零！");
        return false;
    }
    return true;
}

void AirplaneModelManageWindow::addModelItem(const AirplaneModel &model) {
    AirplaneModelItem *item = new AirplaneModelItem(model, modelListWidget);
    modelListWidget->addItem(item);
    connect(item->getDeleteButton(), &QPushButton::clicked, this, [this, item]() {
        onDeleteModel(item);
    });
}

void AirplaneModelManageWindow::onDeleteModel(AirplaneModelItem *item) {
    QMessageBox::StandardButton reply = QMessageBox::question(this, "确认删除", "确定要删除这个机型吗？", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        int row = modelListWidget->row(item);
        if (row != -1) {
            deleteAirplaneModel(item->getModelName());
            delete modelListWidget->takeItem(row);
        }
    }
}

AirplaneModelItem::AirplaneModelItem(const AirplaneModel &model, QListWidget *parent)
    : QListWidgetItem(parent) {
    deleteButton = new QPushButton("删除", parent);

    nameLabel = new QLabel(model.getName().c_str(), parent);
    capacityLabel = new QLabel(QString("总载客量: %1").arg(model.getPassengerCapacity()), parent);

    QWidget *itemWidget = new QWidget(parent);
    QHBoxLayout *layout = new QHBoxLayout(itemWidget);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(nameLabel);
    layout->addWidget(capacityLabel);
    layout->addWidget(deleteButton);

    itemWidget->setLayout(layout);
    itemWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    setSizeHint(itemWidget->sizeHint());
    parent->setItemWidget(this, itemWidget);
}

QPushButton* AirplaneModelItem::getDeleteButton() {
    return deleteButton;
}

String AirplaneModelItem::getModelName() {
    return nameLabel->text().toStdString().c_str();
}
