#include <ui/Sidebar.h>
#include <QPushButton>
#include <QVBoxLayout>

Sidebar::Sidebar(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void Sidebar::setupUI() {
    setFixedWidth(200);

    QPushButton *buttonFlightMap = new QPushButton("航线图", this);
    QPushButton *buttonServiceCenter = new QPushButton("服务中心", this);
    QPushButton *buttonMyAccount = new QPushButton("我的", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(buttonFlightMap);
    layout->addWidget(buttonServiceCenter);
    layout->addWidget(buttonMyAccount);
    layout->addStretch();

    setLayout(layout);
}
