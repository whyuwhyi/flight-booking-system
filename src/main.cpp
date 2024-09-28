#include <QApplication>
#include <ui/WindowManager.h>
#include <ui/MainWindow.h>
#include <QIcon>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icons/app.svg"));
    app.setApplicationName("flight system");

    WindowManager window;
    window.initFrame();


    return app.exec();
}
