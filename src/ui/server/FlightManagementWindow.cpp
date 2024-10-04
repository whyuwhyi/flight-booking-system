#include <ui/server/FlightManageWindow.h>
#include <QVBoxLayout>
#include <QLabel>

FlightManageWindow::FlightManageWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupConnections();
}

FlightManageWindow::~FlightManageWindow() {}

void FlightManageWindow::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel("航班管理内容", this);
    layout->addWidget(label);
}

void FlightManageWindow::setupConnections() {
    // 连接信号和槽
}
