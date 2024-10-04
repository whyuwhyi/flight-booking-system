#include <ui/server/AirplaneModelManageWindow.h>
#include <data/datamanage.h>
#include <QMessageBox>
#include <QHBoxLayout>

AirplaneModelManageWindow::AirplaneModelManageWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupConnections();
    std::cout << airplane_model_map;
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
    connect(addModelButton, &QPushButton::clicked, this, [this]() {
        QDialog *addModelDialog = new QDialog(this);
        addModelDialog->setWindowTitle("添加机型");

        modelNameLineEdit = new QLineEdit(addModelDialog);
        modelNameLineEdit->setPlaceholderText("请输入机型名称");

        firstClassRowsLineEdit = new QLineEdit(addModelDialog);
        firstClassRowsLineEdit->setPlaceholderText("头等舱行数");

        firstClassColumnsLineEdit = new QLineEdit(addModelDialog);
        firstClassColumnsLineEdit->setPlaceholderText("头等舱列数");

        businessClassRowsLineEdit = new QLineEdit(addModelDialog);
        businessClassRowsLineEdit->setPlaceholderText("商务舱行数");

        businessClassColumnsLineEdit = new QLineEdit(addModelDialog);
        businessClassColumnsLineEdit->setPlaceholderText("商务舱列数");

        economyClassRowsLineEdit = new QLineEdit(addModelDialog);
        economyClassRowsLineEdit->setPlaceholderText("经济舱行数");

        economyClassColumnsLineEdit = new QLineEdit(addModelDialog);
        economyClassColumnsLineEdit->setPlaceholderText("经济舱列数");

        QPushButton *confirmAddModelButton = new QPushButton("确认添加机型", addModelDialog);

        QVBoxLayout *layout = new QVBoxLayout(addModelDialog);
        layout->addWidget(modelNameLineEdit);
        layout->addWidget(firstClassRowsLineEdit);
        layout->addWidget(firstClassColumnsLineEdit);
        layout->addWidget(businessClassRowsLineEdit);
        layout->addWidget(businessClassColumnsLineEdit);
        layout->addWidget(economyClassRowsLineEdit);
        layout->addWidget(economyClassColumnsLineEdit);
        layout->addWidget(confirmAddModelButton);
        addModelDialog->setLayout(layout);
        addModelDialog->resize(400, 300);

        connect(confirmAddModelButton, &QPushButton::clicked, this, [this, addModelDialog]() {
            QString name = modelNameLineEdit->text();
            int firstRows = firstClassRowsLineEdit->text().toInt();
            int firstColumns = firstClassColumnsLineEdit->text().toInt();
            int businessRows = businessClassRowsLineEdit->text().toInt();
            int businessColumns = businessClassColumnsLineEdit->text().toInt();
            int economyRows = economyClassRowsLineEdit->text().toInt();
            int economyColumns = economyClassColumnsLineEdit->text().toInt();

            if (name.isEmpty() || firstRows <= 0 || firstColumns <= 0 || businessRows <= 0 || businessColumns <= 0 || economyRows <= 0 || economyColumns <= 0) {
                QMessageBox::warning(this, "错误", "所有字段都必须填写且大于零！");
                return;
            }

            AirplaneModel model(name.toStdString().c_str());
            model.setCabin(AirplaneModel::FirstClass, Cabin(firstRows, firstColumns));
            model.setCabin(AirplaneModel::BusinessClass, Cabin(businessRows, businessColumns));
            model.setCabin(AirplaneModel::EconomyClass, Cabin(economyRows, economyColumns));

            if (addAirplaneModel(model)) {
                addModelItem(model);
                addModelDialog->accept();
            } else {
                QMessageBox::warning(this, "错误", "此机型已存在！！");
            }
        });

        addModelDialog->exec();
    });
}

void AirplaneModelManageWindow::addModelItem(const AirplaneModel &model) {
    AirplaneModelItem *item = new AirplaneModelItem(model, modelListWidget);
    modelListWidget->addItem(item);
    connect(item->getDeleteButton(), &QPushButton::clicked, this, [this, item]() {
        onDeleteModel(item);
    });
}

void AirplaneModelManageWindow::onDeleteModel(AirplaneModelItem *item) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除", "确定要删除这个机型吗？", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        int row = modelListWidget->row(item);
        if (row != -1) {
            deleteAirplaneModel(item->getModelName());
            delete modelListWidget->takeItem(row);
        }
    }
}

AirplaneModelManageWindow::AirplaneModelItem::AirplaneModelItem(const AirplaneModel &model, QListWidget *parent)
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

QPushButton* AirplaneModelManageWindow::AirplaneModelItem::getDeleteButton() {
    return deleteButton;
}

String AirplaneModelManageWindow::AirplaneModelItem::getModelName() {
    return nameLabel->text().toStdString().c_str();
}