#include <QApplication>
#include <ui/WindowManager.h>
#include <ui/MainWindow.h>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    WindowManager window;
    window.show();


    return app.exec();
}
