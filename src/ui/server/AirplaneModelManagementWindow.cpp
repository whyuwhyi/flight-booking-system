#include <ui/AirplaneModelManageWindow.h>
#include <QVBoxLayout>
#include <QLabel>

AirplaneModelManageWindow::AirplaneModelManageWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupConnections();
}

AirplaneModelManageWindow::~AirplaneModelManageWindow() {}

void AirplaneModelManageWindow::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel("机型管理内容", this);
    layout->addWidget(label);
}

void AirplaneModelManageWindow::setupConnections() {
    // 连接信号和槽
}
