#include <ui/server/AirplaneModelManagementWindow.h>

AirplaneModelManageWindow::AirplaneModelManageWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupConnections();
}

AirplaneModelManageWindow::~AirplaneModelManageWindow() {}

void AirplaneModelManageWindow::setupUI() {
    mainLayout = new QVBoxLayout(this);

    modelNameLineEdit = new QLineEdit(this);
    modelNameLineEdit->setPlaceholderText("请输入机型名称");
    mainLayout->addWidget(modelNameLineEdit);

    firstClassRowsLineEdit = new QLineEdit(this);
    firstClassRowsLineEdit->setPlaceholderText("头等舱行数");
    mainLayout->addWidget(firstClassRowsLineEdit);

    firstClassColumnsLineEdit = new QLineEdit(this);
    firstClassColumnsLineEdit->setPlaceholderText("头等舱列数");
    mainLayout->addWidget(firstClassColumnsLineEdit);

    businessClassRowsLineEdit = new QLineEdit(this);
    businessClassRowsLineEdit->setPlaceholderText("商务舱行数");
    mainLayout->addWidget(businessClassRowsLineEdit);

    businessClassColumnsLineEdit = new QLineEdit(this);
    businessClassColumnsLineEdit->setPlaceholderText("商务舱列数");
    mainLayout->addWidget(businessClassColumnsLineEdit);

    economyClassRowsLineEdit = new QLineEdit(this);
    economyClassRowsLineEdit->setPlaceholderText("经济舱行数");
    mainLayout->addWidget(economyClassRowsLineEdit);

    economyClassColumnsLineEdit = new QLineEdit(this);
    economyClassColumnsLineEdit->setPlaceholderText("经济舱列数");
    mainLayout->addWidget(economyClassColumnsLineEdit);

    addModelButton = new QPushButton("添加机型", this);
    mainLayout->addWidget(addModelButton);

    modelListWidget = new QListWidget(this);
    mainLayout->addWidget(modelListWidget);

    setLayout(mainLayout);
}

void AirplaneModelManageWindow::setupConnections() {
    connect(addModelButton, &QPushButton::clicked, this, [this]() {
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

        addModelItem(model);
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
            delete modelListWidget->takeItem(row);
        }
    }
}

AirplaneModelManageWindow::AirplaneModelItem::AirplaneModelItem(const AirplaneModel &model, QListWidget *parent)
    : QListWidgetItem(parent) {
    QWidget *itemWidget = new QWidget(parent);
    nameLabel = new QLabel(QString::fromStdString(model.getName().c_str()), itemWidget);
    capacityLabel = new QLabel(QString("总载客量: %1").arg(model.getPassengerCapacity()), itemWidget);
    deleteButton = new QPushButton("删除", itemWidget);

    QHBoxLayout *layout = new QHBoxLayout(itemWidget);
    layout->addWidget(nameLabel);
    layout->addWidget(capacityLabel);
    layout->addWidget(deleteButton);
    layout->setContentsMargins(0, 0, 0, 0);

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